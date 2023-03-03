#include <SPI.h>
#define PIN_PHOTO_SENSOR A0
#define PIN_PIEZO 12
#define PIN_DIOD 8
#define HIGH_TONE 1000
#define LOW_TONE 500
#define TACT_COUNT 5
#define DELAY_PIEZO 500
bool light = 1;

void setup() {
  pinMode(PIN_DIOD, 1);
  Serial.begin(9600);
  Serial.print("Hello\n");
}

void light_low(){
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
