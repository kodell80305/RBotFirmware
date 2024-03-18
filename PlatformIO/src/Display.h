// NetLog
// Used to log data to various places (MQTT, commandSerial, HTTP) from ArduinoLog module
// Rob Dobson 2018

#pragma once

#include <ArduinoLog.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include "CommandSerial.h"
#include "WorkManager/WorkManager.h"
#include "RestAPIEndpoints.h"
#include "FileManager.h"
#include "LedStrip.h"


typedef void (*CmdHandler)(char *DisplayData);


struct Commands {
    char cmds[128];
    CmdHandler handler;
};

#define FILES_PER_PAGE 5
//todo - HANDLE THIS BETTER
#define MAX_FILES 200

#if 0
class Display 
{
    //Disable copy/construction;
public:
    Display() {};
#ifdef ENABLE_DISPLAY
    void setup(ConfigBase &hwConfig,  RestAPIEndpoints &endpoints, 
        FileManager &fileManager, WorkManager &workManager);
    void service();
    void status(String newstatus);
#else
    //External interface 
    void setup(ConfigBase &hwConfig,  RestAPIEndpoints &endpoints, 
        FileManager &fileManager, WorkManager &workManager) {};
    void service() {};
    void status(String newstatus, String fileName) []
#endif
};
#endif

class SerialDisplay    //: public Display
{
public:

   SerialDisplay( WorkManager &workManager, LedStrip &ledStrip) :
         _workManager(workManager) , _ledStrip(ledStrip){
        lastFilePlayed = "";
        //SerialDisplay should be a singleton -
        //TODO - check if it exists, disallow copy, etc.
    }

    void setup(ConfigBase &hwConfig, RestAPIEndpoints &endpoints);
    void service();
    void status(String newstatus, FileManager& fileManager);

    static SerialDisplay *getInstance();

    void handlePlay(char *DisplayData);
    void handleSplash(char *DisplayData);
    void handleExec(char *DisplayData);
    void handleRed(char *DisplayData);
    void handleGreen(char *DisplayData);
    void handleBlue(char *DisplayData);
    void handleFileSelect(char *DisplayData);
    void handleSettings(char *DisplayData);
    void handleMainMenu(char *DisplayData);
    void handleRbotPos(char *DisplayData);
    void handleManualCtl(char *DisplayData);
    void handleSleep(char *DisplayData);
    void handleWake(char *DisplayData);

private:
    int readSerialDisplay();
    void writeSerialDisplay(const char *sendStr, bool blocking=true);
    void serviceSerialDisplay();
    void calculateUptime(char *timeStr, int len);
    void updateFileList(FileManager& _fileManager);


    static const uint16_t bufferSize = 128;
    char DisplayDataRx[bufferSize+1];
    uint16_t displayIdx;
    uint16_t ffCount;

    RestAPIEndpoints _restAPIEndpoints; 
    WorkManager& _workManager;
    LedStrip& _ledStrip;

    String lastFilePlayed;
    String lastParam;
    int robotStatus;
    int lastRobotStatus;
    int DisplayPage;

#define BUF_SIZE 2048
    char buffer_out[BUF_SIZE];
    int out_rd;
    int out_wr;

    uint8_t brightness;
    uint8_t redVal;
    uint8_t blueVal;
    uint8_t greenVal;

    String filename[MAX_FILES];
    String fileList;
    int maxFiles;

    bool sleepOn;
    int end0;
    int end1;
    int last_end0;
    int last_end1;
};

enum {
    SPLASHSCREEN,
    MAIN_MENU,
    FILESELECT,
    RBOTPOS,
    MANUALCTL,
    SETTINGS
};

enum {
    ROBOT_IDLE,
    ROBOT_PAUSED,
    ROBOT_PLAYFILE,
    ROBOT_PLAYPATTERN,
    ROBOT_HOMING
};