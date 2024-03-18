// Auto Dimming LED Strip
// Matt Grammes 2018

#pragma once

#include "Utils.h"
#include "ConfigNVS.h"
#include "ConfigPinMap.h"
#include <FastLED.h>

// List of patterns to cycle through.  Each is defined as a separate function below.
class LedStrip;

typedef void (LedStrip::*SimplePatterns)();
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#define NUM_PATTERNS 6

class LedStrip
{
public:
    LedStrip(ConfigBase &ledNvValues);
    void setup(ConfigBase* pConfig, const char* ledStripName);
    void service(bool robotIdle);
    void updateLedFromConfig(const char* pLedJson);
    void setRGB(uint8_t brightness, uint8_t redVal, uint8_t greenVal, uint8_t blueVal);
    void getRGB(uint8_t &brightness, uint8_t &redVal, uint8_t &greenVal, uint8_t &blueVal);
    void updateLed();

    const char* getConfigStrPtr();
    void setSleepMode(int sleep);
    void rainbow();
    void addGlitter( fract8 chanceOfGlitter);
    void rainbowWithGlitter();
    void confetti();
    void sinelon();
    void bpm();
    void nextPattern();
    void juggle();

private:
    void configChanged();
    void updateNv();
    uint16_t getAverageSensorReading();

private:
    String _name;
    ConfigBase* _pHwConfig;
    bool _isSetup;
    bool _isSleeping;
    int _ledPin;
    bool _ledOn;

    bool ledConfigChanged = false;


    // Store the settings for LED in NV Storage
    ConfigBase& _ledNvValues;


    uint8_t _ledValue = -1;
    uint8_t _redVal;
    uint8_t _blueVal;
    uint8_t _greenVal;
    uint8_t _numLeds;
    volatile bool _ledChanged;

    ulong idleTime;

    CRGB *leds;


    uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
    uint8_t gHue = 0; // rotating "base color" used by many of the patterns

     SimplePatterns gPatterns[NUM_PATTERNS] ={&LedStrip::rainbow, &LedStrip::rainbowWithGlitter, &LedStrip::confetti, &LedStrip::sinelon, &LedStrip::juggle, &LedStrip::bpm };


    //Adafruit_NeoPixel strip;
};