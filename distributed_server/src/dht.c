#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "dht11.h"
#include "dht.h"

#define TAG "DHT11"
#define DHT11_PIN GPIO_NUM_4

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
            avg_temperature = (last_temperature + dht11_data.temperature) / 2;
            last_temperature = dht11_data.temperature;
            avg_humidity = (last_humidity + dht11_data.humidity) / 2;
            last_humidity = dht11_data.humidity;
            ESP_LOGI(TAG, "LAST => [TEMPERATURA]: %d | [UMIDADE]: %d", last_temperature, last_humidity);
            count++;
        }
        if (count == 5)
        {
            ESP_LOGI(TAG, "AVERAGE => [TEMPERATURA]: %.2f | [UMIDADE]: %.2f", avg_humidity, avg_temperature);
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