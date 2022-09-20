#include <stdio.h>
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

#include "nvs_handler.h"

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
  
  int32_t resultado_leitura = le_valor_nvs("tem-fogo");
  
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