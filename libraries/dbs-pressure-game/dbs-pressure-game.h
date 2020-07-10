#ifndef _DBSGONOGO_H_
#define _DBSGONOGO_H_

#include "Arduino.h"




class DbsPressureGame
{
  public:
    DbsPressureGame(void);
    void HandleFlow(void);

  private:
    int errorToTargetPWM(int error);
    int errorToMorePWM(int error);
    int errorToLessPWM(int error);

    // Pins
    unsigned char a0;
    unsigned char rled;
    unsigned char wled;
    unsigned char gled;

    // Force
    int force;
    int target;
    int error;

    // Program Flow
    unsigned long tnow;
    unsigned long tprev;
};

#endif
