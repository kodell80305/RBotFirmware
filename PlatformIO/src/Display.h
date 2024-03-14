// NetLog
// Used to log data to various places (MQTT, commandSerial, HTTP) from ArduinoLog module
// Rob Dobson 2018

#pragma once

#include <ArduinoLog.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include "CommandSerial.h"
#include "RestAPIEndpoints.h"

typedef void (*CmdHandler)(char *DisplayData);


struct Commands {
    char cmds[128];
    CmdHandler handler;
};



class Display 
{
    //Disable copy/construction;
public:
    Display() {};
#ifdef ENABLE_DISPLAY
    void setup(ConfigBase &hwConfig,  RestAPIEndpoints &endpoints);
    void service();
    void status(String newstatus, String fileName);
#else
    //External interface 
    void setup(ConfigBase &hwConfig,  RestAPIEndpoints &endpoints) {};
    void service() {};
    void status(String newstatus, String fileName) []
#endif
};

class SerialDisplay  : public Display
{
public:
    SerialDisplay();
    void setup(ConfigBase &hwConfig,  RestAPIEndpoints &endpoints);
    void service();
    void status(String newstatus, String fileName);

    static SerialDisplay *getInstance();

    void handlePlay(char *DisplayData);
    void handleSplash(char *DisplayData);
    void handleExec(char *DisplayData);
    void handleRed(char *DisplayData);
    void handleGreen(char *DisplayData);
    void handleBlue(char *DisplayData);
    void handleFileSelect(char *DisplayData);
private:
    int readSerialDisplay();
    void writeSerialDisplay(const char *sendStr);

    static const uint16_t bufferSize = 128;
    char DisplayDataRx[bufferSize+1];
    uint16_t displayIdx;
    uint16_t ffCount;

    RestAPIEndpoints _restAPIEndpoints;

   
};
