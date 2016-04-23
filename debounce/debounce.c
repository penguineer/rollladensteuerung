/*
 * De-bouncing functions accoding to
 * http://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/
 *
 * Author: tux@netz39.de
 */

#include "debounce.h"

#define DEBOUNCE_MASK	0b11000111

void debounce_init_button(uint8_t *history) {
  *history = 0xff;
}

void debounce_update_button(uint8_t *history,
		   uint8_t pinport, uint8_t pinpin)
{
  *history = *history << 1;

  // read the button and store to history
  *history |= ((pinport & (1 << pinpin)) == 0);
}

uint8_t debounce_is_button_pressed(uint8_t *history)
{
  uint8_t pressed = 0;
  if ((*history & DEBOUNCE_MASK) == 0b00000111) {
    pressed = 1;
    *history = 0xff;
  }
  return pressed;
}

uint8_t debounce_is_button_released(uint8_t *history)
{
  uint8_t released = 0;
  if ((*history & DEBOUNCE_MASK) == 0b11000000) {
    released = 1;
    *history = 0x00;
  }
  return released;
}

uint8_t debounce_is_button_down(uint8_t *history){
  return *history == 0xff;
}

uint8_t debounce_is_button_up(uint8_t *history){
  return *history == 0x00;
}
