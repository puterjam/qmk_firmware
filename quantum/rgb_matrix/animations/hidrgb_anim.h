#ifdef HIDRGB_ENABLE
RGB_MATRIX_EFFECT(HIDRGB)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

bool HIDRGB(effect_params_t* params)
{
    static uint32_t wait_timer = 0;

    inline uint32_t interval(void) {
        //return 100 / scale16by8(qadd8(rgb_matrix_config.speed, 16), 16);
        return 16;
    }

    void anim_load(uint8_t i, effect_params_t * params) {
        if (!HAS_ANY_FLAGS(g_led_config.flags[i], params->flags)) {
            return;
        }

        RGB rgb = hidrgb_get_openrgb_color(i);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);

        wait_timer = g_rgb_timer + interval();
        g_repaint_led++;
    }

    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    if (g_rgb_timer > wait_timer && DRIVER_LED_TOTAL > g_repaint_led && hidrgb_get_mode() == HID_MODE_OPENRGB) {
        anim_load(g_repaint_led, params);
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif
