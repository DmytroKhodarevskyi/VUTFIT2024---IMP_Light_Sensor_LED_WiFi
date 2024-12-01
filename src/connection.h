#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "esp_crt_bundle.h"
#include "esp_tls.h"
#include <string.h>
#include <stdio.h>


#define WIFI_SSID "iPhone Khoda"
#define WIFI_PASS "12345678"
#define MQTT_HOST "mqtt://broker.hivemq.com:1883"
#define MQTT_USERNAME "Test1"
#define MQTT_PASSWORD "Aa123123"
#define MQTT_TOPIC "XKHODA01/"

#define WIFI_CONNECTED_BIT BIT0
static EventGroupHandle_t wifi_event_group;

extern esp_mqtt_client_handle_t client;

void wifi_init();
void init_mqtt();

esp_err_t send_led_brightness(uint32_t brightness);



