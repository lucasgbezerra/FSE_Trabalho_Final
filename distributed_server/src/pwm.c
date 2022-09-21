#include "freertos/FreeRTOS.h"
#include "driver/ledc.h"

#include "nvs_handler.h"
#include "pwm.h"

#define LED 2
void stored_pwm_config(){
    int value = read_nvs_value("pwm_value");
    if (value != -1)
    {
        set_pwm(value);
    }
}
void config_pwm()
{
    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK

    };
    ledc_timer_config(&timer_config);
    ledc_channel_config_t channel_config = {
        .gpio_num = LED,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0};
    ledc_channel_config(&channel_config);
    stored_pwm_config();
}

void set_pwm(int value)
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, value * 2.5);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    write_value_nvs("pwm_value", value);
}