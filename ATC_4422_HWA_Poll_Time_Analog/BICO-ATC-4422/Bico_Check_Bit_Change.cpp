#include "Bico_Check_Bit_Change.h"

Bico_Check_Bit_Change::Bico_Check_Bit_Change(uint8_t init_val)
{
  _last_state = init_val;
}

uint8_t Bico_Check_Bit_Change::isChanging(uint8_t val)
{
  if(val != _last_state)
  {
    if(_last_state == HIGH)
    {
      _last_state = val;
      return FALLING;
    }

    if(_last_state == LOW)
    {
      _last_state = val;
      return RISING;
    }
  }
  return 255;
}
