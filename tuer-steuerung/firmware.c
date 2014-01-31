/*
 * Tür-Steuerung
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


/// Port Helper Macros
#define setPortB(mask)   (PORTB |= (mask))
#define resetPortB(mask) (PORTB &= ~(mask))
#define setPortC(mask)   (PORTC |= (mask))
#define resetPortC(mask) (PORTC &= ~(mask))
#define setPortD(mask)   (PORTD |= (mask))
#define resetPortD(mask) (PORTD &= ~(mask))

#define isEndstopClosed (((PIND & (1<<PD2)) == (1<<PD2)) ? 1 : 0)
#define isEndstopOpen (((PIND & (1<<PD3)) == (1<<PD3)) ? 1 : 0)

/*
 * Motor anhalten!
 * (Setzt Motor-Enable auf 0)
 */
inline void stopMotor() {
  resetPortC(1 << PC1);
}

/*
 * Farbanzeige (LED) setzen oder löschen
 * 
 * col      Farbe
 * state    Status an/aus
 * siehe Konstanten unter diesem Kommentar
 */
#define COL_RED   1
#define COL_GREEN 2
#define COL_OFF   0
#define COL_ON    1
inline void color(const char col, const char state) {
  const char mask = 1 << (col);
  if (state)
    setPortB(mask);
  else
    resetPortB(mask);
}

void set_output(const char output) {
  // store state and disable interrupts
  const uint8_t _sreg = SREG;
  cli();
  
  
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
  
  // restore state
  SREG = _sreg;  
}



void init(void) {
  /*
   * Pin-Config PortB:
   *   PB0: IN	Lockstate
   *   PB1: OUT	LED_R
   *   PB2: OUT LED_G
   *   PB3: MOSI
   *   PB4: MISO
   *   PB5: SCK
   * 
   * Pin-Config PortC:
   *   PC0: IN	Doorstate
   *   PC1: OUT	M_EN (Motor enable)
   *   PC2: OUT	M_IN1 (Motor Input direction 1)
   *   PC3: OUT	M_IN2 (Motor Input direction 2)
   *   PC4: IN	SET_CLOSE
   *   PC5: IN	SET_OPEN
   * 
   * Pin-Config PortD:
   *   PD0: RXD
   *   PD1; TXD
   *   PD2: IN	Endstop 1, Closed (INT0)
   *   PD3: IN	Endstop 2, Open   (INT1)
   */
  
  DDRB  = 0b00000110;
  // PullUp für Eingänge
  PORTB = 0b00000000;

  DDRC  = 0b00001110;
  // PullUp für Eingänge
  PORTC = 0b00000000;

  DDRD  = 0b00000000;
  // PullUp für Eingänge
  PORTD = 0b00000000;

   /*  disable interrupts  */
   cli();
   
   // Interrupts einstellen
   // interrupt bei Pegelwechsel
   
   // PD2 -> INT0
   MCUCR |= (1 << ISC00); // | (1 << ISC01);
   // PD3 -> INT1
   MCUCR |= (1 << ISC10); // | (1 << ISC11);
   
   // aktivieren
   GICR |= (1 << INT0) | (1 << INT1);
   
   /*  set clock   */
  //CLKPR = (1 << CLKPCE);  /*  enable clock prescaler update       */
  //CLKPR = 0;              /*  set clock to maximum                */

  /*  timer init  */
  //TIFR &= ~(1 << TOV0);   /*  clear timer0 overflow interrupt flag    */
  //TIMSK |= (1 << TOIE0);  /*  enable timer0 overflow interrupt        */

  /*  start timer0 by setting last 3 bits in timer0 control register B
   *  to any clock source */
  //TCCR0B = (1 << CS02) | (1 << CS00);
  //TCCR0B = (1 << CS00);

    
  // Global Interrupts aktivieren
  sei();  
}

int main(void)
{
  // initialisieren
  init();

  // Bereit-Meldung
  color(COL_GREEN, COL_ON);
  _delay_ms(100);
  color(COL_GREEN, COL_OFF);
  _delay_ms(100);
  color(COL_RED, COL_ON);
  _delay_ms(100);
  color(COL_RED, COL_OFF);
  _delay_ms(100);
  
  setPortC(1 << PC2);
  setPortC(1 << PC1);
  
  while(1) {
    
    
/*    if ((PIND & (1<<PD2)) == (1<<PD2))
      color(COL_GREEN, COL_ON);
    else
      color(COL_GREEN, COL_OFF);

    if ((PIND & (1<<PD3)) == (1<<PD3))
      color(COL_RED, COL_ON);
    else
      color(COL_RED, COL_OFF);
*/
    
  }
  
  return 0;
}


//ISR (TIMER0_OVF_vect)
//{
//}

ISR (INT0_vect) {
    // store state and disable interrupts
  const uint8_t _sreg = SREG;
  cli();
  
  color(COL_RED, isEndstopClosed);
  
  // stop motor
  if (isEndstopClosed) 
    stopMotor();
    
  // restore state
  SREG = _sreg;
}

ISR (INT1_vect) {
    // store state and disable interrupts
  const uint8_t _sreg = SREG;
  cli();

  color(COL_GREEN, isEndstopOpen);

  // stop motor
  if (isEndstopOpen) 
    stopMotor();

  // restore state
  SREG = _sreg;
}
