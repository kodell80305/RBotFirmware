// Mod KMO - since this isn't working anyway, repurpose for WS2812B led strip
#include "LedStrip.h"
#include "Arduino.h"
#include "ConfigNVS.h"



FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014


#define DATA_PIN 32
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB


#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120


static const char* MODULE_PREFIX = "LedStrip: ";

LedStrip::LedStrip(ConfigBase &ledNvValues) : _ledNvValues(ledNvValues)
{
    _isSetup = false;
    _isSleeping = false;
    _ledPin = -1;

    
    _redVal=0;
    _blueVal=255;
    _greenVal=0;
    _numLeds = 178;
}

void
LedStrip::updateLed() {
    Log.notice("Updating led brightness %d RGB(%d,%d,%d)", _ledValue, _redVal, _greenVal, _blueVal);              
    for(int i=0; i < _numLeds; i++) {
        leds[i].setRGB(_redVal, _greenVal, _blueVal);
     }
     _ledChanged = false;
    FastLED.show(_ledValue); 
}
void
LedStrip::wake() {
    _isSleeping = false;
    _ledChanged = true;
    updateLed();
}

void 
LedStrip::setRGB(uint8_t brightness, uint8_t redVal, uint8_t greenVal, uint8_t blueVal) {
    _ledValue = brightness;
    _redVal = redVal;
    _greenVal = greenVal;
    _blueVal = blueVal;
    updateLed();
    Log.notice("Set RGB(%d,%d,%d)\n", _redVal, _greenVal, _blueVal);  
}

void 
LedStrip::getRGB(uint8_t &brightness, uint8_t &redVal, uint8_t &greenVal, uint8_t &blueVal) {
    brightness = _ledValue;
    redVal = _redVal;
    greenVal = _greenVal;
    blueVal = _blueVal;
    Log.notice("Get RGB(%d,%d,%d)\n", redVal, greenVal, blueVal);  

}

void LedStrip::setup(ConfigBase* pConfig, const char* ledStripName)
{



    _name = ledStripName;
    _ledChanged = true;
    // Save config and register callback on config changed
    if (_pHwConfig == NULL)
    {
        _pHwConfig = pConfig;
        _pHwConfig->registerChangeCallback(std::bind(&LedStrip::configChanged, this));
    }



    // Get LED config
    ConfigBase ledConfig(pConfig->getString(ledStripName, "").c_str());
    Log.trace("%ssetup name %s configStr %s\n", MODULE_PREFIX, _name.c_str(),
                    ledConfig.getConfigCStrPtr());


    // LED Strip Negative PWM Pin
    String pinStr = ledConfig.getString("ledPin", "");
    int ledPin = -1;
    if (pinStr.length() != 0)
        ledPin = ConfigPinMap::getPinFromName(pinStr.c_str());
    Log.notice("%sLED pin %d\n", MODULE_PREFIX, ledPin);
    //if (ledPin == -1)
//        return;



    // If there is no LED data stored, set to default
    String ledStripConfigStr = ledConfig.getConfigString();
    if (ledStripConfigStr.length() == 0 || ledStripConfigStr.equals("{}")) {
        Log.trace("%sNo LED Data Found in NV Storge, Defaulting\n", MODULE_PREFIX);
        // Default to LED On, Half Brightness
        _ledOn = true;
        _ledValue = 0x7f;
        updateNv();
    } else {
        _ledOn = ledConfig.getLong("ledOn", 0) == 1;
        _ledValue = ledConfig.getLong("ledValue", 0xFF);
        _redVal = ledConfig.getLong("red", 0);
        _greenVal = ledConfig.getLong("green", 0);   
        _blueVal = ledConfig.getLong("blue", 0);
        _numLeds = ledConfig.getLong("numLeds", 0);

        Log.trace("%sLED Setup from JSON: %s On: %d, Value: %d %d %d %d %d\n", MODULE_PREFIX, 
                    ledStripConfigStr.c_str(), _ledOn, _ledValue, _redVal, _greenVal, _blueVal, _numLeds);
    }

    _isSetup = true;
    // Trigger initial write
    ledConfigChanged = true;
    Log.trace("%sLED Configured: On: %d, Value: %d, red %d green %d blue %d numLed %d\n", MODULE_PREFIX, _ledOn, _ledValue, _redVal, _greenVal, _blueVal, _numLeds);

    leds = new CRGB[_numLeds];

    FastLED.addLeds<WS2811, DATA_PIN, BRG>(leds, _numLeds);
    //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, _numLeds);
}

void LedStrip::updateLedFromConfig(const char * pLedJson) {

    boolean changed = false;
    boolean ledOn = RdJson::getLong("ledOn", 0, pLedJson) == 1;
    if (ledOn != _ledOn) {
        _ledOn = ledOn;
        changed = true;
    }
    byte ledValue = RdJson::getLong("ledValue", 0, pLedJson);
    if (ledValue != _ledValue) {
        _ledValue = ledValue;
        changed = true;
    }
    int redVal= RdJson::getLong("red", 0, pLedJson);
    if (redVal != _redVal) {
        _redVal= redVal;
        changed = true;
    }
    int greenVal= RdJson::getLong("green", 0, pLedJson);
    if (greenVal != _greenVal) {
        _greenVal= greenVal;
        changed = true;
    }

    int blueVal= RdJson::getLong("blue", 0, pLedJson);
    if (blueVal != _blueVal) {
        _blueVal= blueVal;
        changed = true;
    }

    int numLeds= RdJson::getLong("numLeds", 0, pLedJson);
    if (numLeds != _numLeds) {
        _numLeds= numLeds;
        changed = true;
    }

    if (changed)
        updateNv();
}

const char* LedStrip::getConfigStrPtr() {
    return _ledNvValues.getConfigCStrPtr();
}

void 
LedStrip::chase() {
    static int pos=0;
    static int dir=1;
    leds[pos] = leds[_numLeds-pos] = CRGB::Black;
    pos += dir;
    if(pos >= _numLeds/2) {
        dir = -1;
    }
    if(pos <= 0) {
        dir = 1;
    }
    leds[pos] = leds[_numLeds-pos]= CRGB::Blue;

    
}
void 
LedStrip::rainbow()  {
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, _numLeds, gHue, 7);
}
void 
LedStrip::addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    leds[ random16(_numLeds) ] += CRGB::White;
  }
}

void 
LedStrip::rainbowWithGlitter() {
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void 
LedStrip::confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, _numLeds, 10);
  int pos = random16(_numLeds);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void 
LedStrip::sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, _numLeds, 20);
  int pos = beatsin16( 13, 0, _numLeds-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void 
LedStrip::bpm() {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < _numLeds; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void 
LedStrip::juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, _numLeds, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, _numLeds-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}


void 
LedStrip::nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1)%NUM_PATTERNS;
}

String patternName[] = {
    "chase",
    "rainbow",
    "rainbowWithGlitter",
    "confetti", 
    "sinelon", 
    "juggle", 
    "bpm"
};

#define IDLE_TIMEOUT  60*1000UL          //60 seconds

void LedStrip::service(bool robotIdle)
{
    static ulong lastTime;
    static ulong idleTime=0;
    // Check if active
    if (!_isSetup) {
        Log.notice("LED strip not configured\n");
        return;
    }   

    if(idleTime==0) {
        idleTime = lastTime = millis();
        idleTime -= IDLE_TIMEOUT;
    }
#if 0
    static int debugCnt=0;
    if(debugCnt++ > 10000) {
        debugCnt = 0;
        Serial.printf("robot idle %d idle time %d\n", robotIdle, millis()-idleTime);
    }
#endif
    //if robot is idle for more than x seconds, start pattern
    if(robotIdle) 
    {
        if ((millis() - idleTime) > IDLE_TIMEOUT) 
        {

            if((millis() - idleTime) > 2*60*60*1000UL) 
            {
                if(leds[0] != CRGB::Black) 
                {
                    for(int i=0; i < _numLeds; i++) 
                    {
                        leds[i] = CRGB::Black;
                    }
                    FastLED.show(_ledValue); 
         
                    Log.notice("Setting leds to idle\n");
                }
            } else 
            {
                if((millis() - lastTime) > 100) 
                {
                    lastTime = millis();
                    // Call the current pattern function once, updating the 'leds' array
                    (this->*gPatterns[gCurrentPatternNumber])();

                    // send the 'leds' array out to the actual LED strip
                    FastLED.show(_ledValue);  
                    // insert a delay to keep the framerate modest
                    //FastLED.delay(1000/FRAMES_PER_SECOND); 

                    // do some periodic updates
                    EVERY_N_MILLISECONDS( 100 ) { gHue++; } // slowly cycle the "base color" through the rainbow
                    EVERY_N_SECONDS( 60 ) { 
                     nextPattern();
                        Log.notice("Current pattern %d %s\n", gCurrentPatternNumber, patternName[gCurrentPatternNumber].c_str());
                    } // change patterns periodicall
                     
                } 
            
            }
           _ledChanged = true;
        }

    } else {
        //if we need to, reset the led values:
        if(_ledChanged) 
        {
                    //How long have we been idle ?
            Log.notice("reset leds\n");
            updateLed();
        }
        idleTime = millis();
    }
    // If the switch is off or sleeping, turn off the led
    if (!_ledOn || _isSleeping)
    {
        _ledValue = 0x0;
    }

}

void LedStrip::configChanged()
{
    // Reset config
    Log.trace("%sconfigChanged\n", MODULE_PREFIX);
    setup(_pHwConfig, _name.c_str());
}

void LedStrip::updateNv()
{
    String jsonStr;
    jsonStr += "{";
    jsonStr += "\"ledOn\":";
    jsonStr += _ledOn ? "1" : "0";
    jsonStr += ",";
    jsonStr += "\"ledValue\":";
    jsonStr += _ledValue;
    jsonStr += ",";
    jsonStr += "\"red\":";
    jsonStr += _redVal;
    jsonStr += "\"green\":";
    jsonStr += _greenVal;
    jsonStr += "\"blue\":";
    jsonStr += _blueVal;
    jsonStr += "\"numLeds\":";
    jsonStr += _numLeds;
    jsonStr += "}";
    _ledNvValues.setConfigData(jsonStr.c_str());
    _ledNvValues.writeConfig();
    Log.trace("%supdateNv() : wrote %s\n", MODULE_PREFIX, _ledNvValues.getConfigCStrPtr());
    ledConfigChanged = true;
}

// Set sleep mode
void LedStrip::setSleepMode(int sleep)
{
    _isSleeping = sleep;
}
