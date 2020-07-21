#include "dbs-pressure-game.h"
#include "Arduino.h"
#include <Wire.h>

DbsPressureGame::DbsPressureGame() {
  tnow = 0;
  tprev = 0;

  a0 = A0;
  lessled = 3;
  targetled = 5;
  moreled = 6;

  pinMode(a0, INPUT_PULLUP);
  pinMode(lessled, OUTPUT);
  pinMode(targetled, OUTPUT);
  pinMode(moreled, OUTPUT);

  force = 0;
  target = 500;

  UpdateLEDCoefficients();

}

void DbsPressureGame::UpdateLEDCoefficients(void) {

  maxerror = target;
  minerror = target - 1024;

  m1target = -(25500 / minerror);
  m2target = -(25500 / maxerror);
  mmore = (25500 / maxerror);
  mless = (25500 / minerror); 
}

void DbsPressureGame::SampleForce(void) {
  force = analogRead(a0);
  error = target - force;
}

void DbsPressureGame::UpdateLEDPwm(void) {
  if (error < 0)
    targetpwm = (m1target*error + kTarget + 99)/100;
  else
    targetpwm = (m2target*error + kTarget + 99)/100;

  morepwm = (mmore*error + 99)/100;
  if (morepwm < 0)
    morepwm = 0;

  lesspwm = (mless*error + 99)/100;
  if (lesspwm < 0)
    lesspwm = 0;

  analogWrite(lessled, lesspwm);
  analogWrite(targetled, targetpwm);
  analogWrite(moreled, morepwm);
}

void DbsPressureGame::PrintVars(void) {
  Serial.print(tnow);
  Serial.print('\t');
  Serial.print(force);
  Serial.print('\t');
  Serial.print(error);
  Serial.print('\t');
  Serial.print(morepwm);
  Serial.print('\t');
  Serial.print(targetpwm);
  Serial.print('\t');
  Serial.print(lesspwm);
  Serial.print('\t');
  Serial.println();
}

void DbsPressureGame::HandleFlow(void) {
  if ((tnow = millis()) < (tprev + 100))
    return;
  tprev = tnow;

  SampleForce();
  UpdateLEDPwm();
  PrintVars();
}
