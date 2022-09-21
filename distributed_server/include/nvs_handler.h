#ifndef NVS_HANDLER_H
#define NVS_HANDLER_H

#include "nvs.h"
#include "nvs_flash.h"

int32_t read_nvs_value(char *atributo);
void write_value_nvs(char *atributo, int32_t valor);
void init_nvs();

#endif