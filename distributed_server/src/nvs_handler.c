#include <stdio.h>
#include "nvs.h"
#include "nvs_flash.h"

#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"

#include "nvs_handler.h"

#define PARTICAO "Atributos"

#define TAG "NVS"

// extern xSemaphoreHandle nvs_semaphore;

int32_t read_nvs_value(char *atributo)
{
  /* // Inicializa o NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret); */

  init_nvs();

  // LE PARTICAO
  nvs_handle particao_handle;
  esp_err_t res_nvs = nvs_open("armazenameto", NVS_READONLY, &particao_handle);
  int32_t valor = 0;
  if (res_nvs == ESP_ERR_NVS_NOT_FOUND)
  {
    ESP_LOGE(TAG, "Namespace: armazenamento, não encontrado");
  }
  else
  {
    esp_err_t res_leitura = nvs_get_i32(particao_handle, atributo, &valor);

    switch (res_leitura)
    {
    case ESP_OK:
      ESP_LOGI(TAG, "Valor obitido");
      break;
    case ESP_ERR_NOT_FOUND:
      ESP_LOGE(TAG, "Valor não encontrado\n");
      return -1;
    default:
      ESP_LOGE("NVS", "Erro ao acessar o NVS (%s)", esp_err_to_name(res_leitura));
      return -1;
    }
    nvs_close(particao_handle);
  }
  return valor;
}

void write_value_nvs(char *atributo, int32_t valor)
{
  /* // Inicializa o NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret); */

  init_nvs();

  // GRAVAR NA PARTICAO
  nvs_handle particao_handle;
  esp_err_t res_nvs = nvs_open("armazenameto", NVS_READWRITE, &particao_handle);
  if (res_nvs == ESP_ERR_NVS_NOT_FOUND)
  {
    ESP_LOGE(TAG, "Namespace: armazenamento, não encontrado");
  }
  esp_err_t res_gravacao = nvs_set_i32(particao_handle, atributo, valor);
  if (res_gravacao != ESP_OK)
  {
    ESP_LOGE(TAG, "Não foi possível gravar o atributo %s no NVS (%s)", atributo, esp_err_to_name(res_gravacao));
  }
  nvs_commit(particao_handle);
  nvs_close(particao_handle);
}

void init_nvs()
{
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

}