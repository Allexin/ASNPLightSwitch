#include "SwitchHandler.h"

#define ENC_A 5
#define ENC_B 3 
#define ENC_P 4

const int ENCODER_MIN_BORDER = 2;
const long LONG_PRESS_TIME = 1000;
const long DEBOUNCE_TIME = 50;

const uint8_t MAIN_CHANNEL = 3;
const uint8_t GREEN_CHANNEL = 1;


volatile int encoderPos = 0;

void doEncoder() {
  
  /* If pinA and pinB are both high or both low, it is spinning
     forward. If they're different, it's going backward.
  */
  if (digitalRead(ENC_A)==digitalRead(ENC_B)){
    encoderPos++;   
  }
  else {
    encoderPos--;    
  }
}

cSwitchHandler::cSwitchHandler(cPortHandler* port){
  m_Port = port;
  for (int i = 0; i<4; ++i)
    m_Channels[i] = 0;
  m_TurnOnStepTime = 0;
  m_TurnOffStepTime = 0;
  m_DimmerAddress = 254;

  m_EncoderPos = 0;

  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_B), doEncoder, CHANGE); 

  pinMode(ENC_P, INPUT);

  digitalWrite(ENC_P,LOW);
  digitalWrite(ENC_A,HIGH);
  digitalWrite(ENC_B,HIGH);

  pinMode(13, OUTPUT);
}

void cSwitchHandler::setState(uint8_t channels[4]){
  for (int i = 0; i<4; ++i)
    m_Channels[i] = channels[i];
}

void cSwitchHandler::setDimmer(uint8_t address){
  m_DimmerAddress = address;  
}
void cSwitchHandler::setTimers(uint8_t turnOnStepTime, uint8_t turnOffStepTime){
  m_TurnOnStepTime = turnOnStepTime;
  m_TurnOffStepTime = turnOffStepTime;
}

cSwitchHandler::eState cSwitchHandler::getState(){
  if (m_Channels[3]>0)
    return MAIN;
  if (m_Channels[0]>0 || m_Channels[1]>0 || m_Channels[2]>0)
    return ADDITIONAL;
  return OFF;
}

bool cSwitchHandler::update(long dt){
  bool needSave = false;
  m_EncoderPos+=encoderPos;
  encoderPos = 0;

  if (m_ButtonPressed && m_TimeFromPress<LONG_PRESS_TIME){
    m_TimeFromPress+=dt;
    if (m_TimeFromPress>=LONG_PRESS_TIME){
      processLongPress();
      needSave = true;
    }
  }

  bool buttonPressed = digitalRead(ENC_P)==HIGH;
  if (buttonPressed!=m_ButtonPressed){
    if (buttonPressed){
      m_TimeFromPress = 0;
      
    }
    else{
      if (m_TimeFromPress>DEBOUNCE_TIME && m_TimeFromPress<LONG_PRESS_TIME){
        processShortPress();
      }      
    }
    m_ButtonPressed = buttonPressed;
  }

  eState state = getState();

  if (m_EncoderPos!=0){
    if (state!=OFF || m_EncoderPos<=-ENCODER_MIN_BORDER || m_EncoderPos>=ENCODER_MIN_BORDER){
      processEncoder();
    }
  }
  return needSave;
}


void cSwitchHandler::processEncoder(){
  if (m_EncoderPos==0)
    return;

  if (m_EncoderPos<-128)
    m_EncoderPos = -128;
  if (m_EncoderPos> 128)
    m_EncoderPos =  128;

  eState state = getState();
  if (m_EncoderPos>0){
    switch(state){
      case OFF:{
        m_Channels[MAIN_CHANNEL] = m_EncoderPos;
        sendNewState(0);
      };break;
      case MAIN:{
        long newValue = m_Channels[MAIN_CHANNEL] + m_EncoderPos;
        if (newValue>255){
          m_Channels[MAIN_CHANNEL] = 255;
        }
        else{
          m_Channels[MAIN_CHANNEL] = newValue;  
        }        
        sendNewState(0);
      };break;
      case ADDITIONAL:{
        long newValue = m_Channels[GREEN_CHANNEL] + m_EncoderPos;
        if (newValue>255){
          m_Channels[GREEN_CHANNEL] = 255;
        }
        else{
          m_Channels[GREEN_CHANNEL] = newValue;  
        }        
        sendNewState(0);
      };break;
    }
  }
  else{
    switch(state){
      case OFF:{
        //do nothing
      };break;
      case MAIN:{
        long newValue = m_Channels[MAIN_CHANNEL] + m_EncoderPos;
        if (newValue<0){
          m_Channels[MAIN_CHANNEL] = 0;
        }
        else{
          m_Channels[MAIN_CHANNEL] = newValue;  
        }        
        sendNewState(0);
      };break;
      case ADDITIONAL:{
        long newValue = m_Channels[GREEN_CHANNEL] + m_EncoderPos;
        if (newValue<0){
          m_Channels[GREEN_CHANNEL] = 0;
        }
        else{
          m_Channels[GREEN_CHANNEL] = newValue;  
        }        
        sendNewState(0);
      };break;
    }
  }
  m_EncoderPos = 0;
}
void cSwitchHandler::processShortPress(){
  eState state = getState();
  switch(state){
    case OFF:{
      m_Channels[MAIN_CHANNEL] = m_MainDefault;
      sendNewState(m_TurnOnStepTime);
    };break;
    case MAIN:{
      for (int i = 0; i<4; ++i)
        m_Channels[i] = m_Channels[i] / 2;
      sendNewState(0);
      for (int i = 0; i<4; ++i)
        m_Channels[i] = 0;
      sendNewState(m_TurnOffStepTime);
    };break;
    case ADDITIONAL:{
      for (int i = 0; i<4; ++i)
        m_Channels[i] = m_Channels[i] / 2;
      sendNewState(0);
      for (int i = 0; i<4; ++i)
        m_Channels[i] = 0;
      sendNewState(m_TurnOffStepTime);
    };break;
  }
}
void cSwitchHandler::processLongPress(){
  eState state = getState();
  switch(state){
    case OFF:{
      m_Channels[GREEN_CHANNEL] = m_GreenDefault;
      sendNewState(m_TurnOnStepTime);
    };break;
    case MAIN:{
      m_MainDefault = m_Channels[MAIN_CHANNEL];
      for (int i = 0; i<4; ++i)
        m_Channels[i] = m_Channels[i] / 2;
      sendNewState(0);
      for (int i = 0; i<4; ++i)
        m_Channels[i] = 0;
      sendNewState(m_TurnOffStepTime);
    };break;
    case ADDITIONAL:{
      m_GreenDefault = m_Channels[GREEN_CHANNEL];
      for (int i = 0; i<4; ++i)
        m_Channels[i] = m_Channels[i] / 2;
      sendNewState(0);
      for (int i = 0; i<4; ++i)
        m_Channels[i] = 0;
      sendNewState(m_TurnOffStepTime);
    };break;
  }
}

void cSwitchHandler::sendNewState(uint8_t timeOnStep){
  uint8_t buf[7];
  buf[0] = 0x01;
  buf[1] = 0x03;
  buf[2] = m_Channels[0];
  buf[3] = m_Channels[1];
  buf[4] = m_Channels[2];
  buf[5] = m_Channels[3];
  buf[6] = timeOnStep;
  m_Port->queuePackage(START_CHAR_HIGH,m_DimmerAddress,buf,7);
}


