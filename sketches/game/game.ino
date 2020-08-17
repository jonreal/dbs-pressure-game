#include "dbs-pressure-game.h"

DbsPressureGame s;

void setup() {
  Serial.begin(9600);
  s.CalibrateBias();
}

void loop() {
  s.HandleFlow();
}
