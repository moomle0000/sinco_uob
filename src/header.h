#include <Arduino.h>

int enA = 21; // for speed control motor low
int in1 = 2;  // for direction control motor low
int in2 = 4;  // for direction control motor low
// Motor B Left/Right connections
int enB = 15; // for speed control motor high
int in3 = 18; // for direction control motor High    USE THIS
int in4 = 19; // for direction control motor     USE THIS

void ligth(bool PowerState ,int pin)  {
  if(PowerState){
    digitalWrite(pin , PowerState);
  }else{
    digitalWrite(pin , PowerState);
  }
}



