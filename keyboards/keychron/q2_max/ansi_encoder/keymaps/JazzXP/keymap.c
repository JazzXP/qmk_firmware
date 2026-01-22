/* Copyright 2024 @ Keychron (https://www.keychron.com)
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
#include "keychron_common.h"

typedef enum {
    MAC_BASE,
    WIN_BASE,
    MAC_FN1,
    WIN_FN1,
    _FN2,
} layers;

// Define a type containing as many tapdance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_SINGLE_TAP
} td_state_t;

typedef struct {
    bool is_press_action;
    bool reset_on_next;
    td_state_t state;
} td_tap_t;

enum {
    PV_FN1_MAC,
    PV_FN1_WIN,
    PV_FN2,
};


// Function to determine the current tapdance state
td_state_t cur_dance(tap_dance_state_t *state);

// `finished` and `reset` functions for each tapdance keycode
void fn1mac_finished(tap_dance_state_t *state, void *user_data);
void fn1mac_reset(tap_dance_state_t *state, void *user_data);
void fn1win_finished(tap_dance_state_t *state, void *user_data);
void fn1win_reset(tap_dance_state_t *state, void *user_data);
void fn2_finished(tap_dance_state_t *state, void *user_data);
void fn2_reset(tap_dance_state_t *state, void *user_data);

// Virtual keys
#define VK_LCTLESC MT(MOD_LCTL, KC_ESC)
#define VK_FN1MAC  TD(PV_FN1_MAC)
#define VK_FN1WIN  TD(PV_FN1_WIN)
#define VK_FN2     TD(PV_FN2)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_ansi_67(
        KC_GRV,     KC_1,     KC_2,     KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,     KC_MINS,   KC_EQL,   KC_BSPC,          KC_MUTE,
        KC_TAB,     KC_Q,     KC_W,     KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_LBRC,   KC_RBRC,  KC_BSLS,          KC_DEL,
        VK_LCTLESC, KC_A,     KC_S,     KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,  KC_QUOT,             KC_ENT,           KC_HOME,
        KC_LSFT,              KC_Z,     KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,   KC_SLSH,             KC_RSFT, KC_UP,
        KC_LCTL,    KC_LOPTN, KC_LCMMD,                            KC_SPC,                             KC_RCMMD, VK_FN1MAC, VK_FN2,   KC_LEFT, KC_DOWN, KC_RGHT),

    [WIN_BASE] = LAYOUT_ansi_67(
        KC_GRV,     KC_1,     KC_2,     KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,     KC_MINS,   KC_EQL,   KC_BSPC,          KC_MUTE,
        KC_TAB,     KC_Q,     KC_W,     KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_LBRC,   KC_RBRC,  KC_BSLS,          KC_DEL,
        VK_LCTLESC, KC_A,     KC_S,     KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,  KC_QUOT,             KC_ENT,           KC_HOME,
        KC_LSFT,              KC_Z,     KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,   KC_SLSH,             KC_RSFT, KC_UP,
        KC_LCTL,    KC_LGUI,  KC_LALT,                             KC_SPC,                             KC_RALT,  VK_FN1WIN, VK_FN2,   KC_LEFT, KC_DOWN, KC_RGHT),

    [MAC_FN1] = LAYOUT_ansi_67(
        KC_GRV,  KC_BRID,  KC_BRIU,  KC_MCTRL,KC_LNPAD,RGB_VAD, RGB_VAI, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,          RGB_TOG,
        _______, BT_HST1,  BT_HST2,  BT_HST3, P2P4G,   _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,          _______,
        RGB_TOG, RGB_MOD,  RGB_VAI,  RGB_HUI, RGB_SAI, RGB_SPI, _______, _______, _______, _______, _______,  _______,            _______,          KC_END,
        _______,           RGB_RMOD, RGB_VAD, RGB_HUD, RGB_SAD, RGB_SPD, NK_TOGG, _______, _______, _______,  _______,            _______, _______,
        _______, _______,  _______,                             _______,                            _______,  _______,  _______,  _______, _______, _______),

    [WIN_FN1] = LAYOUT_ansi_67(
        KC_GRV,  KC_BRID,  KC_BRIU,  KC_TASK, KC_FILE, RGB_VAD, RGB_VAI, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,          RGB_TOG,
        _______, BT_HST1,  BT_HST2,  BT_HST3, P2P4G,   _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,          _______,
        RGB_TOG, RGB_MOD,  RGB_VAI,  RGB_HUI, RGB_SAI, RGB_SPI, _______, _______, _______, _______, _______,  _______,            _______,          KC_END,
        _______,           RGB_RMOD, RGB_VAD, RGB_HUD, RGB_SAD, RGB_SPD, NK_TOGG, _______, _______, _______,  _______,            _______, _______,
        _______, _______,  _______,                             _______,                            _______,  _______,  _______,  _______, _______, _______),

    [_FN2] = LAYOUT_ansi_67(
        KC_TILD, KC_F1,    KC_F2,   KC_F3,   KC_F4,    KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,   _______,          _______,
        _______, _______,  _______, _______, _______,  _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,          _______,
        KC_CAPS, _______,  _______, _______, _______,  _______, _______, _______, _______, _______, _______,  _______,            _______,          _______,
        _______,           _______, _______, _______,  _______, BAT_LVL, _______, _______, _______, _______,  _______,            _______, _______,
        _______, _______,  _______,                             _______,                            _______,  _______,  _______,  _______, _______, _______)
};

// Initialize tap structure associated with example tap dance key
static td_tap_t ql_tap_state = {
    .is_press_action = true,
    .state = TD_NONE,
    .reset_on_next = false
};

// Determine the tapdance state to return
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1 && ql_tap_state.state != TD_SINGLE_TAP) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    }

    if (state->count == 2) return TD_DOUBLE_SINGLE_TAP;
    else return TD_UNKNOWN; // Any number higher than the maximum state value you return above
}

void generic_finish(layers l, tap_dance_state_t *state) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            if (layer_state_is(l)) {
                layer_off(l);
            } else {
                layer_on(l);
            }
            ql_tap_state.reset_on_next = true;
            break;
        case TD_SINGLE_HOLD:
            layer_on(l);
            break;
        case TD_DOUBLE_SINGLE_TAP:
            if (layer_state_is(l)) {
                layer_off(l);
            } else {
                layer_on(l);
            }
            break;
        default:
            break;
    }
}
void generic_reset(layers l) {
    if (ql_tap_state.state == TD_SINGLE_HOLD) {
        layer_off(l);
    }
}
void fn1mac_finished(tap_dance_state_t *state, void *user_data) {
    generic_finish(MAC_FN1, state);
}

void fn1mac_reset(tap_dance_state_t *state, void *user_data){
    generic_reset(MAC_FN1);
}

void fn1win_finished(tap_dance_state_t *state, void *user_data) {
    generic_finish(WIN_FN1, state);
}
void fn1win_reset(tap_dance_state_t *state, void *user_data) {
    generic_reset(WIN_FN1);
}
void fn2_finished(tap_dance_state_t *state, void *user_data) {
    generic_finish(_FN2, state);
}
void fn2_reset(tap_dance_state_t *state, void *user_data) {
    generic_reset(_FN2);
}

tap_dance_action_t tap_dance_actions[] = {
    [PV_FN1_MAC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, fn1mac_finished, fn1mac_reset),
    [PV_FN1_WIN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, fn1win_finished, fn1win_reset),
    [PV_FN2]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL, fn2_finished,    fn2_reset)
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [MAC_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [WIN_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [MAC_FN1]  = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [WIN_FN1]  = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [_FN2]     = { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)},
};
#endif // ENCODER_MAP_ENABLE

// clang-format on
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_keychron_common(keycode, record)) {
        return false;
    }
    if (ql_tap_state.state == TD_SINGLE_TAP && ql_tap_state.reset_on_next && !record->event.pressed) {
        if ( layer_state_is(MAC_FN1)) { layer_off(MAC_FN1); }
        if ( layer_state_is(WIN_FN1)) { layer_off(WIN_FN1); }
        if ( layer_state_is(_FN2)) { layer_off(_FN2); }
    }
    if (ql_tap_state.reset_on_next && !record->event.pressed) {
        ql_tap_state.reset_on_next = false;
    }
    return true;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return 275;
        default:
            return TAPPING_TERM;
    }
}
void set_matrix_color_for_tap_dance(uint8_t key) {
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            rgb_matrix_set_color(key, RGB_GREEN);
            break;
        case TD_SINGLE_HOLD:
            rgb_matrix_set_color(key, RGB_PURPLE);
            break;
        case TD_DOUBLE_SINGLE_TAP:
            rgb_matrix_set_color(key, RGB_RED);
            break;
        default:
            break;
    }
}
// override current RGB effect for special indicators
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max)
{
    if (layer_state_is(MAC_FN1) || layer_state_is(WIN_FN1)) {
        set_matrix_color_for_tap_dance(61);
    }
    if (layer_state_is(_FN2)) {
        set_matrix_color_for_tap_dance(62);
    }
    return false;
}
