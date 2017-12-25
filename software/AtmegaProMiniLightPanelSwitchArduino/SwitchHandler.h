#ifndef SWITCH_HANDLER_H
#define SWITCH_HANDLER_H
#include <Arduino.h>
#include "PortHandler.h"

class cSwitchHandler{
private:
  enum eState{
    OFF,
    ADDITIONAL,
    MAIN
  };

  cPortHandler* m_Port;
  uint8_t m_Channels[4];
  uint8_t m_TurnOnStepTime;
  uint8_t m_TurnOffStepTime;
  uint8_t m_DimmerAddress;
  uint8_t m_GreenDefault;
  uint8_t m_MainDefault;
  
  long m_EncoderPos;

  bool m_ButtonPressed;
  long m_TimeFromPress;
  
  eState getState();
  void processEncoder();
  void processShortPress();
  void processLongPress();
  void sendNewState(uint8_t timeOnStep);
public:
  cSwitchHandler(cPortHandler* port);
  
  void setState(uint8_t channels[4]);
  void setTimers(uint8_t turnOnStepTime, uint8_t turnOffStepTime);
  void setDimmer(uint8_t address);
  void setDefaults(uint8_t greenDefault, uint8_t mainDefault){
    m_GreenDefault = greenDefault;
    m_MainDefault = mainDefault;
  }
  uint8_t getGreenDefault(){
    return m_GreenDefault;
  }
  uint8_t getMainDefault(){
    return m_MainDefault;
  }
  bool update(long dt);
};

#endif
