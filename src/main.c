#include <stdio.h> 
#include "esp_log.h"

#include "communication.h"
#include "sensor.h"
#include "memory_nvs.h"
#include "led_diode.h"
#include "connection.h"

#define DEFAULT_MINIMUM_LIGHT 0
#define DEFAULT_MAXIMUM_LIGHT 100

void mqtt_log_task(void *pvParameters) {
    wifi_init();
    init_mqtt();

    while (true) {
        printf("Sending brightness to MQTT...\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        esp_err_t err = send_led_brightness(get_led_brightness());
        if (err != ESP_OK) {
            printf("Error sending brightness to MQTT\n");
        }
    }
    vTaskDelete(NULL);
}

void app_main() {

    printf("Starting...\n");

    init_nvs();

    uint16_t minimum;
    uint16_t maximum;

    esp_err_t min_err;
    esp_err_t max_err;

    remove_from_nvs("minimum");
    remove_from_nvs("maximum");

    // if minimum value is not set in memory, set it to default
    min_err = ESP_OK;
    uint16_t min = read_from_nvs("minimum", &min_err);
    if (min_err == ESP_ERR_NOT_FOUND) {
        printf("Minimum light level not found in NVS, saving default\n");
        if (save_to_nvs("minimum", DEFAULT_MINIMUM_LIGHT) != ESP_OK) {
            printf("Error saving minimum light level to NVS\n");
        }
        minimum = DEFAULT_MINIMUM_LIGHT;
    } else {
        minimum = min;
    }

    max_err = ESP_OK;
    uint16_t max = read_from_nvs("maximum", &max_err);
    if (max_err == ESP_ERR_NOT_FOUND) {
        printf("Maximum light level not found in NVS, saving default\n");
        if (save_to_nvs("maximum", DEFAULT_MAXIMUM_LIGHT) != ESP_OK) {
            printf("Error saving maximum light level to NVS\n");
        }
        maximum = DEFAULT_MAXIMUM_LIGHT;
    } else {
        maximum = max;
    }

    i2c_master_init();
    init_led();

    if (xTaskCreate(mqtt_log_task, "mqtt_log_task", 4096, NULL, 1, NULL) != pdPASS) {
        printf("Failed to create mqtt_log_task\n");
    }

    while (true) {

        float lux = read_bh1750();


        uint8_t percentage = float_to_percentage(lux, minimum, maximum);
        // percentage = negtive_percentage(percentage);
        printf("Light Level: %.2f lx, Percentage: %d\n", lux, percentage);
        uint16_t duty = percentage_to_duty(percentage);

        smooth_set_led_brightness(duty, 1000, 50);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}