#ifndef _DBSGONOGO_H_
#define _DBSGONOGO_H_

#include "Arduino.h"


const int kTarget = 25500;


typedef enum {
  STANDBY = 0,
  CALIBRATE = 1,
  GAME = 2,
} state_t;

class DbsPressureGame
{
  public:
    DbsPressureGame(void);
    void HandleFlow(void);
    void CalibrateBias(void);

  private:
    void UpdateLEDCoefficients(void);
    void SetGameLEDPwm(void);
    void SetStandyLEDPwm(void);
    void SetLEDOff(void);
    void UpdateLEDPwm(void);
    void SampleForce(void);
    void PrintVars(void);
    int ButtonPush(void);
    int Calibrate(void);
    void BlinkLED(void);


    state_t state;
    unsigned long cnt;
    unsigned long calibratecnt;
    unsigned long buttoncnt;
    unsigned long negbuttoncnt;
    unsigned long threshold;
    unsigned char buttonstate;

    // Pins
    unsigned char a0;
    unsigned char moreled;
    unsigned char lessled;
    unsigned char targetled;
    unsigned char targetout;
    unsigned char button;

    // Force
    int force;
    int target;
    int error;
    int biasforce;
    int mvcforce;

    // LED params
    int maxerror;
    int minerror;
    int mmore;
    int mless;
    int m1target;
    int m2target;

    int targetpwm;
    int morepwm;
    int lesspwm;

    // Program Flow
    unsigned long tnow;
    unsigned long tprev;
};

#endif
