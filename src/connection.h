#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include <string.h>
#include <stdio.h>


#define WIFI_SSID "your-ssid"
#define WIFI_PASS "123"
#define MQTT_HOST "mqtt://broker.hivemq.com:1883"
#define WIFI_CONNECTED_BIT BIT0
static EventGroupHandle_t wifi_event_group;

extern esp_mqtt_client_handle_t client;

void wifi_init();
void init_mqtt();

esp_err_t send_led_brightness(uint32_t brightness);



