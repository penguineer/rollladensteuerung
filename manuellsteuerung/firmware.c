/*
 * Rollladen-Manuellsteuerung mit I²C-Anbindung
 * Autor: Stefan Haun <tux@netz39.de>
 * 
 * Entwickelt für ATTINY24
 * 
 * nutzt https://github.com/eriksl/usitwislave.git
 * 
 * DO NOT forget to set the fuses s.th. the controller uses the 8 MHz clock!
 */


/* define CPU frequency in MHz here if not defined in Makefile */
#ifndef F_CPU
  #define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>
#include <stdint.h>

#include "usitwislave.h"

// from http://www.rn-wissen.de/index.php/Inline-Assembler_in_avr-gcc#nop
#define nop() \
   asm volatile ("nop")


// Shift register output state
static volatile char G_output = 0;


inline void setPortA(char mask) {
  PORTA |= mask;
}

inline void resetPortA(char mask) {
  PORTA &= ~mask; 
}

inline void setPortB(char mask) {
  PORTB |= mask;
}

inline void resetPortB(char mask) {
  PORTB &= ~mask; 
}

static volatile int isManual = 0;

/// Manual Switch Functions

inline int getManualSwitch() {
  return (PINB & (1<<PB1)) == (1<<PB1);
}

/// Beep Functions

static volatile uint8_t beep = 1;

inline void setBeeper() {
  setPortA(1<<PA7);
}

inline void resetBeeper() {
   resetPortA(1<<PA7);
}

inline void toggleBeeper() {
  if (PINA & (1<<PA7))
    resetBeeper();
  else
    setBeeper();
}


/// Manual Light Functions

/*
 * 0 off
 * 1 blink
 * 2 on
 */
static volatile uint8_t lightState = 1;

inline void setManLight() {
  setPortB(1<<PB0);
}

inline void resetManLight() {
   resetPortB(1<<PB0);
}

inline void toggleManLight() {
  if (PINB & (1<<PB0))
    resetManLight();
  else
    setManLight();
}


inline void adjustManLight() {
  if (lightState)
      setManLight();
    else
      resetManLight();
}


/// Direction Switch Functions
#define PIN_Q7 PA0
#define PIN_PL PA1
#define PIN_CP PA2

inline void srTriggerParallelLoad() {
  // PL is active low
  resetPortA(1<<PIN_PL);
  nop();
  setPortA(1<<PIN_PL);
  nop();
}

inline void srTriggerClock() {
  setPortA(1<<PIN_CP);
  nop();
  resetPortA(1<<PIN_CP);
  nop();
}

// Wert des Schieberegisters auslesen
uint8_t getShiftValue() {
  uint8_t data = 0;

  // set clock to low state
  resetPortA(1<<PIN_CP);

  // pull parallel register
  srTriggerParallelLoad();

  
  uint8_t i;
  for (i = 0; i < 8; i++) {
    // get the data
    const uint8_t b = (PINA & (1 << PIN_Q7)) ? 1 : 0;

    // store bit in data
    data = data << 1;
    data += b;

    // clock signal
    srTriggerClock();
  }
  
  return data;
}

void debug_sr(uint8_t sr) {
  uint8_t data = sr;
  
  resetManLight();
  _delay_ms(1000);
  
  uint8_t i;
  for (i = 0; i < 8; i++) {
    const uint8_t b = data & 0x01;
    data = data >> 1;
    
    setManLight();
    _delay_ms(100);
    if (b)
      _delay_ms(200);
    
    resetManLight();
    
    if (!b)
      _delay_ms(200);
    
    _delay_ms(10);
  }
  
  setManLight();
  _delay_ms(1000);
}


/*
 * I²C Datenformat:
 * 
 * CCCCDDDD
 * 
 * command (CCCC)
 * 
 * data (DDDD)
 */
#define CMD_ALL_STOP  0x0
#define CMD_STOP      0x1
#define CMD_UP        0x2
#define CMD_DOWN      0x3
#define CMD_OPEN      0x4
#define CMD_CLOSE     0x5

static void twi_callback(uint8_t buffer_size,
                         volatile uint8_t input_buffer_length, 
                         volatile const uint8_t *input_buffer,
                         volatile uint8_t *output_buffer_length, 
                         volatile uint8_t *output_buffer) {

  if (input_buffer_length) {
    const char cmd  = (input_buffer[0] & 0xF0) >> 4;
    const char data = input_buffer[0] & 0x0F;
  }
}

uint8_t get_key_press();

uint8_t beep_pattern = 0;

static void twi_idle_callback(void) {
  // store state and disable interrupts
  const uint8_t _sreg = SREG;
  cli();

  
  // void
  if (get_key_press()) {
    const uint8_t sr = getShiftValue();

    if (isManual) {
      lightState = 1;
      beep_pattern = 0;
    } else {
      lightState = 2;
      beep_pattern = sr;
      //snd_delay = sr;
    }
    
    //debug_sr(sr);
    
    //isManual = (sr & 0x04) ? 0 : 1;
    isManual = !isManual;
  }

  // restore state
  SREG = _sreg;
}

void init(void) {
  /*
   * Pin-Config PortA:
   *   PA0: SR: serial data (input)
   *   PA1: SR: parallel load low-active (output)
   *   PA2: SR: clock (output)
   *   PA3: 
   *   PA4: I2C SDC
   *   PA5: INT (out)
   *   PA6: I2C SDA
   *   PA7: Beeper (out)
   */
  DDRA  = 0b11011110;
  // PullUp für Eingänge
  PORTA = 0b11111110;
  /*
   * Pin-Config PortB:
   *   PB0: Anzeige Manual Mode (out)
   *   PB1: Schalter Manuel Mode (in)
   *   PB2: 
   *   PB3: 
   */
  DDRB  = 0b1111101;
  // PullUp für Eingänge
  PORTB = 0b11111101;

   /*  disable interrupts  */
   cli();

  // Do not connect the timer overflow with the I/O port
  TCCR0A = 0;
  // Set prescaler and start the timer
  TCCR0B = (1 << CS01); // | (1 << CS02);
  // Enable timer overflow interrupt
  TIMSK0 |= (1 << TOIE0);
  TIFR0 |= (1 << TOV0);

  // Global Interrupts aktivieren
  sei();
}


int main(void)
{
  // initialisieren
  init();
  resetManLight();
  //srTriggerParallelLoad();
  //srTriggerClock();

  beep_pattern = 0b00010101;
  // blink as start signal
  int i = 5;
  while (i--) {
    setManLight();
    _delay_ms(50);
    resetManLight();
    _delay_ms(50);
  }

  // start TWI (I²C) slave mode
  usi_twi_slave(0x22, 0, &twi_callback, &twi_idle_callback);
  
  while(1)
    twi_idle_callback();
      
  return 0;
}


/// Timer: Manual Light

volatile uint16_t man_blink = 0;

void checkManualLight() {
  // off
  if (lightState == 0) {
    resetManLight();
    return;
  }

  // on
  if (lightState == 2) { 
    setManLight();
    return;
  }

  // blink
  if (lightState == 1) {
    if (man_blink)
      man_blink--;
    
    if (!man_blink) {
      toggleManLight();
      man_blink = 3000;
    }
  }
}

/// Timer: Manual Key

// nach http://www.mikrocontroller.net/articles/Entprellung#Softwareentprellung
uint8_t key_state;
uint8_t key_counter;
volatile uint8_t key_press = 0;

void dechatterKey() {
  // adjust manual key chatter
  uint8_t input = PINB & (1<<PB1);
 
  if( input != key_state ) {
    key_counter--;
    if( key_counter == 0 ) {
      key_counter = 3;
      key_state = input;
      if( input )
        key_press = 1;
    }
  }
  else
    key_counter = 3;
}

uint8_t get_key_press()
{
  uint8_t result;
 
  // store state and disable interrupts
  const uint8_t _sreg = SREG;
  cli();
  
  result = key_press;
  key_press = 0;
  
  // restore state
  SREG = _sreg;
 
  return result;
}

/// Timer: Beep

volatile uint16_t beep_delay = 0;
volatile uint16_t beep_freq = 0;


void doBeep() {
  if (beep_pattern || beep) {
    if (beep_delay)
      beep_delay--;
      
    if (beep_freq)
      beep_freq--;
    
    if (!beep_delay) {
      // next sound from beep pattern
      beep = beep_pattern & 0x01;
      beep_pattern = beep_pattern >> 1;
      beep_delay = 250;
    }
    
    if (!beep_freq) {
      if (beep)
	toggleBeeper();
      beep_freq = 2;
    }
  }
}

ISR (TIM0_OVF_vect)
{
  dechatterKey();
  checkManualLight();
  doBeep();
}

