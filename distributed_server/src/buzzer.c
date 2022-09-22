#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "cJson.h"
#include "mqtt.h"
#include "buzzer.h"
#include "nvs_handler.h"

#define TAG "BUZZER"

int buzzer_on = 0;

void play_buzzer(int value)
{
    buzzer_on = value;
    gpio_set_level(BUZZER_PIN, value);
}

void buzzer_setup()
{
    gpio_pad_select_gpio(BUZZER_PIN);
    gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);
}

void mqtt_published_buzzer()
{   
    cJSON *data = cJSON_CreateObject();
    if (data == NULL)
    {
        ESP_LOGE(TAG, "Erro ao criar JSON");
        return;
    }
    cJSON_AddItemToObject(data, "buzzer_status", cJSON_CreateNumber(buzzer_on));
    mqtt_publish("v1/devices/me/attributes", cJSON_Print(data));
    write_value_nvs("buzzer_ligado", buzzer_on);
}

void set_buzzer_state(int value){
    buzzer_on = value;
    write_value_nvs("buzzer_ligado", buzzer_on);
}