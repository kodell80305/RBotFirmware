#ifdef ENABLE_DISPLAY
#include <Arduino.h>

//#include <esp32_smartdisplay.h>
//#include <ui/ui.h>
#include "Display.h"
#include "pins_arduino.h"
#include "HardwareSerial.h"
#include <WiFi.h>

int dimVal=75;


static SerialDisplay *_instance;

SerialDisplay *SerialDisplay::getInstance() {
    return _instance;
}

                           
//For MKS Tiny Bee
//#define RXD2 16 //todo - ADD TO .json file
//#define TXD2 17

void SerialDisplay::writeSerialDisplay(const char *sendStr, bool blocking) {
#ifdef SERIAL_DISPLAY
    int i;
    for(i=0; i < strlen(sendStr); i++) {
        buffer_out[out_wr++] = sendStr[i];
        if(out_wr >= BUF_SIZE) out_wr = 0;
    }
    for(i=0; i < 3; i++) {
        buffer_out[out_wr++] = 0xff;
        if(out_wr >= BUF_SIZE) out_wr = 0;
    }


   //if(blocking && !Serial2.availableForWrite()) {
   //     Log.notice("Sending %s to Display blocked\n", sendStr);
    //    return;
   //}
    //Serial2.printf("%s%c%c%c", sendStr, 0xff, 0xff, 0xff);
#endif
}


void 
SerialDisplay::setup(ConfigBase &hwConfig, RestAPIEndpoints &endpoints) {
#ifdef SERIAL_DISPLAY
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
    Serial2.setRxBufferSize(256);
#endif
    _restAPIEndpoints = endpoints;
    fileList = "";

    _instance = this;
    sleepOn = false;

    //Reset Display
    out_rd = 0;
    out_wr = 0;
    end0 = 0;
    end1 = 0;

    lastRobotStatus = robotStatus = ROBOT_IDLE;


    writeSerialDisplay("rest");
}

void
SerialDisplay::handleSplash(char *DisplayData) {
    Log.notice("handleSplash called\n");

    DisplayPage = SPLASHSCREEN;

    //Init once values:

    char sendStr[80];
    sprintf(sendStr, "dim=%d", dimVal);
    writeSerialDisplay(sendStr);

    _ledStrip.getRGB(brightness, redVal, greenVal, blueVal);

    sprintf(sendStr, "p[0].ledRed.val=%d", redVal);
    writeSerialDisplay(sendStr);

    sprintf(sendStr, "p[0].ledGreen.val=%d", greenVal);
    writeSerialDisplay(sendStr);
    
    sprintf(sendStr, "p[0].ledBlue.val=%d", blueVal);
    writeSerialDisplay(sendStr);

}


void
SerialDisplay::handleFileSelect(char *DisplayData) {
    DisplayPage = FILESELECT;
    int fileSelectPage;
    char sendStr[80];

    //Get page of file select
    sscanf(DisplayData, "FileSelect %d", &fileSelectPage);
    Log.notice("handleFileSelect called page %d\n", fileSelectPage);

    if(fileSelectPage == 0) {

        sprintf(sendStr, "maxFiles.val=%d", maxFiles);
        writeSerialDisplay(sendStr);

        int maxPage = maxFiles/FILES_PER_PAGE;
        if(maxFiles%FILES_PER_PAGE != 0) maxPage++;

        sprintf(sendStr, "maxPage.val=%d", maxPage-1);
        writeSerialDisplay(sendStr);       
    }

    int fileNo;
    for(int i=0; i < FILES_PER_PAGE; i++) {
        fileNo = i + fileSelectPage*FILES_PER_PAGE;

        if(fileNo >= maxFiles) {
            sprintf(sendStr, "t%d.txt=\"\"");
        } else {
            sprintf(sendStr, "t%d.txt=\"%s\"", i, filename[fileNo].c_str());
        }
        Log.notice("%s\n", sendStr);
        writeSerialDisplay(sendStr);
    }

}

void
SerialDisplay::handleRed(char *DisplayData) {

    _ledStrip.getRGB(brightness, redVal, greenVal, blueVal);
    sscanf(DisplayData, "ledRed %d", &redVal);
    Log.trace("Display set red to %d %s\n", redVal, DisplayData);
    _ledStrip.setRGB(brightness, redVal, greenVal, blueVal);
}
void
SerialDisplay::handleGreen(char *DisplayData) {
    _ledStrip.getRGB(brightness, redVal, greenVal, blueVal);
    sscanf(DisplayData, "ledGreen %d", &greenVal);
    Log.trace("Display set green to %d %s\n", greenVal, DisplayData);
    _ledStrip.setRGB(brightness, redVal, greenVal, blueVal);
}

void
SerialDisplay::handleBlue(char *DisplayData) {
    _ledStrip.getRGB(brightness, redVal, greenVal, blueVal);
    sscanf(DisplayData, "ledBlue %d", &blueVal);
    Log.trace("Displayset blue to %d %s\n", blueVal, DisplayData);
    _ledStrip.setRGB(brightness, redVal, greenVal, blueVal);

}

void
SerialDisplay::handlePlay(char *DisplayData) {

    //See if file ends in .param - strip off the playFile/

    String strDisplay=DisplayData;

    if(strDisplay.indexOf(".param")) {
        //This is a param file
        lastParam=strDisplay.substring(9, strDisplay.indexOf(".param"));
    } else {
        lastParam="";
    }
    Log.trace("Display cmdStr %s\n", DisplayData);
    String retStr;
    _restAPIEndpoints.handleApiRequest(DisplayData, retStr);
            // Display response
    Serial.println(retStr);
    Serial.println();
}
void
SerialDisplay::handleExec(char *DisplayData) {
    Log.trace("Display cmdStr %s\n", DisplayData);
    String retStr;
    _restAPIEndpoints.handleApiRequest(DisplayData, retStr);
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



void
SerialDisplay::calculateUptime(char *timeString, int timeStrLen) {
    unsigned long uptime = millis()/1000;

    timeString[0] = 0;

    if(uptime < 120) {
        snprintf(timeString, timeStrLen, "%ld sec", uptime);
    } else {
         uptime/=60;
        if(uptime < 320) {
            snprintf(timeString,timeStrLen, "%ld min", uptime);
        } else {
            uptime/=60;
            if(uptime < 48) {
                snprintf(timeString, timeStrLen, "%ld hours", uptime);
            } else {
                 uptime /= 24;
                snprintf(timeString, timeStrLen, "%ld days", uptime);
            }
         }
     }
}

void
SerialDisplay::handleSettings(char *Display) {
    DisplayPage = SETTINGS;

}

void SerialDisplay::status(String newStatus, FileManager& _fileManager)
{
    //Sending p[0].status.txt="{"wifiIP":"192.168.50.237","wifiConn":"C","ssid":"deagol","MAC":"8:f9:e0:9e:c2:58","RSSI":-39,"espV":"2.028.002",
    //"XYZ":[0.00,0.00,0.00],"ABC":[0,0,0],"mv":"abs","end":[[2,0], to Display

    //   IP 192.168.50.237 Heap 129364 QOK:0 Avg 91.46uS Max 34819uS Min 80uS Slowest Status 34464, LoopTimer 3665
    //I: Sending p[0].status.txt="[-237.07,262.60,0.00] [18474,-12882,0]" to Display
    //I: Display status update {"wifiIP":"192.168.50.23Wo,"Hmd":0,"pause":0,"ledOn":1,"ledValue":127,"autoDim":0,"tod":"2024-02-22 03:07:14"}
    //I: Display status update {"wifiIP":"192.168.50.237","wifiConn":"C","ssid":"deagol","MAC":"8:f9:e0:9e:c2:58","RSSI":-38,"espV":"2.028.002","XYZ":[-294.68,196.48,0.00],
    //"ABC":[18088,-12509,0],"mv":"abs",
    //"end":[[2,0],[1,0],[0,0]],"OoB":"N","num":0,"Qd":98,"Hmd":0,"pause":0,"ledOn":1,"ledValue":127,"autoDim":0,"tod":"2024-02-22 03:06:46"}
 

    //String WifiIP = RdJson::getString("wifiIP", "", newStatus.c_str());
    String XYZ = RdJson::getString("XYZ", "", newStatus.c_str());
    //String ABC = RdJson::getString("ABC", "", newStatus.c_str());

    String end = RdJson::getString("end", "", newStatus.c_str());

    String pause = RdJson::getString("pause", "", newStatus.c_str());
    String Qd =  RdJson::getString("Qd","" , newStatus.c_str());
    String Hmd = RdJson::getString("Hmd", "", newStatus.c_str());
    String Homing = RdJson::getString("Homing", "", newStatus.c_str());
    String fileName; 

    updateFileList(_fileManager);

    //Update the status when the robot is running:
    int queueLen;
    float pos=100.0;
    int isPaused;
    int isHomed;
    int isHoming = 0;

    char sendStr[200];

    sscanf(Qd.c_str(), "%d",&queueLen);
    sscanf(pause.c_str(),"%d", &isPaused);
    sscanf(Hmd.c_str(), "%d", &isHomed);
    sscanf(end.c_str(), "[[%d,0],[%d", &end0, &end1);



    if(Homing.length() > 0) {
        sscanf(Homing.c_str(), "%d", &isHoming);
    }



    //State can be one of:
    //      -Idle/Stopped
    //      -Paused
    //      -Playing a pattern or a file
    //      -Homing
    //


    if(queueLen > 0) {  //Robot is running or paused?

        int len = _fileManager.getFileLen();
        int filePos = _fileManager.getFilePos();


        if(len > 0) {
            pos = filePos*100.0/len;
        } else {
            pos = 100;
        }
        if(isHoming) {
            snprintf(sendStr, sizeof(sendStr), "p[0].status.txt=\"Homing\"");

            fileName = "homing";
            robotStatus = ROBOT_HOMING;

            lastFilePlayed = fileName;

        } else {
           // Log.notice("checking filemanager active filename %s/ %s \n", fileName, _workManager.evaluatorsPattern());
            if(_workManager.evaluatorsBusy(true)) {
                robotStatus = ROBOT_PLAYPATTERN;
                fileName = _workManager.evaluatorsPattern();
    
            } else {
                robotStatus = ROBOT_PLAYFILE;
              
                fileName = _fileManager.getFileName();

                if(fileName.length() == 0) {
                    //See if this is a pattern
                    fileName = _workManager.evaluatorsPattern();
                }
            }

            if(lastParam != "") {
                fileName = lastParam;
            }
        
            int lpos = fileName.lastIndexOf('/');

            if(lpos > 0) {
                fileName = fileName.substring(lpos+1);
            }
            lastFilePlayed = fileName;

            if(isPaused) {
                robotStatus = ROBOT_PAUSED;

                snprintf(sendStr, sizeof(sendStr), "p[0].status.txt=\"Pause %s %3.1f %% done\"", fileName.c_str(), pos);
            }  else {

                snprintf(sendStr, sizeof(sendStr), "p[0].status.txt=\"Playing %s %3.1f%% done (queue %d)\"", fileName.c_str(), pos, queueLen);
            }
        }

    } else {


        robotStatus = ROBOT_IDLE;

        if(isHomed == 0) {
            String WifiIP = RdJson::getString("wifiIP", "", newStatus.c_str());
            String tod = RdJson::getString("tod", "", newStatus.c_str());
            snprintf(sendStr, sizeof(sendStr), "p[0].status.txt=\"Not homed: %s %s %s\"",  WiFi.getHostname(), WifiIP.c_str(), tod.c_str());
        } else {
            snprintf(sendStr, sizeof(sendStr), "p[0].status.txt=\"Done %s\"",  lastFilePlayed.c_str());
            
        }

    }

    writeSerialDisplay(sendStr);

    snprintf(sendStr, sizeof(sendStr), "p[0].state.val=%d", robotStatus);
    writeSerialDisplay(sendStr);

    if((DisplayPage == RBOTPOS) || (DisplayPage == MANUALCTL)) {
        snprintf(sendStr, sizeof(sendStr), "p[0].progress.val=%d", (int) pos);
        writeSerialDisplay(sendStr);

        snprintf(sendStr,sizeof(sendStr), "p[0].fileName.txt=\"%s \"", fileName.c_str());
        writeSerialDisplay(sendStr);

        snprintf(sendStr,sizeof(sendStr), "end0.txt=\"end0:%s\"", (end0 == 1) ? "hit" : "not hit");
        writeSerialDisplay(sendStr);

        snprintf(sendStr,sizeof(sendStr), "end1.txt=\"end1:%s\"", (end1== 1) ? "hit" : "not hit");
        writeSerialDisplay(sendStr);

        //Also send Hmd & WifiIP (homing might be useful also)
        //We're playing a file, a param or the homing string - above work for file case
    

        //[-218.41,-288.72,0.00]
        float x,y,z;
       // int a,b,c;
        float rho, theta;
        sscanf(XYZ.c_str(), "[%f,%f,%f]",&x,&y,&z);
      //  sscanf(ABC.c_str(), "[%d,%d,%d]",&a,&b,&c);
        rho = sqrt(x*x+y*y);

        //Motors/directions are confusing
        if(rho > 0.0) {
            theta=asin(x/rho);
        } else {
            theta=0;
        }

        theta = theta*180/M_PI;

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

    //SCALE RHOE
        rho /=3.60;   //TODO = get from hardware config

        snprintf(sendStr, sizeof(sendStr), "p[0].ABC.txt=\"%3.0f deg / %3.0f %%\"", theta,rho);
        writeSerialDisplay(sendStr);

        if((end0 == 1) && (end0 !=last_end0)) {
            Log.notice("end0 hit: - theta = %4.1f\n", theta);
        }
        last_end0 = end0;

    } else {
        if(DisplayPage == SETTINGS) {
            char timeString[100];

            calculateUptime(timeString, sizeof(timeString));

            char sendStr[200];

            snprintf(sendStr, sizeof(sendStr), "t4.txt=\"Uptime %s\"", timeString);
            writeSerialDisplay(sendStr);

            snprintf(sendStr, sizeof(sendStr), "t5.txt=\"CPU Temp %7.2f\"", 9.0*temperatureRead()/5.0 + 32.0);
            writeSerialDisplay(sendStr);

            snprintf(sendStr, sizeof(sendStr), "t6.txt=\"Robot ver 0.21` /03/18/2024\"");
            writeSerialDisplay(sendStr);

            snprintf(sendStr, sizeof(sendStr), "t7.txt=\"workMgr: B:%d A:%d, que %d \"", 
                    _workManager.evaluatorsBusy(true) ? 1 : 0, 
                    _workManager.canAcceptWorkItem() ? 1 : 0,
                    queueLen),
                    
            writeSerialDisplay(sendStr);

            
        }
    }

    if(lastRobotStatus != robotStatus) {
        lastRobotStatus = robotStatus;
        snprintf(sendStr, sizeof(sendStr), "wav0.en=1");
        writeSerialDisplay(sendStr);
        Log.notice("Display status update %s\n", newStatus.c_str());

    }

   // Log.notice("Display status update %s\n", newStatus.c_str());

    serviceSerialDisplay();

}

void
SerialDisplay::updateFileList(FileManager& _fileManager) {

    if((fileList.length() == 0) || !_fileManager.isCacheValid()) {
        String retStr;
        _restAPIEndpoints.handleApiRequest("/filelist", retStr);
        Serial.print(retStr);
        fileList = RdJson::getString("files", "", retStr.c_str());

        //Log.notice("Received file system string %s\n FILES %s fsbase %s \nlength %d\n", respStr.c_str(), files.c_str(), fsBase.c_str(), files.length());
        //I'm not sure if the json parser in RdJson will help us here - fine all occurance of "name":"filename"
        //[{"name":"sandbot_latest.tft","size":776020},{"name":"StarryNight.thr","size":68799},{"name":"Butterfly.thr","size":37569},{"name":"ChartresLabyrinthe.thr","size":37084},{"name":"Fractal.thr","size":62054},{"name":"Hosta.thr","size":151377},{"name":"snowflake.thr","size":168857},{"name":"Sponge.thr","size":79131}] fsbase /sd

        int pos = 0;
        int count = 0;
        int len = fileList.length();
        for (;;) {
            pos = fileList.indexOf("name",pos);
            
            if((pos < 0) || (count >= MAX_FILES)) {
                break;
            }
    
            pos += 7;   //Skip name":"
            //Everything up to quote is the file name
            filename[count] = fileList.substring(pos,fileList.indexOf("\"",pos+3));
            Log.notice("filename[%d]=%s\n", count, filename[count].c_str());
        ++count;
        ++pos;
        }
 
        maxFiles = count;
    }

}

void 
SerialDisplay::handleMainMenu(char *DisplayData) {
    DisplayPage = MAIN_MENU;

}
void 
SerialDisplay::handleRbotPos(char *DisplayData) {
    DisplayPage = RBOTPOS;

}
void 
SerialDisplay::handleManualCtl(char *DisplayData) {
    DisplayPage = MANUALCTL;

}
void
SerialDisplay::handleSleep(char *DisplayData) {
    sleepOn = true;

    Log.notice("handleSleep called\n");
    //handleExec("/exec/sleep");
}
void
SerialDisplay::handleWake(char *DisplayData) {
    sleepOn = false;

    Log.notice("handleWakeup called\n");
#ifdef ENABLE_LED   
    for(int i=0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i+0, strip.Color(redVal, greenVal, blueVal));
    }
#endif
    handleExec("/exec/resume");
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
                return idx;
            }
        } else {
            //if(isprint(RxTemp)) {
                DisplayDataRx[displayIdx++] = RxTemp;
                if (displayIdx >= bufferSize) {
                    idx = bufferSize - 1;
                    DisplayDataRx[idx] = 0;
                    ffCount = 0;
                    return idx;
              //  }
            }  
        }
    }
#endif
  return 0;
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
handleSettings(char *DisplayData) {
    SerialDisplay::getInstance()->handleSettings(DisplayData);
}
void
handleMainMenu(char *DisplayData) {
    SerialDisplay::getInstance()->handleMainMenu(DisplayData);
}
void
handleRbotPos(char *DisplayData) {
    SerialDisplay::getInstance()->handleRbotPos(DisplayData);
}
void
handleManualCtl(char *DisplayData) {
    SerialDisplay::getInstance()->handleManualCtl(DisplayData);
}
void
handleSleep(char *DisplayData) {
    SerialDisplay::getInstance()->handleSleep(DisplayData);
}
void
handleNop(char *DisplayData) {};

#define NUM_CMDS 11

Commands commands[] = {
    { "SplashScreen", handleSplash},
    { "FileSelect", handleFileSelect},
    { "playFile", handlePlay},
    { "Settings", handleSettings},
    { "/exec", handleExec},
    { "ledRed", handleRed},
    { "ledBlue", handleBlue},
    { "ledGreen", handleGreen},
    { "MainMenu", handleMainMenu},
    { "RbotPos", handleRbotPos},
    { "ManualControl", handleManualCtl},
    { "Sleep", handleSleep},
    { "", handleNop}
};

void SerialDisplay::serviceSerialDisplay() {
    while((out_rd != out_wr) && Serial2.availableForWrite()) {
        //Serial.printf("%c", buffer_out[out_rd]);
        Serial2.printf("%c", buffer_out[out_rd++]);
        if(out_rd >= BUF_SIZE) out_rd = 0;
    }
}

void SerialDisplay::service() {

    serviceSerialDisplay();
    int bytesRcvd;

    if((bytesRcvd = readSerialDisplay()) > 0) {
        //Parse string & do appropriate actions
        //Serial.println(DisplayDataRx);
        //If first character is "/", 



        if(bytesRcvd == 1)  {
            //This is a status code ..
            Log.notice("Display received %x\n", DisplayDataRx[0]);

            if(DisplayDataRx[0] == 0x86) {
                handleSleep(DisplayDataRx);
            }
        } else {
            //If we're sleeping, any character from screen should wake us up ...
            if(sleepOn) {
                handleWake(DisplayDataRx);
            }

            Log.notice("Display received %s\n", DisplayDataRx);



            for(int i=0; i < sizeof(commands)/sizeof(Commands) && commands[i].cmds && (strlen(commands[i].cmds) > 0); i++) {
                //Log.notice("Checking received %s \n", commands[i].cmds);

    
              if(!strncmp(DisplayDataRx, commands[i].cmds, strlen(commands[i].cmds))) {
                   Log.notice("Received command %s\n", commands[i].cmds);
                  commands[i].handler(DisplayDataRx);
                  serviceSerialDisplay();
                  return;
                }
            }
        }

    }
}
#endif