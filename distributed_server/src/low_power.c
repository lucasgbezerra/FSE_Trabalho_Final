#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "esp32/rom/uart.h"
#include "driver/gpio.h"
#include "cJson.h"

#include "driver/rtc_io.h"
#include "flame_sensor.h"
#include "buzzer.h"
#include "wifi.h"
#include "mqtt.h"
#include "pwm.h"
#include "dht.h"



#define BUTTON 0

extern xSemaphoreHandle wifi_semaphore;
extern xSemaphoreHandle mqtt_semaphore;
extern xSemaphoreHandle wifi_reconnect_semaphore;


cJSON * create_attributes_json()
{
    cJSON *data = cJSON_CreateObject();
    if (data == NULL)
    {
        ESP_LOGE("LOW POWER", "Erro ao criar JSON");
        return NULL;
    }
    cJSON_AddItemToObject(data, "tem_fogo", cJSON_CreateNumber(low_flame_sensor()));
    cJSON_AddItemToObject(data, "temperatura_media", cJSON_CreateNumber(low_power_read('T')));
    cJSON_AddItemToObject(data, "umidade_media", cJSON_CreateNumber(low_power_read('U')));
    cJSON_AddItemToObject(data, "status_led", cJSON_CreateNumber(get_pwm_value()));

    return data;

}
void battery_mode()
{
    gpio_pad_select_gpio(BUTTON);
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
    gpio_wakeup_enable(BUTTON, GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();
    int state = 0;

    cJSON *data = cJSON_CreateObject();
    while (true)
    {

        mqtt_stop();
        wifi_stop();
        uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);
        esp_light_sleep_start();

        if (rtc_gpio_get_level(BUTTON) == 0)
        {
            state = !state;
            wifi_restart();
            if (xSemaphoreTake(wifi_reconnect_semaphore, portMAX_DELAY))
            {
                mqtt_restart();
                data = create_attributes_json();
                if(data != NULL)
                    mqtt_publish("v1/devices/me/attributes", cJSON_Print(data));
            }

            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

