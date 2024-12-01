#include <driver/ledc.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_PIN 5                // GPIO pin connected to LED
#define LEDC_CHANNEL LEDC_CHANNEL_0  // LEDC channel for PWM
#define LEDC_TIMER LEDC_TIMER_0  // Timer used by LEDC
#define LEDC_MODE LEDC_HIGH_SPEED_MODE // LEDC mode (low or high speed)
#define LEDC_FREQUENCY 5000      // PWM frequency in Hz
// #define LEDC_RESOLUTION LEDC_TIMER_13_BIT // PWM resolution in bits
#define LEDC_RESOLUTION LEDC_TIMER_13_BIT // PWM resolution in bits

void init_led();

void set_led_brightness(uint32_t brightness);
void smooth_set_led_brightness(uint32_t new_brightness, uint32_t duration, uint8_t steps);
uint32_t get_led_brightness();

uint16_t percentage_to_duty(uint8_t percentage);
uint8_t negtive_percentage (uint8_t percentage);

uint8_t float_to_percentage(float value, uint16_t min, uint16_t max);