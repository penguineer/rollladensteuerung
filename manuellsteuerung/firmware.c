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


/*
 * Output Status Byte
 * ==================
 * 
 * +------------+------+---------+-----------+
 * | Status LED | Spkr | I3C INT | Block LED |
 * | bl | r | g | beep | sw | bl | bl1 | bl2 |
 * +----+---+---+------+----+----+-----------+
 * 
 * Status LED
 * 	bl - blink
 * 	r  - red LED
 * 	g  - green LED
 * Speaker
 * 	beep - Speaker active
 * I3C INT
 * 	sw - Switch array changed
 * 	bl - Block switch changed
 * Block Switch LED bl1bl2
 * 	00 - off
 * 	01 - slow blink
 * 	10 - fast blink
 * 	11 - on
 */

volatile uint8_t _output_status_byte = 0;

#define OSB_Status_Blink 0x80
#define OSB_Status_Red   0x40
#define OSB_Status_Green 0x20
#define OSB_Beep         0x10
#define OSB_I3C_Sw       0x08
#define OSB_I3C_Bl       0x04
#define OSB_BlSwLED      0x03  // (Bits 0 and 1)

#define OSB_HAS_STATUS(st)   ((_output_status_byte & st) == st)
#define OSB_SET_STATUS(st)   (_output_status_byte |= st)
#define OSB_CLEAR_STATUS(st) (_output_status_byte &= ~st)

#define OSB_Block_Off    0x00
#define OSB_Block_Slow   0x01
#define OSB_Block_Fast   0x02
#define OSB_Block_On     0x03

#define OSB_Get_Block_Status     (_output_status_byte & OSB_BlSwLED)
#define OSB_Set_Block_Status(st) (\
	_output_status_byte = ((_output_status_byte & ~OSB_BlSwLED) | (st & 0x03)) \
    )

/*
 * Switch Array Status
 * ===================
 * 
 * +----------+----------+----------+----------+
 * | Switch 1 | Switch 2 | Switch 3 | Switch 4 | 
 * | Up | Dwn | Up | Dwn | Up | Dwn | Up | Dwn |
 * +----+-----+----+-----+----+-----+----+-----+
 * 
 * Switch N
 * 	N   - number of switch
 * 	Up  - Switch pointing up
 * 	Dwn - Switch pointing down
 */

volatile uint8_t _switch_array_status = 0;

/*
 * Block Switch Status
 * 
 * +-----------------+------+
 * | 7 bits reserved | blsw |
 * +-----------------+------+
 * 
 * blsw - Block Switch pressed
 */

volatile uint8_t _block_switch_status = 0;

#define BSS_BlockSwitch 0x01

#define BSS_HAS_STATUS(st)   ((_block_switch_status & st) == st)
#define BSS_SET_STATUS(st)   (_block_switch_status |= st)
#define BSS_CLEAR_STATUS(st) (_block_switch_status &= ~st)

/*
 * Beep Pattern, 16 Bit cycling MSB to LSB
 */

volatile uint16_t _beep_pattern = 0;

#define BEEP_PATTERN_ACTIVE (_beep_pattern)
#define BEEP_PATTERN_BEEP (_beep_pattern & 0x01)
#define DO_BEEP_PATTERN_SHIFT _beep_pattern = _beep_pattern >> 1;


inline void setBeepPattern(const uint16_t pattern) {
  // store state and disable interrupts
  const uint8_t _sreg = SREG;
  cli();

  _beep_pattern = pattern;

  // restore state
  SREG = _sreg;
}
  
  
/// Port Helper Macros
#define setPortA(mask)   (PORTA |= (mask))
#define resetPortA(mask) (PORTA &= ~(mask))
#define setPortB(mask)   (PORTB |= (mask))
#define resetPortB(mask) (PORTB &= ~(mask))

/// Manual Switch Functions
inline int getManualSwitch() {
  return (PINB & (1<<PB1)) == (1<<PB1);
}

/// Beeper Port Functions

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


/// Block Light Functions
inline void setBlockLight() {
  setPortB(1<<PB0);
  
}

inline void resetBlockLight() {
   resetPortB(1<<PB0);
}

inline void toggleBlockLight() {
  if (PINB & (1<<PB0))
    resetBlockLight();
  else
    setBlockLight();
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

inline void toggleStatus() {
  if ( (PINA & (1<<PA7)) || (PINB & (1<<PB2))) {
    resetStatusRed();
    resetStatusGreen();
  } else {
    // only turn on according to status
    if (OSB_HAS_STATUS(OSB_Status_Red))
      setStatusRed();
    if (OSB_HAS_STATUS(OSB_Status_Green))
      setStatusGreen();
  }
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
 * 
 * data (DDDD)
 */
#define CMD_RESET       0x00
#define CMD_BEEP        0x01
#define CMD_MANUAL_MODE 0x02
#define CMD_GET_SWITCH  0x03
#define CMD_I3C         0x04
#define CMD_MANUAL_SW   0x05

static void twi_callback(uint8_t buffer_size,
                         volatile uint8_t input_buffer_length, 
                         volatile const uint8_t *input_buffer,
                         volatile uint8_t *output_buffer_length, 
                         volatile uint8_t *output_buffer) {

  if (input_buffer_length) {
    const char parity = (input_buffer[0] & 0x80) >> 7;
    const char cmd  = (input_buffer[0] & 0x70) >> 4;
    const char data = input_buffer[0] & 0x0F;
    
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
      case (CMD_I3C): {
	 if (data)
	   OSB_SET_STATUS( OSB_I3C_Bl );
	 else {
	   OSB_CLEAR_STATUS( OSB_I3C_Bl );
	   OSB_CLEAR_STATUS( OSB_I3C_Sw );
	 }
	output = 1;
      }; break;
      case (CMD_BEEP): {
	setBeepPattern(data);
	output = 1;
      }; break;
      case (CMD_MANUAL_MODE): {
	OSB_Set_Block_Status(data);
	output = 1;
      }; break;
      case (CMD_MANUAL_SW): {
	output = BSS_HAS_STATUS(BSS_BlockSwitch) ? 1 : 2;
	if (data == 1)
	  BSS_CLEAR_STATUS(BSS_BlockSwitch);
	else if (data == 2)
	  BSS_SET_STATUS(BSS_BlockSwitch);
      }; break;
      case (CMD_GET_SWITCH): {
	 output = 0;
	 const uint8_t sw = _switch_array_status;
	 if (data == 1) {
	   if ( (sw & 0x10) == 0x10)
	     output = 1;
	   else if ( (sw & 0x08) == 0x08)
	     output = 2;
	   else
	     output = 3;
	 }
	 if (data == 2) {
	   if ( (sw & 0x20) == 0x20)
	     output = 1;
	   else if ( (sw & 0x04) == 0x04)
	     output = 2;
	   else
	     output = 3;
	 }
	 if (data == 3) {
	   if ( (sw & 0x40) == 0x40)
	     output = 1;
	   else if ( (sw & 0x02) == 0x02)
	     output = 2;
	   else
	     output = 3;
	 }
	 if (data == 4) {
	   if ( (sw & 0x01) == 0x01)
	     output = 1;
	   else if ( (sw & 0x80) == 0x80)
	     output = 2;
	   else
	     output = 3;
	 }
      }; break;
    }

    *output_buffer_length = 2;
    output_buffer[0] = output;
    output_buffer[1] = ~(output);
  }
  
}

uint8_t manualKeyPressed();
uint8_t getSwitchState();


static void twi_idle_callback(void) {
  // store state and disable interrupts
  const uint8_t _sreg = SREG;
  cli();


  // set status bit if manual key had been pressed
  if (manualKeyPressed()) {
    OSB_SET_STATUS(OSB_I3C_Bl);
    if (BSS_HAS_STATUS(BSS_BlockSwitch))
      BSS_CLEAR_STATUS(BSS_BlockSwitch);
    else
      BSS_SET_STATUS(BSS_BlockSwitch);
  }

  // read switch array state and notify state changes
  const uint8_t sr = getSwitchState();
  if (sr != _switch_array_status) { 
    _switch_array_status = sr;

    // notify the state change
    OSB_SET_STATUS(OSB_I3C_Sw);
  }
  
  if (i3c_state()) 
    OSB_CLEAR_STATUS( OSB_Status_Red );
  else 
    OSB_SET_STATUS( OSB_Status_Red );
  
  if ( OSB_HAS_STATUS( OSB_I3C_Bl ) || 
       OSB_HAS_STATUS( OSB_I3C_Sw ) )
     OSB_CLEAR_STATUS( OSB_Status_Green );
  else
     OSB_SET_STATUS( OSB_Status_Green );

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

  // blink and beep as start signal
  setBeepPattern(0x15);
  OSB_Set_Block_Status(OSB_Block_Fast);
  OSB_SET_STATUS( OSB_Status_Red );
  _delay_ms(500);
  OSB_SET_STATUS( OSB_Status_Green );
  _delay_ms(500);
  OSB_CLEAR_STATUS( OSB_Status_Red );
  _delay_ms(500);
  OSB_CLEAR_STATUS( OSB_Status_Green );
  OSB_Set_Block_Status( OSB_Block_Off );

  
  // start TWI (I²C) slave mode
  usi_twi_slave(0x22, 0, &twi_callback, &twi_idle_callback);

  return 0;
}


/// Timer: Block Light
volatile uint16_t block_blink = 0;
void checkBlockLight() {
  // off
  if ( OSB_Get_Block_Status == OSB_Block_Off ) {
    resetBlockLight();
  } else
  // on
  if ( OSB_Get_Block_Status == OSB_Block_On ) { 
    setBlockLight();
  } else
  // blink
  {
    if (block_blink)
      block_blink--;
      
    if (!block_blink) {
      toggleBlockLight();
      block_blink = (OSB_Get_Block_Status == OSB_Block_Slow) ? 3000 : 1000;
    }
  }
}

/// Timer: Status Light
volatile uint16_t status_blink = 0;
void checkStatusLight() {
  // blink
  if ( OSB_HAS_STATUS(OSB_Status_Blink) ) {
    if (status_blink)
      status_blink--;
  
    if (!status_blink ) {
      toggleStatus();
      status_blink = 3000;
    }
  } else {
    // green
    if (OSB_HAS_STATUS(OSB_Status_Green))
      setStatusGreen();
    else
      resetStatusGreen();
    
    // red
    if (OSB_HAS_STATUS(OSB_Status_Red))
      setStatusRed();
    else
      resetStatusRed();
  }
}

/// Timer: Manual Key
// nach http://www.mikrocontroller.net/articles/Entprellung#Softwareentprellung
#define DECHATTER_COUNTER 50

uint8_t key_state;
uint8_t key_counter;
volatile uint8_t key_press = 0;

void dechatterKey() {
  // adjust manual key chatter
  uint8_t input = !(PINB & (1<<PB1));
 
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
volatile uint8_t beep_delay = 0;
void doBeep() {
  // Call only if there is something to beep
  if (BEEP_PATTERN_ACTIVE || 
      OSB_HAS_STATUS(OSB_Beep) ) {
    if (beep_delay)
      beep_delay--;

    if (!beep_delay) {
      // next sound from beep pattern
      if (BEEP_PATTERN_BEEP)
	OSB_SET_STATUS(OSB_Beep);
      else {
	OSB_CLEAR_STATUS(OSB_Beep);
	// explicitly turn off the beeper 
	// so that we do not source current on the beeper port
	resetBeeper();
      }
      
      DO_BEEP_PATTERN_SHIFT;

      beep_delay = 250;
    }

    // toggle beeper according to settings and beep pattern
    if ( (beep_delay & 0x01) && 
         OSB_HAS_STATUS(OSB_Beep) )
	toggleBeeper();
  }
}

/// Timer: I3C Interrupt
void checkI3CInt() {
  if ( OSB_HAS_STATUS( OSB_I3C_Bl ) || 
       OSB_HAS_STATUS( OSB_I3C_Sw )   )
    i3c_stateChange();
  else
    i3c_tristate();
}

ISR (TIM0_OVF_vect)
{
  // store state and disable interrupts
  const uint8_t _sreg = SREG;
  cli();
  
  dechatterKey();
  dechatterSwitches();
  checkBlockLight();
  checkStatusLight();
  checkI3CInt();
  doBeep();
  
  // restore state
  SREG = _sreg;
}

