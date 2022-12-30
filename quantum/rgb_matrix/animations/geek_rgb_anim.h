#ifdef GEEKRGB_ENABLE
RGB_MATRIX_EFFECT(GEEKRGB)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#if !defined(GEEKRGB_WELCOME_ANIM_SPD)
#    define GEEKRGB_WELCOME_ANIM_SPD 255
#endif


static void set_openrgb_color(uint8_t led_index, uint8_t val){
    RGB rgb = geekrgb_get_openrgb_color(led_index);

#   ifdef GEEKRGB_USE_UNIVERSAL_BRIGHTNESS
    float brightness = (float) val / UINT8_MAX;
    rgb_matrix_set_color(led_index, rgb.r * brightness, rgb.g * brightness, rgb.b * brightness);
        #else
    rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
#   endif



}

bool GEEKRGB(effect_params_t* params){
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    void set_openrgb_colors(effect_params_t * params) {
        uint8_t time   = scale16by8(g_geekrgb_timer, GEEKRGB_WELCOME_ANIM_SPD); //FIXED SPEED
        HSV hsv        = rgb_matrix_config.hsv;
        g_geekrgb_anim_playing = (time != UINT8_MAX);

        for (uint8_t i = led_min; i < led_max; i++) {
            RGB_MATRIX_TEST_LED_FLAGS();
            if (g_geekrgb_anim_playing) { //play loop loading animation
                int16_t dx   = g_led_config.point[i].x - k_rgb_matrix_center.x;
                int16_t dy   = g_led_config.point[i].y - k_rgb_matrix_center.y;
                hsv.v = 0;

                int16_t effect = time - sqrt16(dx * dx + dy * dy);
                if (effect < 0) effect = 0;

                hsv.v = qadd8(hsv.v, effect);
                hsv.v = scale8(hsv.v, rgb_matrix_config.hsv.v);
            }

            set_openrgb_color(i, hsv.v);
        }

        if (g_geekrgb_anim_playing) g_geekrgb_timer++; //tick counter
    }

    if (geekrgb_get_mode() == HID_MODE_OPENRGB) {
        set_openrgb_colors(params);
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif
