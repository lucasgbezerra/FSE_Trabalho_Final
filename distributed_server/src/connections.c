#include <stdio.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "connections.h"
#include "wifi.h"
#include "mqtt.h"
#include "pwm.h"
#include "dht.h"
#include "flame_sensor.h"
#include "buzzer.h"
#include "nvs_handler.h"

extern xSemaphoreHandle wifi_semaphore;
extern xSemaphoreHandle mqtt_semaphore;

void wifi_connect()
{
  while (true)
  {
    if (xSemaphoreTake(wifi_semaphore, portMAX_DELAY))
    {
      mqtt_start();
    }
  }
}

void get_storage_state()
{
  int pwm = read_nvs_value("pwm_value");
  set_pwm(pwm);
}
void connections()
{

  wifi_start();
  xTaskCreate(&wifi_connect, "Wifi", 4096, NULL, 1, NULL);
  if (xSemaphoreTake(mqtt_semaphore, portMAX_DELAY))
  {

    config_pwm();
    get_storage_state();
    if (OPERATION_MODE == BATTERY_MODE)
    {
      ESP_LOGI("MODO", "Funcionamento via bateria");
      xTaskCreate(battery_mode, "light_sleep_task", 4096, NULL, 1, NULL);
    }
    else
    {
      ESP_LOGI("MODO", "Funcionamento via energia");

      buzzer_setup();
      setup_flame_sensor();
      dht_setup();
    }
  }
}
