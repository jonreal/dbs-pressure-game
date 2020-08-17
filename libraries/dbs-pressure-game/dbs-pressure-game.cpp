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
  button = 13;

  pinMode(a0, INPUT_PULLUP);
  pinMode(lessled, OUTPUT);
  pinMode(targetled, OUTPUT);
  pinMode(moreled, OUTPUT);
  pinMode(targetout,OUTPUT);
  pinMode(button, INPUT_PULLUP);

  analogWrite(lessled, 0);
  analogWrite(targetled, 0);
  analogWrite(moreled, 0);
  analogWrite(targetout, 0);

  force = 0;
  target = 500;
  biasforce = 0;
  mvcforce = 1024;

  state = STANDBY;
  cnt = 0;
  buttoncnt = 0;
  threshold = 25;
  calibratecnt = 0;
}

void DbsPressureGame::CalibrateBias(void) {
  int N = 10;
  for (int i=0; i<N; i++) {
    force = analogRead(a0);
    biasforce += force;
    delay(100);
  }
  biasforce = biasforce / N;
  UpdateLEDCoefficients();
}

int DbsPressureGame::Calibrate(void) {
  calibratecnt++;
  if (force > mvcforce)
    mvcforce = force;

  targetpwm = (force + 3) /4;
  morepwm = (force + 3) /4;
  lesspwm = (force + 3) /4;

  if (targetpwm < 0)
    targetpwm = 0;
  if (morepwm < 0)
    morepwm = 0;
  if (lesspwm < 0)
    lesspwm = 0;

  if (targetpwm > 255)
    targetpwm = 255;
  if (morepwm > 255)
    morepwm = 255;
  if (lesspwm > 255)
    lesspwm = 255;

  if (calibratecnt >= 250) {
    UpdateLEDCoefficients();
    target = (3*mvcforce + 3) / 4;
    return 1;
  }
  else
    return 0;
}

void DbsPressureGame::UpdateLEDCoefficients(void) {

  maxerror = target - biasforce;
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

void DbsPressureGame::SetGameLEDPwm(void) {
  if (error < 0) {
    targetpwm = (m1target*error + kTarget + 99)/100;
    lesspwm = (mless*error + 99)/100 - biasforce;
    morepwm = 0;
  } else {
    targetpwm = (m2target*error + kTarget + 99)/100;
    morepwm = (mmore*error + 99)/100 - biasforce;
    lesspwm = 0;
  }
  if (targetpwm < 0)
    targetpwm = 0;
  if (morepwm < 0)
    morepwm = 0;
  if (lesspwm < 0)
    lesspwm = 0;

  if (targetpwm > 255)
    targetpwm = 255;
  if (morepwm > 255)
    morepwm = 255;
  if (lesspwm > 255)
    lesspwm = 255;
}

void DbsPressureGame::SetLEDOff(void) {
  morepwm = 0;
  targetpwm = 0;
  lesspwm = 0;
}

void DbsPressureGame::SetStandyLEDPwm(void) {
      targetpwm = (force + 3) /4;
      if ((cnt%10) == 0) {
        morepwm = 10;
        lesspwm = 0;
      }
      if ((cnt%20) == 0) {
        morepwm = 0;
        lesspwm = 10;
      }
}

void DbsPressureGame::UpdateLEDPwm(void) {
  analogWrite(lessled, lesspwm);
  analogWrite(targetled, targetpwm);
  analogWrite(moreled, morepwm);
  analogWrite(targetout, target/4);
}

void DbsPressureGame::BlinkLED(void) {
  for (int i=0; i<3; i++) {
    analogWrite(lessled, 0);
    analogWrite(targetled, 0);
    analogWrite(moreled, 0);
    delay(250);
    analogWrite(lessled, 100);
    analogWrite(targetled,100);
    analogWrite(moreled, 100);
    delay(250);
  }
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
  Serial.print(biasforce);
  Serial.print('\t');
  Serial.print(buttonstate);
  Serial.print('\t');
  Serial.print(buttoncnt);
  Serial.print('\t');
  Serial.print(state);
  Serial.print('\t');
  Serial.print(mvcforce);
  Serial.print('\t');
  Serial.print(target);
  Serial.print('\t');
  Serial.println();
}

int DbsPressureGame::ButtonPush(void) {
  buttonstate = digitalRead(button);

  if (!buttonstate)
    buttoncnt++;

  if (buttoncnt > threshold) {
    buttoncnt = 0;
    return 1;
  }
  else {
    return 0;
  }
}

void DbsPressureGame::HandleFlow(void) {
  if ((tnow = millis()) < (tprev + 10))
    return;
  tprev = tnow;
  cnt++;

  SampleForce();

  switch (state) {
    case STANDBY:
      SetStandyLEDPwm();
      if (ButtonPush()) {
        SetLEDOff();
        BlinkLED();
        mvcforce = 0;
        calibratecnt = 0;
        state = CALIBRATE;
      }
      break;
    case CALIBRATE :
      if (Calibrate()) {
        BlinkLED();
        state = GAME;
      }
      break;
    case GAME:
      SetGameLEDPwm();
      if (ButtonPush()) {
        BlinkLED();
        mvcforce = 0;
        calibratecnt = 0;
        state = CALIBRATE;
      }
      break;
  }
  UpdateLEDPwm();
  PrintVars();
}
