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

/// Eingänge

// Endstop-Schalter
#define isEndstopClose   (((PIND & (1<<PD2)) == (1<<PD2)) ? 1 : 0)
#define isEndstopOpen    (((PIND & (1<<PD3)) == (1<<PD3)) ? 1 : 0)

// Command-Eingänge (invertiert!)
#define isSetClose       (((PINC & (1<<PC4)) == (1<<PC4)) ? 0 : 1)
#define isSetOpen        (((PINC & (1<<PC5)) == (1<<PC5)) ? 0 : 1)




/// Motor-Funktionen

// Status Motor-Ansteuerung zurückgeben
#define isHBridgeActive  (((PINC & (1<<PC1)) == (1<<PC1)) ? 1 : 0)
#define isMotorOpen      (((PINC & (1<<PC2)) == (1<<PC2)) ? 1 : 0)
#define isMotorClose     (((PINC & (1<<PC3)) == (1<<PC3)) ? 1 : 0)


/*
 * Motor anhalten!
 */
void stopMotor() {
  resetPortC((1 << PC2) | (1 << PC3));
  // aktives Bremsen passiert nur, wenn die H-Brücke aktiviert ist
  // -> Enable 250ms später löschen
  _delay_ms(250);
  resetPortC((1 << PC1));
}

/*
 * Motor-Stellung und Schalter prüfen, ggf. Motor stoppen.
 */
void checkMotor() {
  // Fall: Motor "auf"
  if (isMotorOpen) {
    // Motor darf nur "auf" drehen, wenn der Auf-Endstop nicht erreicht ist
    if (isEndstopOpen)
      stopMotor();
  }
  
  // Fall: Motor "zu"
  if (isMotorClose) {
    // Motor darf nur "zu" drehen, wenn der Zu-Endstop nicht erreicht ist
    if (isEndstopClose)
      stopMotor();
    //TODO Schließen nur bei geschlossener Tür und offenem Schloss
  }  
}

/*
 * Motor starten
 * direction      Drehrichtung "zu" oder "auf"
 * siehe Konstanten unter diesem Kommentar
 */
#define MOTOR_CLOSE 1
#define MOTOR_OPEN  2
void startMotor(const char direction) {
  // Motor Close
  if (direction == MOTOR_CLOSE) {
    // endstop close darf nicht aktiv sein
    if (!isEndstopClose) {
      // Richtung einstellen
      resetPortC(1 << PC2);
      setPortC(1 << PC3);
      // H-Brücke aktivieren
      setPortC(1 << PC1);
    }
  }

  // Motor Open
  if (direction == MOTOR_OPEN) {
    // endstop open darf nicht aktiv sein
    if (!isEndstopOpen) {
      // Richtung einstellen
      resetPortC(1 << PC3);
      setPortC(1 << PC2);
      // H-Brücke aktivieren
      setPortC(1 << PC1);
    }
  }

  // abschließender Check
  checkMotor();
}


/// Anzeige-Funktionen (LED)
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
void color(const char col, const char state) {
  const char mask = 1 << (col);
  if (state)
    setPortB(mask);
  else
    resetPortB(mask);
}


/// Zustand

/*
 * Prüfen, ob das Schloss komplett geöffnet ist
 * Return: 0, wenn noch nicht komplett geöffnet, sonst ungleich 0
 */
char isFullyOpen() {
  // komplett geöffnet, wenn der Endstop erreicht ist
  return isEndstopOpen;
}

/*
 * Prüfen, ob das Schloss komplett geschlossen ist
 * Return: 0, wenn noch nicht komplett geschlossen, sonst ungleich 0
 */
char isFullyClosed() {
  // Vorerst; komplett geschlossen, wenn der Endstop erreicht ist
  return isEndstopClose;
  
  //TODO auch vom Schloss-Status abhängig machen
}


/// Infrastruktur

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
  PORTC = 0b00110000;

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

int main(void)
{
  // initialisieren
  init();

  // Bereit-Meldung
  _delay_ms(100);
  color(COL_RED, COL_ON);
  _delay_ms(100);
  color(COL_RED, COL_OFF);
  _delay_ms(100);
  color(COL_GREEN, COL_ON);
  _delay_ms(100);
  color(COL_GREEN, COL_OFF);
  _delay_ms(100);
  
  
  while(1) {
    checkMotor();

    if (isSetOpen) {
      startMotor(MOTOR_OPEN);
    }
    else if (isSetClose) {
      startMotor(MOTOR_CLOSE);
    }
    else {
      stopMotor();
    }
  
    // grün, wenn komplett offen
    color(COL_GREEN, isFullyOpen());
    
    // rot, wenn komplett geschlossen
    color(COL_RED, isFullyClosed());
    
  } // while
  
  return 0;
}



/// Interrupt-Vektoren


ISR (TIMER1_COMPA_vect)
{
  // store state and disable interrupts
  const uint8_t _sreg = SREG;
  cli();

  // another motor check here
  checkMotor();
  
  // LED setting

  // restore state
  SREG = _sreg;  
}

ISR (INT0_vect) {
  // store state and disable interrupts
  const uint8_t _sreg = SREG;
  cli();
    
  checkMotor();

  // restore state
  SREG = _sreg;
}

ISR (INT1_vect) {
  // store state and disable interrupts
  const uint8_t _sreg = SREG;
  cli();

  checkMotor();

  // restore state
  SREG = _sreg;
}
