#ifndef BUZZER_H
#define BUZZER_H

#define BUZZER_PIN 27

void buzzer_setup();
void play_buzzer(int value);
void mqtt_published_buzzer();
void set_buzzer_state(int value);
#endif
