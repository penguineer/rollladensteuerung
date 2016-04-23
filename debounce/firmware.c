/*
 * Debounce Demo
 * Autor: Stefan Haun <tux@netz39.de>
 * 
 * Entwickelt für ATMEGA8
 * 
 * DO NOT forget to set the fuses s.th. the controller uses a 16 MHz external oscillator clock!
 */


/* define CPU frequency in MHz here if not defined in Makefile */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#include "debounce.h"


/// Port Helper Macros
#define setPortB(mask)   (PORTB |= (mask))
#define resetPortB(mask) (PORTB &= ~(mask))
#define setPortC(mask)   (PORTC |= (mask))
#define resetPortC(mask) (PORTC &= ~(mask))
#define setPortD(mask)   (PORTD |= (mask))
#define resetPortD(mask) (PORTD &= ~(mask))

/// Infrastruktur

void init(void) {
  /*
   * Pin-Config PortB:
   *   PB0: IN
   *   PB1: IN
   *   PB2: IN
   *   PB3: IN
   *   PB4: IN
   *   PB5: IN
   * 
   * Pin-Config PortC:
   *   PC0: OUT
   *   PC1: OUT
   *   PC2: IN
   *   PC3: IN
   *   PC4: IN
   *   PC5: IN
   * 
   * Pin-Config PortD:
   *   PD0: IN
   *   PD1; IN
   *   PD2: IN
   *   PD3: IN
   */
  
  DDRB  = 0b00000000;
  // PullUp für Eingänge
  PORTB = 0b00000000;

  DDRC  = 0b00000011;
  // PullUp für Eingänge
  PORTC = 0b00000000;

  DDRD  = 0b00000000;
  // PullUp für Eingänge
  PORTD = 0b00000000;

   /*  disable interrupts  */
   cli();
   
   // Interrupts einstellen
   
   /*  set clock   */
  
  // prescaler 1024, CTC
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS02) | (1 << CS00);

  // vergleichswert
  OCR1A = 0xff;
  
  // aktivieren
  TIMSK |= (1 << OCIE1A);
    
  
  
  // Global Interrupts aktivieren
  sei();  
}

static uint8_t count = 0;
static uint8_t button_history;

int main(void)
{
  // initialisieren
  debounce_init_button(&button_history);
  init();
  
  while(1) {
    if (debounce_is_button_pressed(&button_history))
      ++count;

    if (count == 0xff)
      count = 0;

    if (count & 0x01)
      setPortC(0x01);
    else
      resetPortC(0x01);

    if (count & 0x02)
      setPortC(0x02);
    else
      resetPortC(0x02);

  } // while
  
  return 0;
}


/// Interrupt-Vektoren
ISR (TIMER1_COMPA_vect)
{
  // store state and disable interrupts
  const uint8_t _sreg = SREG;
  cli();

  // record the button history
  debounce_update_button(&button_history, PINC, PC2);

  // restore state
  SREG = _sreg;  
}


