#ifndef _DBSGONOGO_H_
#define _DBSGONOGO_H_

#include "Arduino.h"


const int kTarget = 25500;

typedef enum {
  STANDBY = 0,
  CALIBRATE,
  GAME,
} state_t;

class DbsPressureGame
{
  public:
    DbsPressureGame(void);
    void HandleFlow(void);

  private:
    void UpdateLEDCoefficients(void);
    void UpdateLEDPwm(void);
    void SampleForce(void);
    void PrintVars(void);

    // Pins
    unsigned char a0;
    unsigned char moreled;
    unsigned char lessled;
    unsigned char targetled;

    // Force
    int force;
    int target;
    int error;

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
