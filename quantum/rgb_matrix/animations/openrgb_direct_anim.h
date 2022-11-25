#ifdef OPENRGB_ENABLE
RGB_MATRIX_EFFECT(OPENRGB_DIRECT)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

bool OPENRGB_DIRECT(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    return rgb_matrix_check_finished_leds(led_max);
}
#   endif
#endif
