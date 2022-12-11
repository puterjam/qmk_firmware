#ifdef SIGNALRGB_SUPPORT_ENABLE
RGB_MATRIX_EFFECT(SIGNALRGB)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

bool SIGNALRGB(effect_params_t* params)
{
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    for (uint8_t i = led_min; i < led_max; i++) {
        #ifdef HIDRGB_USE_UNIVERSAL_BRIGHTNESS
        float brightness = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
        rgb_matrix_set_color(
            i,
            brightness * g_signalrgb_mode_colors[i].r,
            brightness * g_signalrgb_mode_colors[i].g,
            brightness * g_signalrgb_mode_colors[i].b
        );
        #else
        rgb_matrix_set_color(
            i,
            g_signalrgb_mode_colors[i].r,
            g_signalrgb_mode_colors[i].g,
            g_signalrgb_mode_colors[i].b
        );
        #endif
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif
