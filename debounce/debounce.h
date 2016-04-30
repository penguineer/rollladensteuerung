/**
 * @file debounce.h
 * @author Stefan Haun (tux@netz39.de)
 *
 * @brief Pattern-based de-bouncing functions
 *
 * Note: The button is evaluated with a positive logic, i.e.
 *       generates a logical 1 if the button is hold down and a
 *       logical 0 if the button is up.
 *       The history, however, is stored inverted.
 *
 * @see http://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * \brief Initialize the button to "button up".
 */
void debounce_init_button ( uint8_t *history );

/**
 * \brief Update the button history.
 * \param history Pointer to an 8-bit button history.
 * \param pinport Port of the button input, e.g. PINA
 * \param pinpin  Pin of the button input, e.g. PA2
 *
 * Note: Remember to provide the input macro for the port,
 *       i.e. PINA instead of PORTA!
 */
void debounce_update_button ( uint8_t *history,
                              uint8_t pinport, uint8_t pinpin );

/**
 * \brief Tell if the button has just been pressed.
 * \param history Pointer to an 8-bit button history.
 * \return 1 if the button has just been pressed, otherwise 0
 *
 * The press event is detected based on the history pattern, Make sure
 * to evaluate the pattern between subsequent history updates, otherwise
 * you may miss button press events!
 */
bool debounce_is_button_pressed ( uint8_t *history );

/**
 * \brief Tell if the button has just been released.
 * \param history Pointer to an 8-bit button history.
 * \return 1 if the button has just been released, otherwise 0
 *
 * The release event is detected based on the history pattern, Make sure
 * to evaluate the pattern between subsequent history updates, otherwise
 * you may miss button release events!
 */
bool debounce_is_button_released ( uint8_t *history );

/**
 * \brief Tell if the button is hold down.
 * \param history Pointer to an 8-bit button history.
 * \return 1 if the button is hold down, otherwise 0
 */
bool debounce_is_button_down ( uint8_t *history );

/**
 * \brief Tell if the button is up.
 * \param history Pointer to an 8-bit button history.
 * \return 1 if the button is up, otherwise 0
 */
bool debounce_is_button_up ( uint8_t *history );
