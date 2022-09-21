#include <stdio.h>
#include "esp_event.h"
#include "freertos/semphr.h"
#include "freertos/FreeRTOS.h"

#include "nvs_flash.h"
#include "connections.h"
#include "pwm.h"
#include "dht.h"
#include "flame_sensor.h"
#include "nvs_handler.h"

extern xSemaphoreHandle mqtt_semaphore;

void app_main()
{
  init_nvs();

  connections();
   if (xSemaphoreTake(mqtt_semaphore, portMAX_DELAY))
    {
      config_pwm();
      dht_setup();
      setup_flame_sensor();
    }
}