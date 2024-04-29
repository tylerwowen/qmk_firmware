/* Copyright 2025
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

// Define the keycode, `QK_USER` avoids collisions with existing keycodes
enum keycodes {
    KC_CYCLE_LAYERS = QK_KB,
    SS_COPY_ALL,
    SS_PASTE_ALL,
};

// 1st layer on the cycle
#define LAYER_CYCLE_START 0
// Last layer on the cycle
#define LAYER_CYCLE_END 3

// Add the behaviour of this new keycode
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_CYCLE_LAYERS:
            // Our logic will happen on presses, nothing is done on releases
            if (!record->event.pressed) {
                // We've already handled the keycode (doing nothing), let QMK know so no further code is run unnecessarily
                return false;
            }

            uint8_t current_layer = get_highest_layer(layer_state);

            // Check if we are within the range, if not quit
            if (current_layer > LAYER_CYCLE_END || current_layer < LAYER_CYCLE_START) {
                return false;
            }

            uint8_t next_layer = current_layer + 1;
            if (next_layer > LAYER_CYCLE_END) {
                next_layer = LAYER_CYCLE_START;
            }
            layer_move(next_layer);
            return false;
        case SS_COPY_ALL:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI("ac"));
            }
            return false;
        case SS_PASTE_ALL:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI("av"));
            }
            return false;
        // Process other keycodes normally
        default:
            return true;
    }
}

// Tap Dance declarations
enum {
    TD_COPY_COPY_ALL,
    TD_PASTE_PASTE_ALL,
};

void copy_copy_all_finished(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            tap_code16(LGUI(KC_C));
            break;
        case 2:
            SEND_STRING(SS_LGUI("ac"));
            break;
    }
}

void paste_paste_all_finished(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            tap_code16(LGUI(KC_V));
            break;
        case 2:
            SEND_STRING(SS_LGUI("av"));
            break;
    }
}

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_COPY_COPY_ALL]   = ACTION_TAP_DANCE_FN(copy_copy_all_finished),
    [TD_PASTE_PASTE_ALL] = ACTION_TAP_DANCE_FN(paste_paste_all_finished)};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ortho_3x3(
        KC_1,  KC_2, KC_3,
        KC_4,  KC_5, KC_6,
        KC_7,  KC_8, KC_9
    ),
};
