#include <stdio.h>
#include "esp_event.h"
#include "freertos/semphr.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

#include "nvs_flash.h"
#include "pwm.h"
#include "dht.h"
#include "flame_sensor.h"
#include "nvs_handler.h"
#include "buzzer.h"
#include "connections.h"
#include "low_power.h"

xSemaphoreHandle wifi_semaphore;
xSemaphoreHandle wifi_reconnect_semaphore;
xSemaphoreHandle mqtt_semaphore;

void app_main()
{
  mqtt_semaphore = xSemaphoreCreateBinary();
  wifi_semaphore = xSemaphoreCreateBinary();
  wifi_reconnect_semaphore = xSemaphoreCreateBinary();

  init_nvs();
  connections();
}