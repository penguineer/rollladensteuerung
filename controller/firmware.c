/*
 * Rollladen-Controller mit I²C-Anbindung
 * Autor: Stefan Haun <tux@netz39.de>
 * 
 * Entwickelt für ATTINY25
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

void set_output(const char output) {
   char data = output;
   
   char i;
   for (i = 0; i < 9; i++) {
     const char b = data & 0x01;
     data = data >> 1;
     
     // clear all outputs
     resetPortB((1<<PB3) | (1<<PB4));
     _delay_us(1);
     
     // set DS
     setPortB(b<<PB3);     
     _delay_us(1);
     
     // shift clock
     setPortB(1<<PB4);
     _delay_us(1);
   }

   // clear all outputs
   resetPortB((1<<PB3) | (1<<PB4));
}

#define SWITCH_OFF 0
#define SWITCH_UP 1
#define SWITCH_DOWN 2

const char ADDRS[8] = {0x01,0x04, 0x02,0x08, 0x10,0x40, 0x20,0x80};

void change_switch (volatile char* o, char idx, char state) {
  char d = *o;
  
  // reset
  d &= ~ADDRS[idx*2];
  d &= ~ADDRS[idx*2+1];


  // set if needed
  switch (state) {
    case SWITCH_OFF: {
      break;
    }
    case SWITCH_DOWN: {
      d |= ADDRS[idx*2+1];
      // no break – DOWN switch applies, too.
    }
    case SWITCH_UP: {
      d |= ADDRS[idx*2];
      break;
    }
    default: break; // should not happen
  }

  *o = d;
}

/// I3C

//flag state change
inline void i3c_stateChange() {
  // port PB1 as output
  DDRB |= (1 << PB1);
  // set to low
  PORTB &= ~(1 << PB1);
}

// put to listening mode
inline void i3c_tristate() {
  // port A5 as input
  DDRB &= ~(1 << PB1);
  // no pull-up
  PORTB &= ~(1 << PB1);
}

inline uint8_t i3c_state() {
  return (PINB & (1 << PB1)) >> PB1;
}

/*
 * I²C Datenformat:
 * 
 * CCCCDDDD
 * 
 * command (CCCC)
 *  All Stop  0x00   Alles anhalten
 *      Stop  0x01   Rollladen anhalten
 * 	Up    0x02   Motor hoch starten 
 *      Down  0x03   Motor runter starten
 *      Open  0x04   Rollladen komplett hochfahren
 *      Close 0x05   Rollladen komplett herunterfahren
 * 
 * data (DDDD)
 * 	Rollladen als Bit-Maske
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
    
    switch (cmd) {
      case CMD_ALL_STOP: {
	G_output = 0;
	break;
      }
      case CMD_STOP: {
	change_switch(&G_output, data, SWITCH_OFF);
	break;
      }
      case CMD_UP: {
        change_switch(&G_output, data, SWITCH_UP);
	break;
      }
      case CMD_DOWN: {
        change_switch(&G_output, data, SWITCH_DOWN);
	break;
      }
    }
    set_output(G_output);   
  }
}


static void twi_idle_callback(void) {
  // void
}

void init(void) {
  /*
   * Pin-Config PortB:
   *   PB0: I2C SDA
   *   PB1: INT (out)    [STCP (storage register clock input, out)]
   *   PB2: I2C SDC
   *   PB3: DS (Data Set, out)
   *   PB4: SHCP (shift register clock input, out)
   */
  DDRB  = 0b1111010;
  // PullUp für Eingänge
  PORTB = 0b11111010;

   /*  disable interrupts  */
   cli();
   
   
   /*  set clock   */
  CLKPR = (1 << CLKPCE);  /*  enable clock prescaler update       */
  CLKPR = 0;              /*  set clock to maximum                */

  /*  timer init  */
  TIFR &= ~(1 << TOV0);   /*  clear timer0 overflow interrupt flag    */
  TIMSK |= (1 << TOIE0);  /*  enable timer0 overflow interrupt        */

  /*  start timer0 by setting last 3 bits in timer0 control register B
   *  to any clock source */
  //TCCR0B = (1 << CS02) | (1 << CS00);
  TCCR0B = (1 << CS00);

    
  // Global Interrupts aktivieren
  sei();  
  
  i3c_tristate();
}

int main(void)
{
  // initialisieren
  init();

  set_output(0x04 + 0x08 + 0x40 + 0x80);
  _delay_ms(250);
  set_output(0);

  // start TWI (I²C) slave mode
  usi_twi_slave(0x21, 0, &twi_callback, &twi_idle_callback);

  return 0;
}


ISR (TIMER0_OVF_vect)
{
}
