#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

#include "mqtt.h"
#include "cJson.h"
#include "pwm.h"
#include "buzzer.h"
#include "dht.h"
#include "flame_sensor.h"

#define TAG "MQTT"
#define MQTT_USERNAME CONFIG_ESP_MQTT_USERNAME
#define MQTT_URI CONFIG_ESP_MQTT_URI

extern xSemaphoreHandle mqtt_semaphore;
esp_mqtt_client_handle_t client;

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    int msg_id;

    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        xSemaphoreGive(mqtt_semaphore);
        msg_id = mqtt_subscribe("v1/devices/me/rpc/request/+");
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        mqtt_event_data_handler(event->data);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

void set_attributes_states(char *key, int value)
{

    printf("KEY: %s\n", key);
    if (strcmp("setPwmValue", key) == 0)
    {
        set_pwm(value);
    }
    else if (strcmp("setBuzzerEnable", key) == 0)
    {
        set_buzzer_state(value);
    }
    else if (strcmp("setFlameSensorState", key) == 0)
    {
        set_flame_sensor_state(value);
    }
    else if (strcmp("setDhtState", key) == 0)
    {
        set_dht_state(value);
    }
}

void mqtt_event_data_handler(char *data)
{
    cJSON *json = cJSON_Parse(data);
    if (json == NULL)
        return;
    char *key = cJSON_GetObjectItem(json, "method")->valuestring;
    int value = cJSON_GetObjectItem(json, "params")->valueint;
    if(strstr(key, "set") != NULL){
        set_attributes_states(key, value);
    }
}
void mqtt_start()
{
    esp_mqtt_client_config_t mqtt_config = {
        .uri = MQTT_URI,
        .username = MQTT_USERNAME};
    client = esp_mqtt_client_init(&mqtt_config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void mqtt_publish(char *topic, char *message)
{
    int message_id = esp_mqtt_client_publish(client, topic, message, 0, 1, 0);
    ESP_LOGI(TAG, "Mensagem publish enviada, ID: %d", message_id);
}

int mqtt_subscribe(char *topic)
{
    int message_id = esp_mqtt_client_subscribe(client, topic, 0);
    ESP_LOGI(TAG, "Subscribe mensagem: %d no topico: %s", message_id, topic);
    return message_id;
}
void mqtt_restart()
{
    esp_mqtt_client_start(client);
    ESP_LOGI(TAG, "Reiniciado");
}
void mqtt_stop()
{
    esp_mqtt_client_stop(client);
    ESP_LOGI(TAG, "Parado");
}