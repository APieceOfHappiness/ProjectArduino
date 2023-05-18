#include <stdlib.h>
#include <unistd.h>

#define PIN_PHOTO_SENSOR A0

#define PIN_LED 13

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
}

void loop()
{
  int val = analogRead(PIN_PHOTO_SENSOR);
  Serial.println(val);
  if (val > 200) {
    digitalWrite(PIN_LED, HIGH);
  } else {
    digitalWrite(PIN_LED, LOW);    
  }
}