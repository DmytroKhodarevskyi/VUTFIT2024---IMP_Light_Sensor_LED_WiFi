#include "connection.h"

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        esp_wifi_connect();
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        printf("Connected to Wi-Fi\n");
    }
}

void mqtt_handler(void *handler_args, esp_event_base_t base, int32_t event_id,
                  void *event_data)
{
    switch (event_id)
    {
    case MQTT_EVENT_CONNECTED:
        printf("MQTT: Connected\n");
        break;

    case MQTT_EVENT_DISCONNECTED:
        printf("MQTT: Disconnected\n");
        break;

    default:
        break;
    }
}

void wifi_init()
{
    wifi_event_group = xEventGroupCreate();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();

    // Wait for Wi-Fi connection
    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY);
}

esp_mqtt_client_handle_t client = NULL;

void init_mqtt()
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_HOST,

    //    .broker = {
    //         .address = {
    //             .uri = "mqtts://94ebb2070b1540a3ae7a6b8bed77731d.s1.eu.hivemq.cloud:8883",
    //             .verification = {
    //                 .skip_cert_common_name_check = true, // Skip Common Name (CN) check
    //             },
    //         },
    //     },

    // .broker.verification.certificate
    // .credentials.client_id = MQTT_USERNAME,

    // .credentials.username = MQTT_USERNAME,
    // .credentials.authentication.password = MQTT_PASSWORD,

    };

    // const esp_tls_cfg_t tls_cfg = {
        // .
    // };

    client = esp_mqtt_client_init(&mqtt_cfg);
    if (client == NULL)
    {
        printf("Error: Failed to initialize MQTT client.\n");
        return;
    }

    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_handler, client);
    esp_mqtt_client_start(client);
}

esp_err_t send_led_brightness(uint32_t brightness)
{
    char brightness_str[6], message_topic[64];
    unsigned int int_brightness = (unsigned int)brightness;
    snprintf(brightness_str, sizeof(brightness_str), "%u", int_brightness);
    // snprintf(message_topic, sizeof(message_topic), "home/led/brightness");
    snprintf(message_topic, sizeof(message_topic), "%sled/brightness", MQTT_TOPIC);

    // printf("TOPIC: %s\n", message_topic);

    // int message_id = esp_mqtt_client_publish(client, "home/led/brightness", brightness_str, 0, 1, 0);
    int message_id = esp_mqtt_client_publish(client, message_topic, brightness_str, 0, 1, 0);

    if (message_id == -1)
    {
        printf("Error: Failed to send MQTT message.\n");
        return ESP_FAIL;
    }
    else
    {
        printf("MQTT message sent with message ID: %d\n", message_id);
        return ESP_OK;
    }
}
