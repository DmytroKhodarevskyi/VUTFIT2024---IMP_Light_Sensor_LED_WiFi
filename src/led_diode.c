#include "led_diode.h"

void init_led() {
    // Configure timer for LEDC
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_RESOLUTION,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // Configure LEDC channel
    ledc_channel_config_t ledc_channel = {
        .gpio_num = LED_PIN,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .duty = 0, // Start with LED off
        .hpoint = 0
    };
    ledc_channel_config(&ledc_channel);
}

void set_led_brightness(uint32_t brightness) {
    // Limit brightness to max 13-bit value
    if (brightness > 8191) {
        brightness = 8191;
    }

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, brightness);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

uint32_t get_led_brightness() {
    return ledc_get_duty(LEDC_MODE, LEDC_CHANNEL);
}

//convert percentage to 16-bit value
uint16_t percentage_to_duty(uint8_t percentage) {
    return percentage * 8191 / 100;
}

//convert float to percentage, with min and max values
uint8_t float_to_percentage(float value, uint16_t min, uint16_t max) {
    if (value < min) {
        value = min;
    } else if (value > max) {
        value = max;
    }
    return (value - min) / (max - min) * 100;
}

uint8_t negtive_percentage (uint8_t percentage) {
    return 100 - percentage;
}

void smooth_set_led_brightness(uint32_t new_brightness, uint32_t duration, uint8_t steps) {
    uint32_t current_brightness = get_led_brightness();

    if (new_brightness == current_brightness) {
        return;
    }

    if (duration == 0) {
        set_led_brightness(new_brightness);
        return;
    }

    int32_t new_signed = (int32_t)new_brightness;
    int32_t curr_signed = (int32_t)current_brightness;

    // uint32_t steps = duration / 10;
    float step_brightness = (new_signed - curr_signed) / steps;
    for (int i = 0; i < steps; i++) {
        set_led_brightness(current_brightness + step_brightness);
        current_brightness += step_brightness;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    set_led_brightness(new_brightness);
}

