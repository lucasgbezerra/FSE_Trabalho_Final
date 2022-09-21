#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "cJson.h"
#include "mqtt.h"
#include "buzzer.h"

void play_buzzer(int value)
{
    gpio_set_level(BUZZER_PIN, value);
}

void buzzer_setup()
{
    gpio_pad_select_gpio(BUZZER_PIN);
    gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);
}
