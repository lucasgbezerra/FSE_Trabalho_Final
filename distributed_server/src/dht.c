#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "cJson.h"
#include "mqtt.h"

#include "dht11.h"
#include "dht.h"
#include "nvs_handler.h"

#define TAG "DHT11"
#define DHT11_PIN GPIO_NUM_4

#define MEDIA 0
#define LAST 1

int dht11_on = 1;

void mqtt_published_dht11(void *temperature, void *humidity, int type)
{
    cJSON *data = cJSON_CreateObject();
    if (data == NULL)
    {
        ESP_LOGE(TAG, "Erro ao criar JSON");
        return;
    }

    if (type == MEDIA)
    {
        float temp = *(float *)temperature;
        float umd = *(float *)humidity;

        cJSON_AddItemToObject(data, "temperatura_media", cJSON_CreateNumber(temp));
        cJSON_AddItemToObject(data, "umidade_media", cJSON_CreateNumber(umd));
        mqtt_publish("v1/devices/me/telemetry", cJSON_Print(data));
    }
    else
    {
        int temp = *(int *)temperature;
        int umd = *(int *)humidity;
        cJSON_AddItemToObject(data, "temperatura", cJSON_CreateNumber(temp));
        cJSON_AddItemToObject(data, "umidade", cJSON_CreateNumber(umd));
        mqtt_publish("v1/devices/me/telemetry", cJSON_Print(data));
    }
}

void read_dht11()
{
    float avg_temperature = 0.0;
    int last_temperature = 0;
    float avg_humidity = 0.0;
    int last_humidity = 0;
    struct dht11_reading dht11_data;
    int count = 0;

    while (true)
    {
        if (dht11_on)
        {
            dht11_data = DHT11_read();
            if (dht11_data.status == DHT11_OK)
            {
                avg_temperature = (last_temperature + dht11_data.temperature) / 2.0;
                last_temperature = dht11_data.temperature;
                avg_humidity = (last_humidity + dht11_data.humidity) / 2.0;
                last_humidity = dht11_data.humidity;
                ESP_LOGI(TAG, "LAST => [temperatura]: %d | [umidade]: %d", last_temperature, last_humidity);
                mqtt_published_dht11(&last_temperature, &last_humidity, LAST);
                count++;
            }
            if (count == 10)
            {
                ESP_LOGI(TAG, "AVERAGE => [temperatura]: %.2f | [umidade]: %.2f", avg_temperature, avg_humidity);
                mqtt_published_dht11(&avg_temperature, &avg_humidity, MEDIA);
                count = 0;
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void dht_setup()
{
    DHT11_init(DHT11_PIN);
    xTaskCreate(&read_dht11, "Realiza Leitura das infos DHT11", 4096, NULL, 1, NULL);
}

void set_dht_state(int value)
{
    dht11_on = value;
}