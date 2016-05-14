/*
 * Controller für Schließanlage mit I³C-Anbindung
 * Autor: Stefan Haun <tux@netz39.de>
 * 
 * Entwickelt für ATTINY44
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

#include "../debounce/debounce.h"

// from http://www.rn-wissen.de/index.php/Inline-Assembler_in_avr-gcc#nop
#define nop() \
   asm volatile ("nop")

// Define the debounce histories
static uint8_t dbh_btnRed;
static uint8_t dbh_btnGreen;
static uint8_t dbh_sigDoorClosed;
static uint8_t dbh_sigLockOpen;
  
/// Port Helper Macros
#define setPortA(mask)   (PORTA |= (mask))
#define resetPortA(mask) (PORTA &= ~(mask))
#define setPortB(mask)   (PORTB |= (mask))
#define resetPortB(mask) (PORTB &= ~(mask))


/// Status Light Functions
#define isStatusRed   (((PINB & (1<<PB2)) == (1<<PB2)) ? 1 : 0)
#define isStatusGreen (((PINB & (1<<PB1)) == (1<<PB1)) ? 1 : 0)

inline void setStatusRed() {
  setPortB(1<<PB2);
}

inline void resetStatusRed() {
   resetPortB(1<<PB2);
}

inline void setStatusGreen() {
  setPortB(1<<PB1);
}

inline void resetStatusGreen() {
   resetPortB(1<<PB1);
}

/// Extra input macros
#define isForceOpen  (((PINB & (1<<PB0)) == (1<<PB0)) ? 1 : 0)
#define isForceClose (((PINA & (1<<PA7)) == (1<<PA7)) ? 1 : 0)

/// Door Communication

#define DOOR_NONE  0
#define DOOR_OPEN  1
#define DOOR_CLOSE 2
inline void setDoorCommand(const char st) {
  resetStatusRed();
  resetStatusGreen();

  // Tür-Commands sind Low-Active!
  switch (st) {    
    case DOOR_NONE: {
      setPortA(1<<PA2);
      setPortA(1<<PA3);
    }; break;
    case DOOR_OPEN: {
      setPortA(1<<PA2);
      resetPortA(1<<PA3);
      setStatusGreen();
    }; break;
    case DOOR_CLOSE: {
      setPortA(1<<PA3);
      resetPortA(1<<PA2);
      setStatusRed();
    }; break;
  }
}

#define isCommandOpen  (((PINA & (1<<PA3)) == (1<<PA3)) ? 1 : 0)
#define isCommandClose (((PINA & (1<<PA2)) == (1<<PA2)) ? 1 : 0)
#define isDoorClosed   (((PINA & (1<<PA1)) == (1<<PA1)) ? 1 : 0)
#define isLockOpen     (((PINA & (1<<PA0)) == (1<<PA0)) ? 1 : 0)


/// State Handling Infrastructure
/***
 * Input Status Byte
 * +-----+----+----+----+----+
 * | 7-4 | 3  | 2  | 1  | 0  |
 * | res | DC | LO | FC | FO |
 * +-----+----+----+----+----+
 * DC Door Closed
 * LO Lock Open
 * FC Force Close
 * FO Force Open
 */
char getInputs() {
  return (isDoorClosed << 3) |
         (isLockOpen   << 2) |
         (isForceClose << 1) |
         (isForceOpen);
}

// Filled by the dechatter function (see interrupt functions below)
uint8_t getInputState();


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

/*
 * I²C Datenformat:
 * 
 * CCCCDDDD
 * 
 * command (CCCC)
 * data (DDDD)
 * 
 * für Debug-Ansteuerung:
 * CMD_RESET       (I²C 00)
 * CMD_OPEN        (I²C 90)
 * CMD_CLOSE       (I²C A0)
 * CMD_STATE       (I²C 30)
 */
#define CMD_RESET       0x00
#define CMD_OPEN        0x01
#define CMD_CLOSE       0x02
#define CMD_STATE       0x03

static void twi_callback(uint8_t buffer_size,
                         volatile uint8_t input_buffer_length, 
                         volatile const uint8_t *input_buffer,
                         volatile uint8_t *output_buffer_length, 
                         volatile uint8_t *output_buffer) {

  if (input_buffer_length) {
    const char parity = (input_buffer[0] & 0x80) >> 7;
    const char cmd  = (input_buffer[0] & 0x70) >> 4;
    //const char data = input_buffer[0] & 0x0F;
    
    // check parity
    char v = input_buffer[0] & 0x7F;
    char c;
    for (c = 0; v; c++)
      v &= v-1;
    c &= 1;
    
    // some dummy output value, as 0 states an error
    uint8_t output=0;

    // only check if parity matches
    if (parity == c)
    switch (cmd) {
      // handle commands
      case CMD_RESET: {
	setDoorCommand(DOOR_NONE);
	i3c_tristate();
	output = 1;
      }; break;
      case CMD_OPEN: {
	setDoorCommand(DOOR_OPEN);
	output = 1;
      }; break;
      case CMD_CLOSE: {
	// this may instantly overridden by a force-open input
	setDoorCommand(DOOR_CLOSE);
	output = 1;
      }; break;
      case CMD_STATE: {
	// move input state to data
	output = getInputState();
	// reset I³C interrupt
	i3c_tristate();
      }; break;
    }

    *output_buffer_length = 2;
    output_buffer[0] = output;
    output_buffer[1] = ~(output);
  }
  
}

static void twi_idle_callback(void) {
  // store state and disable interrupts
  const uint8_t _sreg = SREG;
  cli();

  // changing the door state works instantly without the consent
  // of an external operator
  // opening the door has precendence
  if (isForceOpen)
    setDoorCommand(DOOR_OPEN);
  else if (isForceClose)
    setDoorCommand(DOOR_CLOSE);
  
  // restore state
  SREG = _sreg;
}

void init(void) {
  // Init debounce histories
  debounce_init_button(&dbh_btnRed);
  debounce_init_button(&dbh_btnGreen);
  debounce_init_button(&dbh_sigDoorClosed);
  debounce_init_button(&dbh_sigLockOpen);

  /*
   * Pin-Config PortA:
   *   PA0: IN  Door State (1 == Closed)
   *   PA1: IN  Lock State (1 == Open)
   *   PA2: OUT Command Close
   *   PA3: OUT Command Open
   *   PA4:     I2C SDC
   *   PA5:     INT (out)
   *   PA6:     I2C SDA
   *   PA7: IN  Input Force Close
   */
  DDRA  = 0b00001100;
  // PullUp für Eingänge, bzw pre-set für Ausgänge
  PORTA = 0b00001100;

  /*
   * Pin-Config PortB:
   *   PB0: IN  Input Force Open
   *   PB1: OUT Status LED
   *   PB2: OUT Status LED
   *   PB3: RESET
   */
  DDRB  = 0b00000110;
  // PullUp für Eingänge
  PORTB = 0b00000000;

  // set I3C INT to tristate
  i3c_tristate();

  /*  disable interrupts  */
  cli();

  // Do not connect the timer overflow with the I/O port
  TCCR0A = 0;
  // Set prescaler and start the timer
  TCCR0B = (1 << CS01);
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
  _delay_ms(1);

  // blink as start signal
  setStatusRed();
  _delay_ms(500);
  setStatusGreen();
  _delay_ms(500);
  resetStatusRed();
  _delay_ms(500);
  resetStatusGreen();
  _delay_ms(500);
  
  // start TWI (I²C) slave mode
  usi_twi_slave(0x23, 0, &twi_callback, &twi_idle_callback);

  return 0;
}

/// Timer: Switches
// nach http://www.mikrocontroller.net/articles/Entprellung#Softwareentprellung
#define DECHATTER_COUNTER 50

volatile uint8_t input_state = 0;
uint8_t input_counter;

void dechatterSwitches() {
  uint8_t input = getInputs();
  
  if (input != input_state) {
    input_counter--;
    if (input_counter == 0) {
      input_counter = DECHATTER_COUNTER;
      input_state = input;
      // note change on I³C
      i3c_stateChange();
    }
  } else
    input_counter = DECHATTER_COUNTER;
}

uint8_t getInputState() {
  return input_state;
}

ISR (TIM0_OVF_vect)
{
  // store state and disable interrupts
  const uint8_t _sreg = SREG;
  cli();
  
  dechatterSwitches();

  // update the debounce histories
  debounce_update_button(&dbh_btnRed, PINA, PA7);
  debounce_update_button(&dbh_btnGreen, PINB, PB0);
  debounce_update_button(&dbh_sigDoorClosed, PINA, PA1);
  debounce_update_button(&dbh_sigLockOpen, PINA, PA0);
  
  // restore state
  SREG = _sreg;
}

