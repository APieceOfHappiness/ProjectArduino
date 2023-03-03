#include <SPI.h>
#include "macros.h"
bool light = 1;

void setup() {
  pinMode(PIN_DIOD, 1);
  Serial.begin(9600);
  Serial.print("Hello\n");
}

void light_low() {
  for (int i = 0; i < TACT_COUNT; ++i) {
    tone(PIN_PIEZO, HIGH_TONE, DELAY_PIEZO);
    delay(DELAY_PIEZO);
    tone(PIN_PIEZO, LOW_TONE, DELAY_PIEZO);
    delay(DELAY_PIEZO);
  }
  light = 1;
}

void loop() {
  int photo_sensor_value = analogRead(PIN_PHOTO_SENSOR);
  if (photo_sensor_value >= 1000) {
    digitalWrite(PIN_DIOD, 1);
    if (!light) {
      light_low();
    }
  } else {
    digitalWrite(PIN_DIOD, 0);
    noTone(PIN_PIEZO);
    light = 0;
  }
}
