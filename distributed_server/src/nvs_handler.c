#include <stdio.h>
#include "nvs.h"
#include "nvs_flash.h"

#define PARTICAO "Atributos"


int32_t le_valor_nvs(char *atributo){
  // Inicializa o NVS
  esp_err_t ret = nvs_flash_init_partition(PARTICAO);
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  // LE PARTICAO
  nvs_handle particao_handle;
  esp_err_t res_nvs =  nvs_open_from_partition(PARTICAO, "armazenameto", NVS_READONLY, &particao_handle);
  int32_t valor = 0;
  if(res_nvs == ESP_ERR_NVS_NOT_FOUND){
    ESP_LOGE("NVS", "Namespace: armazenamento, não encontrado");
  }
  else{
    esp_err_t res_leitura = nvs_get_i32(particao_handle, atributo, &valor);

    switch (res_leitura)
    {
    case ESP_OK:
      printf("Valor armazenado: %d\n", valor);
      break;
    case ESP_ERR_NOT_FOUND:
      ESP_LOGE("NVS", "Valor não encontrado\n");  
      return -1;
    default:
      ESP_LOGE("NVS", "Erro ao acessar o NVS (%s)", esp_err_to_name(res_leitura));
      return -1;
    }
    nvs_close(particao_handle);
  }
  return valor;

}

void grava_valor_nvs(char *atributo, int32_t valor){
  // Inicializa o NVS
  esp_err_t ret = nvs_flash_init_partition(PARTICAO);
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  // GRAVAR NA PARTICAO
  nvs_handle particao_handle;
  esp_err_t res_nvs =  nvs_open_from_partition(PARTICAO, "armazenameto", NVS_READWRITE, &particao_handle);
  int32_t valor = 0;
  if(res_nvs == ESP_ERR_NVS_NOT_FOUND){
    ESP_LOGE("NVS", "Namespace: armazenamento, não encontrado");
  }
  esp_err_t res_gravacao = nvs_set_i32(particao_handle, atributo, valor);
  if(res_gravacao != ESP_OK){
    ESP_LOGE("NVS", "Não foi possível gravar o atributo no NVS (%s)", esp_err_to_name(res_gravacao));
  }
  nvs_commit(particao_handle);
  nvs_close(particao_handle);
}