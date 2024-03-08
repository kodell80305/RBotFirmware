#include "Display.h"
#include "pins_arduino.h"
#include "HardwareSerial.h"
#include <WiFi.h>
#include "RobotMotion/MotionControl/i2s_lcl.h"

#include <Adafruit_NeoPixel.h>



void sendToDisplay(const char *sendStr) {
    Log.notice("Sending %s to Display\n", sendStr);
    Serial2.print(sendStr);

    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
}


#define NUM_LEDS 20

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, 32, NEO_GRB);
int redVal=0;
int blueVal=255;
int greenVal=0;
int dimVal=75;

//TFT_eSPI tft = TFT_eSPI();
#if 0
MCStepper @ 0.7.3
|-- WiFi @ 1.0
|-- ESP32 BLE Arduino @ 1.0.1
|-- lv_arduino @ 2.1.5
|-- BluetoothSerial @ 1.0
|-- WebServer @ 1.0
|-- WiFiClientSecure @ 1.0
|-- WebSockets @ 2.1.2
|-- DNSServer @ 1.1.0
|-- ESP32SSPD @ 1.0
|-- ESPmDNS @ 1.0
|-- FS @ 1.0
|-- SD(esp32) @ 1.0.5
|-- SPIFFS @ 1.0
|-- Update @ 1.0
|-- ArduinoOTA @ 1.0
|-- TFT_eSPI @ 2.3.70
|-- EEPROM @ 1.0.3
|-- Preferences @ 1.0
|-- Wire @ 1.0.1
|-- SPI @ 1.0
#endif

void grbl_send(uint8_t client, const char* text) {};
void grbl_sendf(uint8_t client, const char* format, ...) {};
//void grbl_msg_sendf(uint8_t client, MsgLevel level, const char* format, ...) {};

float mpos=0.0;
float* system_get_mpos() {return &mpos;};
void   mpos_to_wpos(float* position) {};
void report_status_message(int status_code, uint8_t client) {};

void
lcd_setup() {

   
    strip.begin();
    strip.setBrightness(255);
    for(int i=0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i+0, strip.Color(redVal, greenVal, blueVal));    }
    strip.show();


}

#if 0

    #include <U8g2lib.h>

// obviously replace that by the pins you are using
#define LCD_CLOCK 18  // Clock (Common), sometimes called SCK or SCL
#define LCD_MOSI 23   // MOSI (common), sometimes called SDA or DATA
#define LCD_RESET 0   // LCD reset, sometimes called RST or RSTB
#define LCD_CS 21      // LCD CS, sometimes called EN or SS
#define LCD_RS 4      // LCD RS, sometimes called A0 or DC

U8G2_ST7567_JLX12864_1_4W_SW_SPI u8g2_lcd(U8G2_R0, 
                                          LCD_CLOCK, 
                                          LCD_MOSI, 
                                          LCD_CS, 
                                          LCD_RS, 
                                          LCD_RESET); // clock, data, cs, dc, reset

    u8g2_lcd.begin();Filef
#endif

unsigned long lastMillis=0;
#if 0
#include <stdint.h>
#include "mks/MKS_TS35.h"
#include "mks/MKS_LVGL.h"
#include "mks/mks_test.h"
#include "mks/MKS_SDCard.h"
#endif
//#include <vfs_api.h>


//SDFS SD = SDFS(FSImplPtr(new VFSImpl()));

void Display::setup(ConfigBase &hwConfig, RestAPIEndpoints &endpoints) {

     _restAPIEndpoints = endpoints;
#define RXD2 0 //todo - ADD TO .json file
#define TXD2 32
#define LCD_EN					    GPIO_NUM_5    


   Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

    //Reset Display

    sendToDisplay("rest");

    lastMillis=millis();

    lcd_setup();
   // mks_listDir(SD, "/",1);


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
handleSplash(char *DisplayData) {
    Log.notice("handleSplash called\n");

    //Init once values:

    char sendStr[80];
    sprintf(sendStr, "dim=%d", dimVal);
    sendToDisplay(sendStr);

    sprintf(sendStr, "Splashscreen.ledRed.val=%d", redVal);
    sendToDisplay(sendStr);

    
    sprintf(sendStr, "Splashscreen.ledGreen.val=%d", greenVal);
    sendToDisplay(sendStr);
    
    sprintf(sendStr, "Splashscreen.ledBlue.val=%d", blueVal);
    sendToDisplay(sendStr);


}

#include "FileManager.h"
 #include "RestAPIEndpoints.h"

void
handleFileSelect(char *DisplayData) {
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
    sendToDisplay(sendStr);

    for(int i=0; i < count; i++) {
        

        sprintf(sendStr, "va%d.txt=\"%s\"", i, filename[i].c_str());
        sendToDisplay
    (sendStr);
    }
}

void
handleRed(char *DisplayData) {
    sscanf(DisplayData, "ledRed %d", &redVal);
    Log.trace("Display set red to %d %s\n", redVal, DisplayData);
    
    for(int i=0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i+0, strip.Color(redVal, greenVal, blueVal));
    }
    strip.show();
}
void
handleGreen(char *DisplayData) {
    sscanf(DisplayData, "ledGreen %d", &greenVal);
    Log.trace("Display set green to %d %s\n", greenVal, DisplayData);
    
    for(int i=0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i+0, strip.Color(redVal, greenVal, blueVal));
    }
    strip.show();
}

void
handleBlue(char *DisplayData) {
    sscanf(DisplayData, "ledBlue %d", &blueVal);
    Log.trace("Displayset blue to %d %s\n", blueVal, DisplayData);
    
    for(int i=0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i+0, strip.Color(redVal, greenVal, blueVal));
    }
    strip.show();
}

void
handleRobotPosition(char *DisplayData) {
}


void
handleManualControl(char *DisplayData) {
}

void
handleHome(char *DisplayData) {
}
void
handleClean(char *DisplayData) {
}

void
Display::handlePlay(char *DisplayData) {
}
void
handlePlay(char *DisplayData) {
    extern RestAPIEndpoints restAPIEndpoints;

    Log.trace("Display cmdStr %s\n", DisplayData);
    String retStr;
     restAPIEndpoints.handleApiRequest(DisplayData, retStr);
            // Display response
    Serial.println(retStr);
    Serial.println();
}
void
handleExec(char *DisplayData) {
    extern RestAPIEndpoints restAPIEndpoints;

    Log.trace("Display cmdStr %s\n", DisplayData);
    String retStr;
     restAPIEndpoints.handleApiRequest(DisplayData, retStr);
            // Display response
    Serial.println(retStr);
    Serial.println();
}

void
handleNop(char *DisplayData) {}





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

void Display::status(String newStatus,String fileName)
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
    sendToDisplay(sendStr);
#endif
    
extern uint32_t getI2S();


    snprintf(sendStr, sizeof(sendStr), "Splashscreen.isPaused.val=%s %x %d", pause.c_str(), getI2S(), flag);
    sendToDisplay(sendStr);

    snprintf(sendStr, sizeof(sendStr), "Splashscreen.Qd.val=%s", Qd.c_str());
    sendToDisplay(sendStr);

    snprintf(sendStr, sizeof(sendStr), "Splashscreen.progress.val=%d", pos);
    sendToDisplay(sendStr);


    snprintf(sendStr, sizeof(sendStr), "Splashscreen.fileName.txt=\"%s \"", fileName.c_str());
    sendToDisplay(sendStr);

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
    //sendToDisplay(sendStr
    snprintf(sendStr, sizeof(sendStr), "Splashscreen.ABC.txt=\"%3.0f deg / %3.0f %%\"", theta,rho);
    sendToDisplay(sendStr);

    //snprintf(sendStr, sizeof(sendStr), "Splashscreen.status.txt=\"XYZ %s ABC %s end %s tod %s pause %s Qd %s\"", XYZ.c_str(), ABC.c_str(), end.c_str(), tod.c_str(), pause.c_str(), Qd.c_str());

    //sendToDisplay(sendStr);
    //Log.notice("Display status update %s\n", newStatus.c_str());
#endif
}




// Receives data on serial port 1 as sent to Display (received by Display)
// Formats for easy reading in serial monitor

const uint8_t bufferSize = 128;
char DisplayDataRx[bufferSize];
uint8_t DisplayDataIndex = 0;
bool dataReady = false;
bool overflow = false;


bool RxDisplayData() {
  char RxTemp;

  static uint8_t ffCount;

  while (Serial2.available() > 0) {
    RxTemp = Serial2.read();
     Serial.println("next=" + String(RxTemp));
    if (RxTemp == 0xff) {
      ++ffCount;
      if (ffCount >= 3) {
        dataReady= (DisplayDataIndex > 0);
   
        ffCount = 0;
        DisplayDataRx[DisplayDataIndex] = 0;
        DisplayDataIndex = 0;
        Serial.println("ffCount=" + String(ffCount));
        return dataReady;
      }
    } else {
        if(isprint(RxTemp)) {
            DisplayDataRx[DisplayDataIndex] = RxTemp;
            ++DisplayDataIndex;
            if (DisplayDataIndex >= bufferSize) {
                DisplayDataRx[bufferSize - 1] = 0;
                dataReady = true;
                overflow = true;
                return true;
            
            }
        }  //else we should handle this - it's a Display
     return dataReady;
    }
  }
  return false;
}


#define NUM_CMDS 11

#if 1
  Commands commands[] = {
    { "SplashScreen", handleSplash},
    { "FileSelect", handleFileSelect},
    { "RobotPostiton", handleRobotPosition},
    { "ManualControl", handleManualControl},
    { "Home", handleHome},
    { "Clean", handleClean},
    { "playFile", handlePlay},
    { "/exec", handleExec},
    { "ledRed", handleRed},
    { "ledBlue", handleBlue},
    { "ledGreen", handleGreen},
    { "", handleNop}

};
#endif
void Display::service() {



    if(RxDisplayData()) {
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