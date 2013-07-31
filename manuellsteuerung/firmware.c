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


// Shift register output state
static volatile char G_output = 0;


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


/// Manual Light Functions

/*
 * 0 off
 * 1 blink
 * 2 on
 */
static volatile int lightState = 1;

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

static void twi_idle_callback(void) {
  // void
  //isManual = get_key_press();
  if (get_key_press()) {
    if (isManual)
      lightState = 1;
    else
      lightState = 2;
      
    isManual = !isManual;
  }
  //adjustManLight();

}

void init(void) {
  /*
   * Pin-Config PortA:
   *   PA0: 
   *   PA1: 
   *   PA2: 
   *   PA3: 
   *   PA4: I2C SDC
   *   PA5: INT (out)
   *   PA6: I2C SDA
   *   PA7: Beeper
   */
  DDRA  = 0b0101111;
  // PullUp für Eingänge
  PORTA = 0b11111111;
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
   
   
   /*  set clock   */
  //CLKPR = (1 << CLKPCE);  /*  enable clock prescaler update       */
  //CLKPR = 0;              /*  set clock to maximum                */

  /*  timer init  */
  //TIFR1 &= ~(1 << TOV1);   /*  clear timer0 overflow interrupt flag    */
  //TIMSK1 |= (1 << TOIE1);  /*  enable timer0 overflow interrupt        */

  /*  start timer0 by setting last 3 bits in timer0 control register B
   *  to any clock source */
  //TCCR1B = (1 << CS02) | (1 << CS00);
  //TCCR1B = (1 << CS00);

  // Do not connect the timer overflow with the I/O port
  TCCR0A = 0;
  // Set prescaler and start the timer
  TCCR0B = (1 << CS00); // | (1 << CS02);
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

  // start TWI (I²C) slave mode
  usi_twi_slave(0x22, 0, &twi_callback, &twi_idle_callback);

  return 0;
}


/// Timer: Manual Light

volatile int man_blink = 0;

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
      man_blink = 1000*1000;
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
    if( key_counter == 0xFF ) {
      key_counter = 5;
      key_state = input;
      if( input )
        key_press = 1;
    }
  }
  else
    key_counter = 5;
}

uint8_t get_key_press()
{
  uint8_t result;
 
  cli();
  result = key_press;
  key_press = 0;
  sei();
 
  return result;
}

ISR (TIM0_OVF_vect)
{
  dechatterKey();
  checkManualLight();
}

