#include <Wire.h>

int pins[6] = {6, 9, 10, 11, 12, 13};

void setup() 
{
  for (int i = 0; i < 6; i++){
    pinMode(pins[i], OUTPUT);
  }
}

void loop() 
{
  for (int i = 0; i < 6; i++){
    allOff();
    delay(500);
    digitalWrite(pins[i], HIGH);
    Serial.println(pins[i]);
    delay(500);
  }
}

void allOff(){
  for (int i = 0; i < 6; i++){
    digitalWrite(pins[i], LOW);
  }
}