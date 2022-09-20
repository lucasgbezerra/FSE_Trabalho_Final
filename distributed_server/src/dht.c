#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "cJson.h"
#include "mqtt.h"

#include "dht11.h"
#include "dht.h"

#define TAG "DHT11"
#define DHT11_PIN GPIO_NUM_4

#define ATTRIBUTE 0
#define TELEMETRY 1

void mqtt_published_dht11(void *temperature, void *humidity, int type)
{
    cJSON *data = cJSON_CreateObject();
    if (data == NULL)
    {
        ESP_LOGE(TAG, "Erro ao criar JSON");
        return;
    }

    if (type == ATTRIBUTE)
    {
        float temp = *(float *)temperature;
        float umd = *(float *)humidity;

        cJSON_AddItemToObject(data, "Temperatura Média", cJSON_CreateNumber(temp));
        cJSON_AddItemToObject(data, "Umidade Média", cJSON_CreateNumber(umd));
        mqtt_send_message("v1/devices/me/attributes", cJSON_Print(data));
    }
    else
    {
        int temp = *(int *)temperature;
        int umd = *(int *)humidity;
        cJSON_AddItemToObject(data, "Temperatura", cJSON_CreateNumber(temp));
        cJSON_AddItemToObject(data, "Umidade", cJSON_CreateNumber(umd));
        mqtt_send_message("v1/devices/me/telemetry", cJSON_Print(data));
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
        dht11_data = DHT11_read();
        if (dht11_data.status == DHT11_OK)
        {
            avg_temperature = (last_temperature + dht11_data.temperature) / 2.0;
            last_temperature = dht11_data.temperature;
            avg_humidity = (last_humidity + dht11_data.humidity) / 2.0;
            last_humidity = dht11_data.humidity;
            ESP_LOGI(TAG, "LAST => [Temperatura]: %d | [Umidade]: %d", last_temperature, last_humidity);
            mqtt_published_dht11(&last_temperature, &last_humidity, TELEMETRY);
            count++;
        }
        if (count == 5)
        {
            ESP_LOGI(TAG, "AVERAGE => [Temperatura]: %.2f | [Umidade]: %.2f", avg_temperature, avg_humidity);
            mqtt_published_dht11(&avg_temperature, &avg_humidity, ATTRIBUTE);
            count = 0;
        }

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void dht_setup()
{
    DHT11_init(DHT11_PIN);
    xTaskCreate(&read_dht11, "Realiza Leitura das infos DHT11", 4096, NULL, 1, NULL);
}