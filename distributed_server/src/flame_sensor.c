#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_log.h"
// #include "driver/dac.h"

#define FLAME_ADC_CHANNEL ADC1_CHANNEL_7
#define FLAME_DAC_CHANNEL ADC2_CHANNEL_8
#define TAG "FLAME"

static const adc_bits_width_t width = ADC_WIDTH_12Bit;
void check_flame()
{
    int voltage = 0;
    int has_fire = 0;
    while (true)
    {
        voltage = adc1_get_raw(FLAME_ADC_CHANNEL);

        has_fire = gpio_get_level(GPIO_NUM_25);
        switch (has_fire)
        {
        case 0:
            break;
        case 1:
            ESP_LOGI(TAG, "Fogo detectado!");
            ESP_LOGI(TAG, "Voltage: %d | Fire: %d", voltage, has_fire);
            break;
        default:
            break;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup_flame_sensor()
{
    // Analogica
    gpio_pad_select_gpio(GPIO_NUM_33);
    gpio_set_direction(GPIO_NUM_33, GPIO_MODE_INPUT);

    adc1_config_width(width);
    adc1_config_channel_atten(FLAME_ADC_CHANNEL, ADC_ATTEN_0db);

    // Digital
    gpio_pad_select_gpio(GPIO_NUM_25);
    gpio_set_direction(GPIO_NUM_25, GPIO_MODE_INPUT);

    xTaskCreate(&check_flame, "Sensor de chama", 4096, NULL, 1, NULL);
}