/**
 * @file debounce.c
 * @author Stefan Haun (tux@netz39.de)
 *
 * @brief Pattern-based de-bouncing functions
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

#include "debounce.h"

/*
 * If you change this mask, the pattern masks in the below functions must
 * be changed as well!
 */
#define DEBOUNCE_MASK 0b11000111

void debounce_init_button ( uint8_t *history )
{
    *history = 0b11111111;
}

void debounce_update_button ( uint8_t *history,
                              uint8_t pinport, uint8_t pinpin )
{
    *history = *history << 1;

    // read the button and store to history
    *history |= ( ( pinport & ( 1 << pinpin ) ) == 0 );
}

bool debounce_is_button_pressed ( uint8_t *history )
{
    bool pressed = false;
    if ( ( *history & DEBOUNCE_MASK ) == 0b00000111 ) {
        pressed = true;
        *history = 0b11111111;
    }
    return pressed;
}

bool debounce_is_button_released ( uint8_t *history )
{
    bool released = false;
    if ( ( *history & DEBOUNCE_MASK ) == 0b11000000 ) {
        released = true;
        *history = 0b00000000;
    }
    return released;
}

bool debounce_is_button_down ( uint8_t *history )
{
    return *history == 0b00000000;
}

bool debounce_is_button_up ( uint8_t *history )
{
    return *history == 0b11111111;
}
