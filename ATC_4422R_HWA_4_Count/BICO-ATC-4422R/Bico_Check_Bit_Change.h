#ifndef BICO_CHECK_BIT_CHANGE
#define BICO_CHECK_BIT_CHANGE

#include "Arduino.h"

class Bico_Check_Bit_Change
{
  public:
    Bico_Check_Bit_Change(uint8_t init_val);
    uint8_t isChanging(uint8_t val);

  private:
    uint8_t _last_state;
};

#endif
