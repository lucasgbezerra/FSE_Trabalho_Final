#include <stdio.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "wifi.h"
#include "mqtt.h"
#include "pwm.h"
#include "dht.h"
#include "flame_sensor.h"

xSemaphoreHandle wifi_semaphore;
xSemaphoreHandle mqtt_semaphore;

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

// void mqtt_connect()
// {
//   while (true)
//   {
   
//   }
// }

void connections()
{
  mqtt_semaphore = xSemaphoreCreateBinary();
  wifi_semaphore = xSemaphoreCreateBinary();
  wifi_start();

  xTaskCreate(&wifi_connect, "Wifi", 4096, NULL, 1, NULL);
  // xTaskCreate(&mqtt_connect, "MQTT", 4096, NULL, 1, NULL);
}
