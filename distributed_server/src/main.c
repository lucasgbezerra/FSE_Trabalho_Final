#include <stdio.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"

#include "wifi.h"
#include "mqtt.h"
#include "dht.h"
#include "flame_sensor.h"

xSemaphoreHandle wifiSemaphore;
xSemaphoreHandle mqttSemaphore;

void conectadoWifi(void *params)
{
  while (true)
  {
    if (xSemaphoreTake(wifiSemaphore, portMAX_DELAY))
    {
      mqtt_start();
    }
  }
}

void app_main()
{

  // Inicializa o NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  wifiSemaphore = xSemaphoreCreateBinary();
  mqttSemaphore = xSemaphoreCreateBinary();
  wifi_start();

  xTaskCreate(&conectadoWifi, "Conexão ao MQTT", 4096, NULL, 1, NULL);
  if (xSemaphoreTake(mqttSemaphore, portMAX_DELAY))
  {
    dht_setup();
    setup_flame_sensor();
  }
}