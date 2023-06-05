#define PIN_LED 13
#define PIN_PHOTO_SENSOR A0
#define PIN_SOUND_SENSOR A2

#include "Adafruit_SHTC3.h"

Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();

boolean statuslamp;

void setup() {
  Serial.begin(115200);

  while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("SHTC3 test");
  if (!shtc3.begin()) {
    Serial.println("Couldn't find SHTC3");
    while (1) delay(1);
  }
  Serial.println("Found SHTC3 sensor");
}


void loop() {
  sensors_event_t humidity, temperature;
  shtc3.getEvent(&humidity, &temperature);// populate temp and humidity objects with fresh data
  
  uint8_t sound = analogRead(PIN_SOUND_SENSOR);
  uint8_t light = analogRead(PIN_PHOTO_SENSOR);
  
  Serial.print(humidity.relative_humidity); Serial.print(' ');
  Serial.print(temperature.temperature); Serial.print(' ');
  Serial.print(sound); Serial.print(' ');
  Serial.println(light);
}