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

uint8_t beep_pattern = 0;

inline void setBeepPattern(const uint8_t pattern) {
  // store state and disable interrupts
  const uint8_t _sreg = SREG;
  cli();

  beep_pattern = pattern;

  // restore state
  SREG = _sreg;
}

inline void setBeeper() {
  setPortA(1<<PA3);
}

inline void resetBeeper() {
   resetPortA(1<<PA3);
}

inline void toggleBeeper() {
  if (PINA & (1<<PA3))
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

/// Status Light Functions

inline void setStatusRed() {
  setPortB(1<<PB2);
}

inline void resetStatusRed() {
   resetPortB(1<<PB2);
}

inline void setStatusGreen() {
  setPortA(1<<PA7);
}

inline void resetStatusGreen() {
   resetPortA(1<<PA7);
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
}

inline void srTriggerClock() {
  setPortA(1<<PIN_CP);
  nop();
  resetPortA(1<<PIN_CP);
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
    // store bit in data
    data <<= 1;
    data += (PINA & (1 << PIN_Q7)) >> PIN_Q7;

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
    data >>= 1;
    
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

/// I3C

//flag state change
inline void i3c_stateChange() {
  // port A5 as output
  DDRA |= (1 << PA5);
  // set to low
  PORTA &= ~(1 << PA5);
}

// put to listening mode
inline void i3c_tristate() {
  // port A5 as input
  DDRA &= ~(1 << PA5);
  // no pull-up
  PORTA &= ~(1 << PA5);
}

inline uint8_t i3c_state() {
  return (PINA & (1 << PA5)) >> PA5;
}

uint8_t registered_switch_state = 0;

/*
 * I²C Datenformat:
 * 
 * CCCCDDDD
 * 
 * command (CCCC)
 * 
 * data (DDDD)
 */
#define CMD_RESET       0x00
#define CMD_BEEP        0x01
#define CMD_MANUAL_MODE 0x02
#define CMD_GET_SWITCH  0x03

static void twi_callback(uint8_t buffer_size,
                         volatile uint8_t input_buffer_length, 
                         volatile const uint8_t *input_buffer,
                         volatile uint8_t *output_buffer_length, 
                         volatile uint8_t *output_buffer) {

  if (input_buffer_length) {
    const char cmd  = (input_buffer[0] & 0xF0) >> 4;
    const char data = input_buffer[0] & 0x0F;
    
    uint8_t output=0;

    switch (cmd) {
      case (CMD_RESET): {
	 i3c_tristate();
      }
      case (CMD_BEEP): {
	setBeepPattern(data);
      }; break;
      case (CMD_MANUAL_MODE): {
	lightState = data;
      }; break;
      case (CMD_GET_SWITCH): {
	 output = 0;
	 const uint8_t sw = registered_switch_state;
	 if (data == 1) {
	   if ( (sw & 0x10) == 0x10)
	     output = 1;
	   if ( (sw & 0x08) == 0x08)
	     output = 2;
	 }
	 if (data == 2) {
	   if ( (sw & 0x04) == 0x04)
	     output = 1;
	   if ( (sw & 0x02) == 0x02)
	     output = 2;
	 }
	 if (data == 3) {
	   if ( (sw & 0x01) == 0x01)
	     output = 1;
	   if ( (sw & 0x20) == 0x20)
	     output = 2;
	 }
	 if (data == 4) {
	   if ( (sw & 0x40) == 0x40)
	     output = 1;
	   if ( (sw & 0x80) == 0x80)
	     output = 2;
	 }
      }; break;
    }

    * output_buffer_length = 1;
    output_buffer[0] = output;
  }
  
}

uint8_t manualKeyPressed();
uint8_t getSwitchState();


static void twi_idle_callback(void) {
  // store state and disable interrupts
  const uint8_t _sreg = SREG;
  cli();

  const uint8_t sr = getSwitchState();

  // void
  if (manualKeyPressed()) {

    if (isManual) {
      lightState = 1;
      beep_pattern = 0;
    } else {
      lightState = 2;
      setBeepPattern(sr);
    }

    isManual = !isManual;
  }

  if (sr != registered_switch_state) { 
    registered_switch_state = sr;
    //setBeepPattern(0x01);
    // notify the state change
    i3c_stateChange();
  }
  
  if (i3c_state()) {
    setStatusGreen();
    resetStatusRed();
  } else {
    setStatusRed();
    resetStatusGreen();
  }

  // restore state
  SREG = _sreg;
}

void init(void) {
  /*
   * Pin-Config PortA:
   *   PA0: SR: serial data (input)
   *   PA1: SR: parallel load, low-active (output)
   *   PA2: SR: clock (output)
   *   PA3: Beeper (out)
   *   PA4: I2C SDC
   *   PA5: INT (out)
   *   PA6: I2C SDA
   *   PA7: Anzeige Status Grün
   */
  DDRA  = 0b11001110;
  // PullUp für Eingänge
  PORTA = 0b00001110;

  /*
   * Pin-Config PortB:
   *   PB0: Anzeige Manual Mode (out)
   *   PB1: Schalter Manuel Mode, low-active (in)
   *   PB2: Anzeige Status Rot
   *   PB3: RESET
   */
  DDRB  = 0b1111101;
  // PullUp für Eingänge
  PORTB = 0b11111111;

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
  
  i3c_tristate();
}


int main(void)
{
  // initialisieren
  init();
  _delay_ms(1);
  registered_switch_state = getSwitchState();

  // blink and beep as start signal
  setBeepPattern(0x15);
  int i=5;
  while (i--) {
    setManLight();
    setStatusGreen();
    _delay_ms(50);
    setStatusRed();
    _delay_ms(100);
    resetManLight();
    resetStatusGreen();
    _delay_ms(50);
    resetStatusRed();
    _delay_ms(50);
  }
  resetManLight();
  resetStatusRed();
  resetStatusGreen();

  
  // start TWI (I²C) slave mode
  usi_twi_slave(0x22, 0, &twi_callback, &twi_idle_callback);

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
#define DECHATTER_COUNTER 3

uint8_t key_state;
uint8_t key_counter;
volatile uint8_t key_press = 0;

void dechatterKey() {
  // adjust manual key chatter
  uint8_t input = PINB & (1<<PB1);
 
  if( input != key_state ) {
    key_counter--;
    if( key_counter == 0 ) {
      key_counter = DECHATTER_COUNTER;
      key_state = input;
      if( !input )
        key_press = 1;
    }
  }
  else
    key_counter = DECHATTER_COUNTER;
}

uint8_t manualKeyPressed()
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

/// Timer: Switches
volatile uint8_t switch_state = 0;
uint8_t switch_counter;

void dechatterSwitches() {
  uint8_t input = getShiftValue();
  
  if (input != switch_state) {
    switch_counter--;
    if (switch_counter == 0) {
      switch_counter = DECHATTER_COUNTER;
      switch_state = input;
    }
  } else
    switch_counter = DECHATTER_COUNTER;
}

uint8_t getSwitchState() {
  return switch_state;
}

/// Timer: Beep

static volatile uint8_t beep = 1;
volatile uint8_t beep_delay = 0;

void doBeep() {
  if (beep_pattern || beep) {
    if (beep_delay)
      beep_delay--;

    if (!beep_delay) {
      // next sound from beep pattern
      beep = beep_pattern & 0x01;
      beep_pattern = beep_pattern >> 1;
      beep_delay = 250;
    }

    if ( (beep_delay & 0x01) && beep)
	toggleBeeper();
  }
}

ISR (TIM0_OVF_vect)
{
  dechatterKey();
  dechatterSwitches();
  checkManualLight();
  doBeep();
}

