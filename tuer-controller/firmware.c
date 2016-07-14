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
#include <stdbool.h>

#include "usitwislave.h"

#include "../debounce/debounce.h"

// from http://www.rn-wissen.de/index.php/Inline-Assembler_in_avr-gcc#nop
#define nop() \
   asm volatile ("nop")

// store interrupt state and disable
#define store_SREG() \
  const uint8_t _sreg = SREG; \
  cli();

// restore interrupt state and enable, if not disabled before
#define restore_SREG() \
  SREG = _sreg;


/// Define the debounce histories
static uint8_t dbh_btnRed;
static uint8_t dbh_btnGreen;
static uint8_t dbh_sigDoor;
static uint8_t dbh_sigLock;


/// State Handling Infrastructure
/***
 * Input Status Byte (ISB)
 *
 * +-----+----+----+----+----+----+----+
 * | 7-6 | 5  | 4  | 3  | 2  | 1  | 0  |
 * | res | GB | RB | DC | LO | FC | FO |
 * +-----+----+----+----+----+----+----+
 *
 * GB Green Button active (Force-open door)
 * RB Red Button active (Force-close door)
 * DO Door Open
 * LC Lock Closed
 * FC Force Close
 * FO Force Open
 */
static uint8_t inputStatusByte = 0;

// ISB masks for setISB and clearISB
#define ISB_GB (1 << 5)
#define ISB_RB (1 << 4)
#define ISB_DO (1 << 3)
#define ISB_LC (1 << 2)
#define ISB_FC (1 << 1)
#define ISB_FO (1 << 0)

inline void setISB(const uint8_t mask) {
  inputStatusByte |= mask;
}

inline void clearISB(const uint8_t mask) {
  inputStatusByte &= ~mask;
}

/**
 * \return true iif all bits from mask are set in the ISB
 */
inline bool getMaskedISB(const uint8_t mask) {
  return (inputStatusByte & mask) ? true : false;
}

inline uint8_t getInputStatusByte() {
  return inputStatusByte;
}

void updateInputState(uint8_t *history, const uint8_t mask) {
  if (debounce_is_button_pressed(history)
      || debounce_is_button_down(history)) {
    setISB(mask);
  }
  if (debounce_is_button_released(history)
      || debounce_is_button_up(history)) {
    clearISB(mask);
  }
}

void waitForClearState(uint8_t *history, const uint8_t mask) {
  bool stateOk = false;
  while (!stateOk) {
    if (debounce_is_button_down(history)) {
      setISB(mask);
      stateOk = true;
    }
    if (debounce_is_button_up(history)) {
      clearISB(mask);
      stateOk = true;
    }
  }
}


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


/// I3C
//flag state change
inline void i3c_stateChange() {
  store_SREG();

  // port A5 as output
  DDRA |= (1 << PA5);
  // set to low
  PORTA &= ~(1 << PA5);

  restore_SREG();
}

// put to listening mode
inline void i3c_tristate() {
  store_SREG();

  // port A5 as input
  DDRA &= ~(1 << PA5);
  // no pull-up
  PORTA &= ~(1 << PA5);

  restore_SREG();
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
	// clear the force state
	clearISB(ISB_FC | ISB_FO);

	i3c_tristate();
	output = 1;
      }; break;
      case CMD_OPEN: {
	// force door open
	setISB(ISB_FO);
	clearISB(ISB_FC);

	output = 1;
      }; break;
      case CMD_CLOSE: {
	// force door closed
	// this may instantly overridden by a force-open input
	setISB(ISB_FC);
	clearISB(ISB_FO);

	output = 1;
      }; break;
      case CMD_STATE: {
	// move input state to data
	output = getInputStatusByte();
	// reset I³C interrupt
	i3c_tristate();
      }; break;
    }

    *output_buffer_length = 2;
    output_buffer[0] = output;
    output_buffer[1] = ~(output);
  }
}

/**
 * \brief update the ports based on the ISB
 */
void updatePorts() {
  // Check if the door should be open (this case has precedence)
  if (getMaskedISB(ISB_FO)) {
      setPortA(1<<PA2);
      resetPortA(1<<PA3);
      setStatusGreen();
      resetStatusRed();
  }
  // Check if door should be closed
  else if (getMaskedISB(ISB_FC)) {
      setPortA(1<<PA3);
      resetPortA(1<<PA2);
      setStatusRed();
      resetStatusGreen();
  }
  // otherwise clear everything
  else {
      setPortA(1<<PA2);
      setPortA(1<<PA3);
      resetStatusRed();
      resetStatusGreen();
  }
}

/*
 * Button update counter from the TMR0 ISR
 *
 * This counter enumerates the round-robin passes in the ISR.
 */
static uint8_t rrUpdateCounter = 0;

static void twi_idle_callback(void) {
  // decide if the idle call should be executed
  bool exec = false;

  {
    store_SREG();

    // evaluate the states only if there are at least two passes in the
    // debounce round-robin
    if (rrUpdateCounter >= 2) {
      exec = true;
      rrUpdateCounter = 0;
    }

    restore_SREG();
  }

  if (!exec)
    return;


  // store the old input state
  const uint8_t oldISB = getInputStatusByte();

  // set the ISB according to the debounce histories

  // Green Button state
  updateInputState(&dbh_btnGreen, ISB_GB);
  // Red Button state
  updateInputState(&dbh_btnRed, ISB_RB);
  // Door-closed state
  updateInputState(&dbh_sigDoor, ISB_DO);
  // Lock-open state
  updateInputState(&dbh_sigLock, ISB_LC);

  /* With the following structure the buttons override subsequent
    * commands that might be issued by the I2C master while the button
    * is pressed. Only after a button-release event I2C commands will
    * have an effect on the lock state.
    */

  /*
    * Opening the door has precedence:
    * 	The green button overrides the red button,
    *   Force-open state overrides the force-close state.
    * This way a user can always open the door.
    */

  // set the forced state based on button inputs
  // changing the door state by buttons works instantly without
  // the consent of an external application
  if (getMaskedISB(ISB_GB)) {
    // force open on green button press
    setISB(ISB_FO);
    clearISB(ISB_FC);
  } else if (getMaskedISB(ISB_RB)) {
    // force closed on red button press
    setISB(ISB_FC);
    clearISB(ISB_FO);
  }

  // Set outputs according to ISB
  updatePorts();

  // trigger a state change if the ISB has changed
  if (getInputStatusByte() != oldISB)
    i3c_stateChange();
}

/// Initialization
void init(void) {
  // Init debounce histories
  debounce_init_button(&dbh_btnRed);
  debounce_init_button(&dbh_btnGreen);
  debounce_init_button(&dbh_sigDoor);
  debounce_init_button(&dbh_sigLock);

  /*
   * Pin-Config PortA:
   *   PA0: IN  Door State (0 == Closed)
   *   PA1: IN  Lock State (0 == Open)
   *   PA2: OUT Command Force-Close
   *   PA3: OUT Command Force-Open
   *   PA4:     I2C SDC
   *   PA5:     INT (out)
   *   PA6:     I2C SDA
   *   PA7: IN  Input Button Red Close
   */
  DDRA  = 0b00001100;
  // PullUp für Eingänge, bzw pre-set für Ausgänge
  PORTA = 0b00001100;

  /*
   * Pin-Config PortB:
   *   PB0: IN  Input Button Green Open
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
  TCCR0B = (1 << CS02);
  // Enable timer overflow interrupt
  TIMSK0 |= (1 << TOIE0);
  TIFR0 |= (1 << TOV0);

  // Global Interrupts aktivieren
  sei();  
}


/// Main
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

  /*
   * Note: The above sequence generates a 2000ms pause, which should
   *       allow the dechatter-histories to settle.
   *
   *       If the blinking should be removed, replace by an appropriate
   *       sleep command!
   */

  /*
   * Cold start: we need to find out the door and lock state, as
   * they may have been set without us seeing the events.
   */
  waitForClearState(&dbh_sigDoor, ISB_DO);
  waitForClearState(&dbh_sigLock, ISB_LC);

  // set force state according to lock state
  if (getMaskedISB(ISB_LC))
    setISB(ISB_FO);
  else
    setISB(ISB_FC);

  /*
   * The state machines are calculated in twi_idle_callback.
   */
  
  // start TWI (I²C) slave mode
  usi_twi_slave(0x23, 0, &twi_callback, &twi_idle_callback);

  return 0;
}


/// Interrupt handling
// Round-Robin selector
static uint8_t rrSelect = 0;
ISR (TIM0_OVF_vect)
{
  store_SREG();
  
  /* update the debounce histories */
  // round-robin the updates, that is still fast enough
  // and otherwise the ISR gets clogged
  ++rrSelect;
  switch (rrSelect) {
    case 1: {
      // Green, door-open button
      debounce_update_button(&dbh_btnGreen, PINB, PB0);
    }; break;
    case 2: {
      // Red, door-close button
      debounce_update_button(&dbh_btnRed, PINA, PA7);
    }; break;
    case 3: {
      // door-is-closed signal
      debounce_update_button(&dbh_sigDoor, PINA, PA1);
    }; break;
    case 4: {
      // lock-is-open signal
      debounce_update_button(&dbh_sigLock, PINA, PA0);
    }; break;
    default: {
      rrSelect = 0;
      // update the round-robin counter
      ++rrUpdateCounter;
    }
  } // switch

  // Shorten the timer interrupt period
  TCNT0 = 0x80;

  restore_SREG();
}
