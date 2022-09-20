#ifndef NVS_HANDLER_H
#define NVS_HANDLER_H

#include "nvs.h"
#include "nvs_flash.h"

int32_t le_valor_nvs(char *atributo);
void grava_valor_nvs(char *atributo, int32_t valor);

#endif