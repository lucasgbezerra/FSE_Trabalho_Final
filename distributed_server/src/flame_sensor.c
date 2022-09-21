#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_log.h"
// #include "driver/dac.h"
#include "cJson.h"
#include "mqtt.h"

#include "nvs_handler.h"

#define FLAME_ADC_CHANNEL ADC1_CHANNEL_7
#define FLAME_DAC_CHANNEL ADC2_CHANNEL_8
#define TAG "FLAME"

#define ATTRIBUTE 0
#define TELEMETRY 1

void mqtt_published_flame(int msg, int type)
{
    cJSON *data = cJSON_CreateObject();
    if (data == NULL)
    {
        ESP_LOGE(TAG, "Erro ao criar JSON");
        return;
    }
    if (type == ATTRIBUTE)
    {
        cJSON_AddItemToObject(data, "tem_fogo", cJSON_CreateNumber(msg));
        mqtt_publish("v1/devices/me/attributes", cJSON_Print(data));
        write_value_nvs("tem_fogo", msg);
        // read_nvs_value("tem_fogo");
    }else{
        cJSON_AddItemToObject(data, "voltagem", cJSON_CreateNumber(msg));
        mqtt_publish("v1/devices/me/telemetry", cJSON_Print(data));
    }
}

void check_flame()
{
    int voltage = 0;
    int has_fire = 0;
    int count = 0;
    while (true)
    {
        count++;
        voltage = adc1_get_raw(FLAME_ADC_CHANNEL);
        has_fire = gpio_get_level(GPIO_NUM_25);
        switch (has_fire)
        {
        case 0:
            mqtt_published_flame(has_fire, ATTRIBUTE);
            break;
        case 1:
            ESP_LOGI(TAG, "Fogo detectado!");
            mqtt_published_flame(has_fire, ATTRIBUTE);
            break;
        default:
            break;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        if (count == 10)
        {
            ESP_LOGI(TAG, "Voltage: %d | Fire: %d", voltage, has_fire);
            mqtt_published_flame(voltage, TELEMETRY);
            count = 0;
        }
    }
}

void setup_flame_sensor()
{
    // Analogica
    gpio_pad_select_gpio(GPIO_NUM_33);
    gpio_set_direction(GPIO_NUM_33, GPIO_MODE_INPUT);

    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(FLAME_ADC_CHANNEL, ADC_ATTEN_0db);

    // Digital
    gpio_pad_select_gpio(GPIO_NUM_25);
    gpio_set_direction(GPIO_NUM_25, GPIO_MODE_INPUT);

    xTaskCreate(&check_flame, "Sensor de chama", 4096, NULL, 1, NULL);
}