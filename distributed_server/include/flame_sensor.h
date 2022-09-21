#ifndef FLAME_SENSOR_H
#define FLAME_SENSOR_H

#define FLAME_ANALOG_PIN 33
#define FLAME_DIGITAL_PIN 25

#define ATTRIBUTE 0
#define TELEMETRY 1

void setup_flame_sensor();
void setup_analog_flame_sensor(int PIN);
void setup_digital_flame_sensor(int PIN);
void mqtt_published_flame(int msg, int type);
void fire_warning(int has_fire);

#endif
