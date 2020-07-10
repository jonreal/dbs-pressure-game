#include "dbs-pressure-game.h"
#include "Arduino.h"
#include <Wire.h>

DbsPressureGame::DbsPressureGame() {
  tnow = 0;
  tprev = 0;

  a0 = A0;
  rled = 3;
  wled = 5;
  gled = 6;

  pinMode(a0, INPUT_PULLUP);
  pinMode(rled, OUTPUT);
  pinMode(wled, OUTPUT);
  pinMode(gled, OUTPUT);

  force = 0;
  target = 500;

}



int DbsPressureGame::errorToTargetPWM(int error) {
  int slope = -1;
  int rtn = slope * abs(error) + 255;
  if (rtn < 0)
    rtn = 0;
  return rtn;
}

int DbsPressureGame::errorToMorePWM(int error) {
  if (error < 0)
    return 0;

  int slope = 1;
  int rtn = slope * error;

  return rtn;
}

int DbsPressureGame::errorToLessPWM(int error) {
  if (error > 0)
    return 0;

  int slope = -1;
  int rtn = slope * error;

  return rtn;
}

void DbsPressureGame::HandleFlow(void) {

  if ((tnow = millis()) < (tprev + 100))
    return;
  tprev = tnow;

  force = analogRead(a0);

  error = target - force;

  int val1 = errorToTargetPWM(error);
  int val2 = errorToMorePWM(error);
  int val3 = errorToLessPWM(error);
  analogWrite(rled, val3);
  analogWrite(wled, val1);
  analogWrite(gled, val2);

  Serial.print(force);
  Serial.print('\t');
  Serial.print(error);
  Serial.print('\t');
  Serial.print(val1);
  Serial.print('\t');
  Serial.print(val2);
  Serial.print('\t');
  Serial.print(val3);
  Serial.print('\t');
  Serial.println(tnow);

}
