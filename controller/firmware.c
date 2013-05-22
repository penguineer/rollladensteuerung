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


inline void setPortB(char mask) {
  PORTB |= mask;
}

inline void resetPortB(char mask) {
  PORTB &= ~mask; 
}

inline void setOutput(const int output) {
   int data = output;
   
   int i;
   for (i = 0; i < 8; i++) {
     const int b = data & 0x01;
     data = data >> 1;
     
     // clear all outputs
     resetPortB((1<<PB1) | (1<<PB3) | (1<<PB4));
     
     // set DS
     setPortB(b<<PB1);     
     _delay_us(100);
     
     // shift clock
     setPortB(1<<PB4);
     _delay_us(100);
   }

     // clear all outputs
     resetPortB((1<<PB1) | (1<<PB3) | (1<<PB4));
     _delay_us(100);

   // store clock   
   setPortB(1<<PB3);
}


/*
 * I²C Datenformat:
 * 
 * CCCCDDDD
 * 
 * command (CCCC)
 * 	GetLight 0x01   Aktuellen Datenwert ausgeben
 *      SetLight 0x02   Neuen Datenwert setzen
 * 
 * data (DDDD)
 * 	1 bit blink-Status
 * 	3 bit Farbe:	0 keine
 * 			1 rot
 * 			2 grün
 */
#define CMD_GETLIGHT 0x01
#define CMD_SETLIGHT 0x02

static void twi_callback(uint8_t buffer_size,
                         volatile uint8_t input_buffer_length, 
                         volatile const uint8_t *input_buffer,
                         volatile uint8_t *output_buffer_length, 
                         volatile uint8_t *output_buffer) {
  
 /* if (input_buffer_length) {
    const int cmd  = (input_buffer[0] & 0xF0) >> 4;
    const int data = input_buffer[0] & 0x0F;
    
    switch (cmd) {
      case CMD_GETLIGHT: {
	const uint8_t state = ((is_blink == 0 ? 0 : 1) << 3) + current_color;
	output_buffer[0] = state;
	*output_buffer_length = 1;
	break;
      }
      case CMD_SETLIGHT: {
	setColor(data&0x7, data&0x8);
	*output_buffer_length=0;
	break;
      }
    }
    
  }*/
 
 //if (input_buffer_length) {
   const int data = input_buffer[0];
   setOutput(data);
 //}  
}


static void twi_idle_callback(void) {
  // void
}

void init(void) {
  /*
   * Pin-Config PortB:
   *   PB0: I2C SDA
   *   PB1: DS (Data Set, out)
   *   PB2: I2C SDC
   *   PB3: STCP (storage register clock input, out)
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
}

int main(void)
{
  // initialisieren
  init();

  // start TWI (I²C) slave mode
  usi_twi_slave(0x21, 0, &twi_callback, &twi_idle_callback);

  return 0;
}


ISR (TIMER0_OVF_vect)
{
}
