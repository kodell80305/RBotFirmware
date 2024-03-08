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
  

    void setup(ConfigBase &hwConfig,  RestAPIEndpoints &endpoints);
    void service();

    void status(String newstatus, String fileName);
    void handlePlay(char *DisplayData);
    void handleSplash(char *DisplayData);

private:
    RestAPIEndpoints _restAPIEndpoints;

};
