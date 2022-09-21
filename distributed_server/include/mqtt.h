#ifndef MQTT_H
#define MQTT_H

void mqtt_start();
void mqtt_restart();
void mqtt_stop();
void mqtt_publish(char *topic, char *menssage);
int mqtt_subscribe(char *topic);
void mqtt_event_data_handler(char *data);
#endif
