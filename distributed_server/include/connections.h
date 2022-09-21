#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#define BATTERY_MODE 0
#define ENERGY_MODE 1

#if defined(CONFIG_MODE_ENERGY)
#define OPERATION_MODE ENERGY_MODE
#endif

#if defined(CONFIG_MODE_BATTERY)
#define OPERATION_MODE BATTERY_MODE
#endif

void wifi_connect();
void mqtt_connect();
void connections();
void battery_mode();

#endif