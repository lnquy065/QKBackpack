#include <Arduino.h>
#include <Sensor.h>

int NoiseRead(int aPin) {
  int value = analogRead(aPin);
  return value;
}
