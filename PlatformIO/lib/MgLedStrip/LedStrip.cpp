// Auto Dimming LED Strip
// Matt Grammes 2018
// Mod KMO - since this isn't working anyway, repurpose for WS2812B led strip
#include "LedStrip.h"
#include "Arduino.h"
#include "ConfigNVS.h"


#define NUM_LEDS 178

//Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_WS2812B_PIN, NEO_GRB);
/// @file    DemoReel100.ino
/// @brief   FastLED "100 lines of code" demo reel, showing off some effects
/// @example DemoReel100.ino

#include <FastLED.h>

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
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120


static const char* MODULE_PREFIX = "LedStrip: ";

LedStrip::LedStrip(ConfigBase &ledNvValues) : _ledNvValues(ledNvValues)
{
    _isSetup = false;
    _isSleeping = false;
    _ledPin = -1;
    _sensorPin = -1;
    
    _redVal=0;
    _blueVal=255;
    _greenVal=0;
    _numLeds = 178;
}

void
LedStrip::updateLed() {
    

}

void 
LedStrip::setRGB(uint8_t brightness, uint8_t redVal, uint8_t greenVal, uint8_t blueVal) {
    _ledValue = brightness;
    _redVal = redVal;
    _greenVal = greenVal;
    _blueVal = blueVal;

}

void 
LedStrip::getRGB(uint8_t &brightness, uint8_t &redVal, uint8_t &greenVal, uint8_t &blueVal) {
    brightness = _ledValue;
    redVal = _redVal;
    greenVal = _greenVal;
    blueVal = _blueVal;

}

void LedStrip::setup(ConfigBase* pConfig, const char* ledStripName)
{

    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

    _name = ledStripName;
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

    // Ambient Light Sensor Pin
    String sensorStr = ledConfig.getString("sensorPin", "");
    int sensorPin = -1;
    if (sensorStr.length() != 0)
        sensorPin = ConfigPinMap::getPinFromName(sensorStr.c_str());

    Log.notice("%sLED pin %d Sensor pin %d\n", MODULE_PREFIX, ledPin, sensorPin);
    // Sensor pin isn't necessary for operation.
    if (ledPin == -1)
        return;
#if 1
    // Setup led pin
    if (_isSetup && (ledPin != _ledPin))
    {
        ledcDetachPin(_ledPin);
    }
    else
    {
        _ledPin = ledPin;
        ledcSetup(LED_STRIP_LEDC_CHANNEL, LED_STRIP_PWM_FREQ, LED_STRIP_LEDC_RESOLUTION);
        ledcAttachPin(_ledPin, LED_STRIP_LEDC_CHANNEL);
    }
#endif
    // Setup the sensor
    _sensorPin = sensorPin;
    if (_sensorPin != -1) {
        pinMode(_sensorPin, INPUT);
        for (int i = 0; i < NUM_SENSOR_VALUES; i++) {
            sensorValues[i] = 0;
        }
    }

    // If there is no LED data stored, set to default
    String ledStripConfigStr = _ledNvValues.getConfigString();
    if (ledStripConfigStr.length() == 0 || ledStripConfigStr.equals("{}")) {
        Log.trace("%sNo LED Data Found in NV Storge, Defaulting\n", MODULE_PREFIX);
        // Default to LED On, Half Brightness
        _ledOn = true;
        _ledValue = 0x7f;
        _autoDim = false;
        updateNv();
    } else {
        _ledOn = _ledNvValues.getLong("ledOn", 0) == 1;
        _ledValue = _ledNvValues.getLong("ledValue", 0xFF);
        _autoDim = _ledNvValues.getLong("autoDim", 0) == 1;
        Log.trace("%sLED Setup from JSON: %s On: %d, Value: %d, Auto Dim: %d\n", MODULE_PREFIX, 
                    ledStripConfigStr.c_str(), _ledOn, _ledValue, _autoDim);
    }

    _isSetup = true;
    // Trigger initial write
    ledConfigChanged = true;
    Log.trace("%sLED Configured: On: %d, Value: %d, AutoDim: %d red %d green %d blue %d numLed %d\n", MODULE_PREFIX, _ledOn, _ledValue, _autoDim, _redVal, _greenVal, _blueVal, _numLeds);
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
    boolean autoDim = RdJson::getLong("autoDim", 0, pLedJson) == 1;
    if (autoDim != _autoDim) {
        // TODO Never Enable Auto Dim
        //_autoDim = autoDim;
        _autoDim = false;
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



#if 0
void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}
#endif


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))



void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}
void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };
void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

String patternName[] = {
    "rainbow",
    "rainbowWithGlitter",
    "confetti", 
    "sinelon", 
    "juggle", 
    "bpm"
};

void LedStrip::service(bool robotIdle)
{
    // Check if active
    if (!_isSetup)
        return;

    static ulong lastTime;
    static ulong idleTime=0;
    static bool bPatternActive = true;

    if(idleTime==0) {
        idleTime = lastTime = millis();
    }

    //if robot is idle for more than x seconds, start pattern:

    if(robotIdle) {
        if((millis() - idleTime) > 30*1000UL) {
            if((millis() - idleTime) > 120*1000UL) {
                if(leds[0] != CRGB::Black) {
                    for(int i=0; i < NUM_LEDS; i++) {
                        leds[i] = CRGB::Black;
                    }
                    FastLED.show(); 
                    Log.notice("Setting leds to idle\n");
                }
            } else {

                if((millis() - lastTime) > 100) {
                    lastTime = millis();
                    // Call the current pattern function once, updating the 'leds' array
                    gPatterns[gCurrentPatternNumber]();

                    // send the 'leds' array out to the actual LED strip
                    FastLED.show();  
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
            bPatternActive = true;
            
        }
    } else {
        //if we need to, reset the led values:
        if(bPatternActive) {
                    //How long have we been idle ?
            Log.notice("robot idle cleared\n");
            for(int i=0; i < NUM_LEDS; i++) {
                leds[i].setRGB(_redVal, _blueVal, _greenVal);
            }
            FastLED.show();  
            bPatternActive = false;
            idleTime = millis();
        }
    }

    // If the switch is off or sleeping, turn off the led
    if (!_ledOn || _isSleeping)
    {
        _ledValue = 0x0;
    }
    else
    {
        // TODO Auto Dim isn't working as expected - this should never go enabled right now
        // Check if we need to read and evaluate the light sensor
        if (_autoDim)
        {
            if (_sensorPin != -1) {
                sensorValues[sensorReadingCount++ % NUM_SENSOR_VALUES] = analogRead(_sensorPin);
                uint16_t sensorAvg = LedStrip::getAverageSensorReading();
                // if (count % 100 == 0) {
                // Log.trace("Ambient Light Avg Value: %d, reading count %d\n", sensorAvg, sensorReadingCount % NUM_SENSOR_VALUES);
                
                // Convert ambient light (int) to led value (byte)
                int ledBrightnessInt = sensorAvg / 4;

                // This case shouldn't be hit
                if (ledBrightnessInt > 255) {
                    ledBrightnessInt = 255;
                    Log.error("%sAverage Sensor Value over max!\n", MODULE_PREFIX);
                }
                byte ledValue = ledBrightnessInt;
                if (_ledValue != ledValue) {
                    _ledValue = ledValue;
                    updateNv();
                }
            }
        }
    }

    if (ledConfigChanged) {
        ledConfigChanged = false;
        Log.trace("Writing LED Value: 0x%x\n", _ledValue);
        ledcWrite(LED_STRIP_LEDC_CHANNEL, _ledValue);
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
    jsonStr += "\"autoDim\":";
    jsonStr += _autoDim ? "1" : "0";
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

// Get the average sensor reading
uint16_t LedStrip::getAverageSensorReading() {
    uint16_t sum = 0;
    for (int i = 0; i < NUM_SENSOR_VALUES; i++) {
        sum += sensorValues[i];
    }
    return sum / NUM_SENSOR_VALUES;
}

// Set sleep mode
void LedStrip::setSleepMode(int sleep)
{
    _isSleeping = sleep;
}
