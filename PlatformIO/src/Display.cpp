#ifdef ENABLE_DISPLAY
#include <Arduino.h>

//#include <esp32_smartdisplay.h>
//#include <ui/ui.h>
#include "Display.h"
#include "pins_arduino.h"
#include "HardwareSerial.h"
#include <WiFi.h>
#include "FileManager.h"
#include "RestAPIEndpoints.h"

#include "RobotMotion/MotionControl/i2s_lcl.h"

#include <Adafruit_NeoPixel.h>

 static SerialDisplay *_instance;

SerialDisplay *SerialDisplay::getInstance() {
    return _instance;
}

SerialDisplay::SerialDisplay() {
    //SerialDisplay should be a singleton -
    //TODO - check if it exists, disallow copy, etc.
    _instance = this;
}
#define NUM_LEDS 4 //78

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_WS2812B_PIN, NEO_GRB);
int redVal=0;
int blueVal=255;
int greenVal=0;
int dimVal=75;

void
lcd_setup() {

   
    strip.begin();
    strip.setBrightness(255);
    for(int i=0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i+0, strip.Color(redVal, greenVal, blueVal));    }
    strip.show();


}
                           
//For MKS Tiny Bee
//#define RXD2 16 //todo - ADD TO .json file
//#define TXD2 17

void SerialDisplay::writeSerialDisplay(const char *sendStr) {
#ifdef SERIAL_DISPLAY
    if(Serial2.availableForWrite()) {
        Log.notice("Sending %s to Display\n", sendStr);
        Serial2.printf("%s%c%c%c", sendStr, 0xff, 0xff, 0xff);
    } else {
         Log.notice("Sending %s to Display blocked\n", sendStr);
    }
#endif
}

//SDFS SD = SDFS(FSImplPtr(new VFSImpl()));

void SerialDisplay::setup(ConfigBase &hwConfig, RestAPIEndpoints &endpoints) {

    _restAPIEndpoints = endpoints;
#ifdef SERIAL_DISPLAY
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
    Serial2.setRxBufferSize(2048);
#endif

    //Reset Display

    writeSerialDisplay("rest");

    lcd_setup();         //TMP


    #if 0
    // Get config
    ConfigBase otaConfig(config.getString("OTAUpdate", "").c_str());
    _otaEnabled = otaConfig.getLong("enabled", 0) != 0;
    _otaDirectEnabled = (otaConfig.getLong("directOk", 1) != 0) && _otaEnabled;
    // Project name must match the file store naming
    _projectName = projectName;
    _currentVers = currentVers;
    // Update server
    _updateServerName = otaConfig.getString("server", "");
    _updateServerPort = otaConfig.getLong("port", 80);
    // Init timer
    _updateStateEntryMs = millis();
#endif

}

void
SerialDisplay::handleSplash(char *DisplayData) {
    Log.notice("handleSplash called\n");

    //Init once values:

    char sendStr[80];
    sprintf(sendStr, "dim=%d", dimVal);
    writeSerialDisplay(sendStr);

    sprintf(sendStr, "Splashscreen.ledRed.val=%d", redVal);
    writeSerialDisplay(sendStr);

    
    sprintf(sendStr, "Splashscreen.ledGreen.val=%d", greenVal);
    writeSerialDisplay(sendStr);
    
    sprintf(sendStr, "Splashscreen.ledBlue.val=%d", blueVal);
    writeSerialDisplay(sendStr);


}

void
SerialDisplay::handleFileSelect(char *DisplayData) {
    Log.notice("handleFileSelect called\n");


    extern FileManager fileManager;
    String respStr;
    String fileSystemStr = "";
    String folderStr = "/";
    fileManager.getFilesJSON(fileSystemStr, folderStr, respStr);
    String files = RdJson::getString("files", "", respStr.c_str());
    String fsBase = RdJson::getString("fsBase", "", respStr.c_str());
#if 0
{
   "rslt": "ok",
   "fsName": "sd",
   "fsBase": "/sd",
   "diskSize": 3.1719424E10,
   "diskUsed": 917504,
   "folder": "/sd/",
   "files": [
      {
         "name": "sandbot_latest.tft",
         "size": 776020
      }
   ]
}
#endif
#define MAX_FILES 50
    String filename[MAX_FILES];
    //Log.notice("Received file system string %s\n FILES %s fsbase %s \nlength %d\n", respStr.c_str(), files.c_str(), fsBase.c_str(), files.length());
    //I'm not sure if the json parser in RdJson will help us here - fine all occurance of "name":"filename"
    //[{"name":"sandbot_latest.tft","size":776020},{"name":"StarryNight.thr","size":68799},{"name":"Butterfly.thr","size":37569},{"name":"ChartresLabyrinthe.thr","size":37084},{"name":"Fractal.thr","size":62054},{"name":"Hosta.thr","size":151377},{"name":"snowflake.thr","size":168857},{"name":"Sponge.thr","size":79131}] fsbase /sd

    int pos = 0;
    int count = 0;
    int len = files.length();
    for (;;) {
        pos = files.indexOf("name",pos);
        Log.notice("got pos %d %d\n", pos, len);
        
        if((pos < 0) || (count >= MAX_FILES)) {
            break;
        }
   
        pos += 7;   //Skip name":"
        //Everything up to quote is the file name
        filename[count] = files.substring(pos,files.indexOf("\"",pos+3));
        Log.notice("Got file name %s\n", filename[count].c_str());
       ++count;
       ++pos;

    }
    char sendStr[80];
    sprintf(sendStr, "maxFiles.val=%d", count);
    writeSerialDisplay(sendStr);

    for(int i=0; i < count; i++) {
        

        sprintf(sendStr, "va%d.txt=\"%s\"", i, filename[i].c_str());
        writeSerialDisplay
    (sendStr);
    }
}

void
SerialDisplay::handleRed(char *DisplayData) {
    sscanf(DisplayData, "ledRed %d", &redVal);
    Log.trace("Display set red to %d %s\n", redVal, DisplayData);
    
    for(int i=0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i+0, strip.Color(redVal, greenVal, blueVal));
    }
    strip.show();
}
void
SerialDisplay::handleGreen(char *DisplayData) {
    sscanf(DisplayData, "ledGreen %d", &greenVal);
    Log.trace("Display set green to %d %s\n", greenVal, DisplayData);
    
    for(int i=0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i+0, strip.Color(redVal, greenVal, blueVal));
    }
    strip.show();
}

void
SerialDisplay::handleBlue(char *DisplayData) {
    sscanf(DisplayData, "ledBlue %d", &blueVal);
    Log.trace("Displayset blue to %d %s\n", blueVal, DisplayData);
    
    for(int i=0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i+0, strip.Color(redVal, greenVal, blueVal));
    }
    strip.show();
}



void
SerialDisplay::handlePlay(char *DisplayData) {
    extern RestAPIEndpoints restAPIEndpoints;

    Log.trace("Display cmdStr %s\n", DisplayData);
    String retStr;
     restAPIEndpoints.handleApiRequest(DisplayData, retStr);
            // Display response
    Serial.println(retStr);
    Serial.println();
}
void
SerialDisplay::handleExec(char *DisplayData) {
    extern RestAPIEndpoints restAPIEndpoints;

    Log.trace("Display cmdStr %s\n", DisplayData);
    String retStr;
     restAPIEndpoints.handleApiRequest(DisplayData, retStr);
            // Display response
    Serial.println(retStr);
    Serial.println();
}

#if 0
    bool _ptUnitsSteps : 1;
    bool _dontSplitMove : 1;
    bool _extrudeValid : 1;
    bool _feedrateValid : 1;
    bool _moveClockwise : 1;
    bool _moveRapid : 1;
    bool _allowOutOfBounds : 1;
    bool _pause : 1;
    bool _moreMovesComing : 1;
    bool _isHoming: 1;
    bool _hasHomed: 1;
    // Command control
    int _queuedCommands;
    int _numberedCommandIndex;
    // Coords etc
    AxisFloats _ptInMM;
    AxisFloats _ptInCoordUnits;
    AxisInt32s _ptInSteps;
    float _extrudeValue;
    float _feedrateValue;
    RobotMoveTypeArg _moveType;
    AxisMinMaxBools _endstops;
#endif

#include <driver/i2s.h>
static uint8_t flag;

void SerialDisplay::status(String newStatus,String fileName)
{
    //Sending Splashscreen.status.txt="{"wifiIP":"192.168.50.237","wifiConn":"C","ssid":"deagol","MAC":"8:f9:e0:9e:c2:58","RSSI":-39,"espV":"2.028.002",
    //"XYZ":[0.00,0.00,0.00],"ABC":[0,0,0],"mv":"abs","end":[[2,0], to Display

 //   IP 192.168.50.237 Heap 129364 QOK:0 Avg 91.46uS Max 34819uS Min 80uS Slowest Status 34464, LoopTimer 3665
//I: Sending Splashscreen.status.txt="[-237.07,262.60,0.00] [18474,-12882,0]" to Display
//I: Display status update {"wifiIP":"192.168.50.23Wo,"Hmd":0,"pause":0,"ledOn":1,"ledValue":127,"autoDim":0,"tod":"2024-02-22 03:07:14"}
//I: Display status update {"wifiIP":"192.168.50.237","wifiConn":"C","ssid":"deagol","MAC":"8:f9:e0:9e:c2:58","RSSI":-38,"espV":"2.028.002","XYZ":[-294.68,196.48,0.00],
//"ABC":[18088,-12509,0],"mv":"abs",
//"end":[[2,0],[1,0],[0,0]],"OoB":"N","num":0,"Qd":98,"Hmd":0,"pause":0,"ledOn":1,"ledValue":127,"autoDim":0,"tod":"2024-02-22 03:06:46"}
#if 1
    String WifiIP = RdJson::getString("wifiIP", "", newStatus.c_str());
    String XYZ = RdJson::getString("XYZ", "", newStatus.c_str());
    String ABC = RdJson::getString("ABC", "", newStatus.c_str());
    String end = RdJson::getString("end", "", newStatus.c_str());
    String tod = RdJson::getString("tod", "", newStatus.c_str());
    String pause = RdJson::getString("pause", "", newStatus.c_str());
    String Qd =  RdJson::getString("Qd","" , newStatus.c_str());
    String Hmd = RdJson::getString("Hmd", "", newStatus.c_str());
    
   

    //Update the status when the robot is running:
    int queueLen;
    int pos=100;
    int isPaused;
    char sendStr[200];
    extern FileManager fileManager;

    sscanf(Qd.c_str(), "%d",&queueLen);

    sscanf(pause.c_str(),"%d", &isPaused);

        

    if(queueLen > 0) {  //Robot is running or paused?

        int len = fileManager.getFileLen();
        int filePos = fileManager.getFilePos();

        if(len > 0) {
            pos = filePos*100.0/len;
        } else {
            pos = 100;
        }

        snprintf(sendStr, sizeof(sendStr), "Splashscreen.status.txt=\"%s %s flag=%d \"", 
            isPaused ? "Paused" : "Playing", fileName.c_str(), flag);

    } else {
        //stopped/homing
        snprintf(sendStr, sizeof(sendStr), "Splashscreen.status.txt=\"%s %s\"",  WiFi.getHostname(), WifiIP.c_str());

    }
#if 1
    writeSerialDisplay(sendStr);
#endif
    
extern uint32_t getI2S();


    snprintf(sendStr, sizeof(sendStr), "Splashscreen.isPaused.val=%s %x %d", pause.c_str(), getI2S(), flag);
    writeSerialDisplay(sendStr);

    snprintf(sendStr, sizeof(sendStr), "Splashscreen.Qd.val=%s", Qd.c_str());
    writeSerialDisplay(sendStr);

    snprintf(sendStr, sizeof(sendStr), "Splashscreen.progress.val=%d", pos);
    writeSerialDisplay(sendStr);


    snprintf(sendStr, sizeof(sendStr), "Splashscreen.fileName.txt=\"%s \"", fileName.c_str());
    writeSerialDisplay(sendStr);

    //Also send Hmd & WifiIP (homing might be useful also)
    //We're playing a file, a param or the homing string - above work for file case
  

    //[-218.41,-288.72,0.00]
    float x,y,z;
    int a,b,c;
    float rho, theta;
    sscanf(XYZ.c_str(), "[%f,%f,%f]",&x,&y,&z);
    sscanf(ABC.c_str(), "[%d,%d,%d]",&a,&b,&c);
    rho = sqrt(x*x+y*y);

    //Motors/directions are confusing
    if(rho > 0.0) {
        theta=asin(x/rho);
    } else {
        theta=0;
    }

    theta = theta*180/M_PI;
#if 1
    if(x>=0) {
        if(y>=0) {
            //First quadrant
        } else {
            //We're in 2nd quadrant
            theta = 180-theta;

        }
    } else {
        if(y<0) {
            //We're in third quadrant
            theta = 180-theta;
        } else {
            //We're in first quadrant
            theta = 360+theta;
        }
    }
#endif
//SCALE RHOE
        rho /=3.57;   //TODO = get from hardware config

    //snprintf(sendStr, sizeof(sendStr), "Splashscreen.XYZ.txt=\"%7.2f,%7.2f\"", y,x);
    //writeSerialDisplay(sendStr
    snprintf(sendStr, sizeof(sendStr), "Splashscreen.ABC.txt=\"%3.0f deg / %3.0f %%\"", theta,rho);
    writeSerialDisplay(sendStr);

    //snprintf(sendStr, sizeof(sendStr), "Splashscreen.status.txt=\"XYZ %s ABC %s end %s tod %s pause %s Qd %s\"", XYZ.c_str(), ABC.c_str(), end.c_str(), tod.c_str(), pause.c_str(), Qd.c_str());

    //writeSerialDisplay(sendStr);
    //Log.notice("Display status update %s\n", newStatus.c_str());
#endif
}






#define MAX_CHAR 15

int
SerialDisplay::readSerialDisplay() {
    char RxTemp;
    
    int maxChar=0;
    int idx = 0;

#ifdef SERIAL_DISPLAY
    while(Serial2.available() > 0) {
        if(maxChar++ >=MAX_CHAR) {
            return false;
        }
        RxTemp = Serial2.read();

        //Serial.printf("next=%02x\n",RxTemp);

        if (RxTemp == 0xff) {
            ++ffCount;
            if (ffCount >= 3) {
                idx = displayIdx;
                DisplayDataRx[idx] = 0;
                ffCount = displayIdx = 0;
            }
        } else {
            if(isprint(RxTemp)) {
                DisplayDataRx[displayIdx++] = RxTemp;
                if (displayIdx >= bufferSize) {
                    idx = bufferSize - 1;
                    DisplayDataRx[idx] = 0;
                    ffCount = 0;
                   
                }
            }  //else we should handle this - it's a Display return code 
        }
    }
#endif
  return idx;
}

void 
handlePlay(char *DisplayData) {
    SerialDisplay::getInstance()->handlePlay(DisplayData);
}
void 
handleSplash(char *DisplayData) {
    SerialDisplay::getInstance()->handleSplash(DisplayData);
}
void 
handleExec(char *DisplayData) {
    SerialDisplay::getInstance()->handleExec(DisplayData);
}
void 
handleRed(char *DisplayData) {
    SerialDisplay::getInstance()->handleRed(DisplayData);
}
void 
handleBlue(char *DisplayData) {
    SerialDisplay::getInstance()->handleBlue(DisplayData);
}
void 
handleGreen(char *DisplayData) {
    SerialDisplay::getInstance()->handleGreen(DisplayData);
}
void
handleFileSelect(char *DisplayData) {
    SerialDisplay::getInstance()->handleFileSelect(DisplayData);
}
void
handleNop(char *DisplayData) {};

#define NUM_CMDS 11

Commands commands[] = {
    { "SplashScreen", handleSplash},
    { "FileSelect", handleFileSelect},
    { "/exec", handleExec},
    { "ledRed", handleRed},
    { "ledBlue", handleBlue},
    { "ledGreen", handleGreen},
    { "", handleNop}

};

void SerialDisplay::service() {
    if(readSerialDisplay() > 0) {
        //Parse string & do appropriate actions
        //Serial.println(DisplayDataRx);
        //If first character is "/", 
        Log.notice("Display received %s\n", DisplayDataRx);

        for(int i=0; i < sizeof(commands)/sizeof(Commands) && commands[i].cmds && (strlen(commands[i].cmds) > 0); i++) {
            //Log.notice("Checking received %s \n", commands[i].cmds);

    
            if(!strncmp(DisplayDataRx, commands[i].cmds, strlen(commands[i].cmds))) {
                Log.notice("Received command %s\n", commands[i].cmds);
                commands[i].handler(DisplayDataRx);
                return;
             }
        }

    }
}
#endif