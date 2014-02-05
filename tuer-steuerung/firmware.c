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

// Türschloss
#define isLockOpen       (((PINB & (1<<PB0)) == (1<<PB0)) ? 1 : 0)

// Türstatus
#define isDoorClosed     (((PINC & (1<<PC0)) == (1<<PC0)) ? 1 : 0)


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
    // schließen nur bei geschlossener Tür
    if (!isDoorClosed)
      stopMotor();
    
    // Motor darf nur "zu" drehen, wenn der Zu-Endstop nicht erreicht ist
    if (isEndstopClose)
      stopMotor();
    
    // fertig, wenn das Schloss nicht mehr offen ist
    if (!isLockOpen)
      stopMotor();    
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
    // endstop close darf nicht aktiv sein, Schloss muss offen sein und Tür muss geschlossen sein
    if (!isEndstopClose && isLockOpen && isDoorClosed) {
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

// Status
/*
 * Bits 0,1 rot
 * Bits 2,3 grün
 */

#define LED_OFF  0b00
#define LED_SLOW 0b01
#define LED_FAST 0b10
#define LED_ON   0b11

#define GET_RED     (LEDstatus & 0b0011)
#define GET_GREEN   ((LEDstatus & 0b1100) >> 2)

static char LEDstatus = 0;

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

void setLED(const char col, const char state) {
  if (col == COL_RED) {
    LEDstatus &= ~(0b0011);
    LEDstatus |= state;
  }
  if (col == COL_GREEN) {
    LEDstatus &= ~(0b1100);
    LEDstatus |= (state<<2);
  }
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
  // komplett geschlossen, wenn der Endstop erreicht 
  // oder das Schloss nicht mehr offen ist
  return isEndstopClose || !isLockOpen;
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
  _delay_ms(250);
  setLED(COL_RED, LED_FAST);
  _delay_ms(1000);
  setLED(COL_RED, LED_OFF);
  setLED(COL_GREEN, LED_FAST);
  _delay_ms(700);
  setLED(COL_GREEN, LED_OFF);
  
  
  while(1) {
    checkMotor();

    // bei offener Tür immer auch das Schloss öffnen!
    if (!isDoorClosed && !isFullyOpen()) {
      startMotor(MOTOR_OPEN);
    }
    else if (isSetOpen) {
      startMotor(MOTOR_OPEN);
    }
    else if (isSetClose) {
      startMotor(MOTOR_CLOSE);
    } else {
      // bei unklarem Status: tür auf
      startMotor(MOTOR_OPEN);
    }
  
    // Es wird nur einer der Stati rot/grün angezeigt, 
    // wenn die Tür "geschlossen" signalisiert, wird rot bevorzugt,
    // bei gleichzeitigem "offen"-status blinkt grün schnell
    // (dann liegt ein unzulässiger Zustand vor).

    // grün blinken, wenn Motor Richtung "auf"
    if (isMotorOpen)
      setLED(COL_GREEN, LED_SLOW);
    // grün, wenn komplett offen, sonst aus
    else if (isFullyOpen())
      setLED(COL_GREEN, isFullyClosed() ? LED_FAST : LED_ON);
//      setLED(COL_GREEN, isFullyClosed() ? LED_OFF : LED_ON);
    else
      setLED(COL_GREEN, LED_OFF);
    
    // rot blinken, wenn Motor Richtung "zu"
    if (isMotorClose)
      setLED(COL_RED, LED_SLOW);
    // rot, wenn komplett geschlossen, sonst aus
    else
      setLED(COL_RED, isFullyClosed() ?  LED_ON : LED_OFF);
  
    
    
  } // while
  
  return 0;
}



/// Interrupt-Vektoren

volatile uint8_t blink = 0;
volatile uint8_t phase = 0;
ISR (TIMER1_COMPA_vect)
{
  // store state and disable interrupts
  const uint8_t _sreg = SREG;
  cli();

  // another motor check here
  checkMotor();
  
  // LED setting

    // red led
  const char rst = GET_RED;
  
  if (rst == LED_ON)
    color(COL_RED, COL_ON);
  else if (rst == LED_OFF)
    color(COL_RED, COL_OFF);

  // green led
  const char gst = GET_GREEN;
  if (gst == LED_ON)
    color(COL_GREEN, COL_ON);
  else if (gst == LED_OFF)
    color(COL_GREEN, COL_OFF);

  // anything blinking
  blink++;
  if (blink > 10) {
    blink = 0;
    phase++;
  
    if (phase > 3)
      phase = 0;

    if (rst == LED_SLOW)
      color(COL_RED, phase>1);
    else if (rst == LED_FAST)
      color(COL_RED, phase%2);
  
    if (gst == LED_SLOW)
      color(COL_GREEN, phase>1);
    else if (gst == LED_FAST)
      color(COL_GREEN, phase%2);
  } // blink


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
