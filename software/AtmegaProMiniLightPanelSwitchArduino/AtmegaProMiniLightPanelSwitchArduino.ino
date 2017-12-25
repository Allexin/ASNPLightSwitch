#include "PortHandler.h"
#include "SwitchHandler.h"
#include <EEPROM.h>
#include <avr/wdt.h>

void startWatchDog(){
  wdt_enable(WDTO_1S);
}

void resetWatchDog(){
  wdt_reset();
}

const uint8_t CMD_GROUP = 0x01;

const long DEFAULT_BAUD_RATE = 19200;

cPortHandler* port;
cSwitchHandler* control;

uint8_t addrSelf;
uint8_t addrDimmer;
uint8_t turnOnTimeout;
uint8_t turnOffTimeout;
uint8_t greenSaved;
uint8_t mainSaved;

void setSwitchSettings(){
  control->setTimers(turnOnTimeout,turnOffTimeout);
  control->setDimmer(addrDimmer);
  control->setDefaults(greenSaved,mainSaved);
}

void saveData(){
  EEPROM.write(0, addrSelf);
  EEPROM.write(1, addrDimmer);
  EEPROM.write(2, turnOnTimeout);
  EEPROM.write(3, turnOffTimeout);
  EEPROM.write(4, greenSaved);
  EEPROM.write(5, mainSaved);
  uint8_t sum = addrSelf+addrDimmer+turnOnTimeout+turnOffTimeout+greenSaved+mainSaved;
  EEPROM.write(6, sum); //checksum
  
  setSwitchSettings();
}

void loadData(){  
  addrSelf = EEPROM.read(0); 
  addrDimmer = EEPROM.read(1); 
  turnOnTimeout = EEPROM.read(2); 
  turnOffTimeout = EEPROM.read(3); 
  greenSaved = EEPROM.read(4); 
  mainSaved = EEPROM.read(5); 
  uint8_t checksum = EEPROM.read(6); 
  
  uint8_t sum = addrSelf+addrDimmer+turnOnTimeout+turnOffTimeout+greenSaved+mainSaved;

  if (checksum!=sum || addrSelf==0 || addrDimmer==0 || greenSaved==0 || mainSaved==0){
    addrSelf = 1;
    addrDimmer = 255;
    turnOnTimeout = 8;
    turnOffTimeout = 80;
    greenSaved = 128;
    mainSaved = 255;
  }
}

long long lastUpdate;
long long sendRegisterMessageTimout = 0;

void sendRegisterMessageWithTimeout(){
  sendRegisterMessageTimout = 500+addrSelf*20;
}

void setup() {
  loadData();
  
  Serial.begin(DEFAULT_BAUD_RATE);  

  port = new cPortHandler(&Serial);
  control = new cSwitchHandler(port);
  setSwitchSettings();

  saveData();

  sendRegisterMessageWithTimeout();

  startWatchDog();  

  lastUpdate = millis();
}


void sendRegisterAddr(){
    uint8_t buf[5];
    buf[0] = SYS_MSG;
    buf[1] = 1;//PONG
    buf[2] = 2;
    buf[3] = addrSelf;
    buf[4] = addrDimmer;      
    port->queuePackage(START_CHAR_HIGH,BROADCAST_ADDRESS,buf,5);
}

void sendTraceState(){
  uint8_t buf[5];
  buf[0] = SYS_MSG;
  buf[1] = 0x12;
  buf[2] = addrSelf;
  buf[3] = 1;
  buf[4] = 0;      
  port->queuePackage(START_CHAR_HIGH,BROADCAST_ADDRESS,buf,5);
}

const uint8_t deviceTypeLength = 15;
char deviceInfo[deviceTypeLength] = "LIGHT_SWITCH_EX";
const uint8_t descriptionLength = 46;
char descriptionInfo[descriptionLength] = "Process user input and send commands to dimmer";

void sendInfo(uint8_t hostAddr){
  const uint8_t packageLength = 3+1+deviceTypeLength+1+descriptionLength;
  uint8_t buf[packageLength];
  buf[0] = SYS_MSG;
  buf[1] = 0x21;
  buf[2] = addrSelf;
  
  buf[3] = deviceTypeLength;
  memcpy(&buf[3+1],deviceInfo,deviceTypeLength);
  
  buf[4+deviceTypeLength] = descriptionLength; 
  memcpy(&buf[4+deviceTypeLength+1],descriptionInfo,descriptionLength);
  
  port->queuePackage(START_CHAR_HIGH,BROADCAST_ADDRESS,buf,packageLength);
}

void sendSettings(uint8_t hostAddr){
  uint8_t buf[6];
  buf[0] = SYS_MSG;
  buf[1] = 0x32;
  buf[2] = addrSelf;
  buf[3] = addrDimmer;
  buf[4] = turnOnTimeout;
  buf[5] = turnOffTimeout;    
  port->queuePackage(START_CHAR_HIGH,hostAddr,buf,6);
}

void loop() {
  resetWatchDog();
  
  long long currentTime = millis();
  long dt = currentTime - lastUpdate;
  if (currentTime<lastUpdate)
    dt = currentTime;
  lastUpdate = currentTime;

  if (sendRegisterMessageTimout>0){
      sendRegisterMessageTimout-=dt;
      if (sendRegisterMessageTimout<=0){
        sendRegisterMessageTimout = 0;
        sendRegisterAddr();
      }
  }
  

  if (port->processData(dt)){    
    uint8_t* receivedPackage = port->getReceivedPackage();
    
    uint8_t addr = receivedPackage[ADDRESS_POS];
    uint8_t dataLength = receivedPackage[LENGTH_POS];
    if (dataLength>=2 && (addr==0 || addr==addrSelf || addr==addrDimmer)){
      uint8_t cmdGroup = receivedPackage[DATA_START_POS+0];
      uint8_t cmd = receivedPackage[DATA_START_POS+1];
      
      if (cmdGroup==SYS_MSG){
        switch(cmd){
          case 0x00:{//PING
            if (dataLength==2){//PING
              sendRegisterMessageWithTimeout();
            }
            else{//PONG
              //wtf???
            }
          };break;
          case 0x11:{//TRACE_STEP
            sendTraceState();
          };break;
          case 0x20:{//REQUEST_INFO
            if (dataLength==3){
              sendInfo(receivedPackage[DATA_START_POS+2]);
            }
          };break;
          case 0x30:{//SET_ADDRESS
            if (dataLength==3){
              uint8_t newAddr = receivedPackage[DATA_START_POS+2];
              if (newAddr!=0 && newAddr!=addrSelf){
                addrSelf = newAddr;
                saveData();
                sendRegisterAddr();
              }
            }
          };break;
          case 0x31:{//REQUEST_SETTINGS
            if (dataLength==3 && addr==addrSelf){
              sendSettings(receivedPackage[DATA_START_POS+2]);
            }
          }; break;
          case 0x33:{//SET_SETTINGS
            if (dataLength==6 && addr==addrSelf){
              if (receivedPackage[DATA_START_POS+3]!=0){
                addrDimmer = receivedPackage[DATA_START_POS+3];  
                turnOnTimeout = receivedPackage[DATA_START_POS+4];  
                turnOffTimeout = receivedPackage[DATA_START_POS+5];  
                saveData();
                sendSettings(receivedPackage[DATA_START_POS+2]);
              }
            }            
          }; break;
        }
      }
      else
      if (cmdGroup==CMD_GROUP){        
        if (addr==addrDimmer){
          switch(cmd){
            case 0x03:{//SET_DIMMER_STATE
              if (dataLength==7){
                uint8_t channels[4];
                channels[0] = receivedPackage[DATA_START_POS+2];
                channels[1] = receivedPackage[DATA_START_POS+3];
                channels[2] = receivedPackage[DATA_START_POS+4];
                channels[3] = receivedPackage[DATA_START_POS+5];
                control->setState(channels);
              }
            }; break;
          }
        }
      }
      
    }
  }

  if (control->update(dt)){
    greenSaved = control->getGreenDefault();
    mainSaved = control->getMainDefault();
    saveData();
  }

  port->startSending();
}
