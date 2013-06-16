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


static void twi_idle_callback(void) {
  // void
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
   *   PA7: 
   */
  DDRA  = 0b0101111;
  // PullUp für Eingänge
  PORTA = 0b11111111;
  /*
   * Pin-Config PortB:
   *   PB0: 
   *   PB1: 
   *   PB2: 
   *   PB3: 
   */
  DDRB  = 0b1111111;
  // PullUp für Eingänge
  PORTB = 0b11111111;

   /*  disable interrupts  */
   cli();
   
   
   /*  set clock   */
//  CLKPR = (1 << CLKPCE);  /*  enable clock prescaler update       */
//  CLKPR = 0;              /*  set clock to maximum                */

  /*  timer init  */
//  TIFR &= ~(1 << TOV0);   /*  clear timer0 overflow interrupt flag    */
//  TIMSK |= (1 << TOIE0);  /*  enable timer0 overflow interrupt        */

  /*  start timer0 by setting last 3 bits in timer0 control register B
   *  to any clock source */
  //TCCR0B = (1 << CS02) | (1 << CS00);
//  TCCR0B = (1 << CS00);

    
  // Global Interrupts aktivieren
  sei();  
}

int main(void)
{
  // initialisieren
  init();


  // start TWI (I²C) slave mode
  usi_twi_slave(0x22, 0, &twi_callback, &twi_idle_callback);

  return 0;
}


//ISR (TIMER0_OVF_vect)
//{
//}
