/*
 * De-bouncing functions accoding to
 * http://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/
 *
 * Author: tux@netz39.de
 *
 * Note: The button is evaluated with a positive logic, i.e.
 *       generates a logical 1 if the button is hold down and a
 *       logical 0 if the button is up.
 *       The history, however, is stored inverted.
 */

#pragma once

#include <stdint.h>

/**
 * \brief Initialize the button to "button up".
 */
void debounce_init_button(uint8_t *history);

/**
 * \brief Update the button history.
 * \param history Pointer to an 8-bit button history.
 * \param pinport Port of the button input, e.g. PINA
 * \param pinpin  Pin of the button input, e.g. PA2
 *
 * Note: Remember to provide the input macro for the port,
 *       i.e. PINA instead of PORTA!
 */
void debounce_update_button(uint8_t *history,
			    uint8_t pinport, uint8_t pinpin);

/**
 * \brief Tell if the button has just been pressed.
 * \param history Pointer to an 8-bit button history.
 * \return 1 if the button has just been pressed, otherwise 0
 *
 * The press event is detected based on the history pattern, Make sure
 * to evaluate the pattern between subsequent history updates, otherwise
 * you may miss button press events!
 */
uint8_t debounce_is_button_pressed(uint8_t *history);

/**
 * \brief Tell if the button has just been released.
 * \param history Pointer to an 8-bit button history.
 * \return 1 if the button has just been released, otherwise 0
 *
 * The release event is detected based on the history pattern, Make sure
 * to evaluate the pattern between subsequent history updates, otherwise
 * you may miss button release events!
 */
uint8_t debounce_is_button_released(uint8_t *history);

/**
 * \brief Tell if the button is hold down.
 * \param history Pointer to an 8-bit button history.
 * \return 1 if the button is hold down, otherwise 0
 */
uint8_t debounce_is_button_down(uint8_t *history);

/**
 * \brief Tell if the button is up.
 * \param history Pointer to an 8-bit button history.
 * \return 1 if the button is up, otherwise 0
 */
uint8_t debounce_is_button_up(uint8_t *history);
