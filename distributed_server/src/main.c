#include <stdio.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#include "wifi.h"
#include "mqtt.h"

#include <cJson.h>
#define PIN 2

xSemaphoreHandle wifiSemaphore;
xSemaphoreHandle mqttSemaphore;

void blink_led()
{
    gpio_pad_select_gpio(PIN);
    gpio_set_direction(PIN, GPIO_MODE_OUTPUT);
    int isOn = 0;
    int time = 100; //short
    while (true)
    {
        isOn = !isOn;
        gpio_set_level(PIN, isOn);
        vTaskDelay(time / portTICK_PERIOD_MS);    
    } 
}

void conectadoWifi(void * params)
{
  while(true)
  {
    if(xSemaphoreTake(wifiSemaphore, portMAX_DELAY))
    {
      mqtt_start();
    }
  }
}


void serverComunication(void * params)
{
  char *jsonAtributos;
  cJSON *dados = NULL;
  if(xSemaphoreTake(mqttSemaphore, portMAX_DELAY))
  {
    while(true)
    {
       float temperatura = 20.0f + (float)rand()/(float)(RAND_MAX/10.0);
       dados = cJSON_CreateObject();
       cJSON_AddItemToObject(dados, "temperatura", cJSON_CreateNumber(temperatura));
       jsonAtributos = cJSON_Print(dados);
       mqtt_send_message("v1/devices/me/telemetry", jsonAtributos);
       vTaskDelay(3000 / portTICK_PERIOD_MS);

       dados = cJSON_CreateObject();
       cJSON_AddItemToObject(dados, "umidade", cJSON_CreateNumber(100.9));
       jsonAtributos = cJSON_Print(dados);
       mqtt_send_message("v1/devices/me/attributes", jsonAtributos);
       vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
  }
}

void app_main() {
    
    // Inicializa o NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    wifiSemaphore = xSemaphoreCreateBinary();
    mqttSemaphore = xSemaphoreCreateBinary();
    wifi_start();

    xTaskCreate(&conectadoWifi,  "Conexão ao MQTT", 4096, NULL, 1, NULL);
    xTaskCreate(&serverComunication, "Comunicação com Broker", 4096, NULL, 1, NULL);

    xTaskCreate(&blink_led,  "Pisca led", 4096, NULL, 1, NULL);

}