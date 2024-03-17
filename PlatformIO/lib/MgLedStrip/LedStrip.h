// Auto Dimming LED Strip
// Matt Grammes 2018

#pragma once

#include "Utils.h"
#include "ConfigNVS.h"
#include "ConfigPinMap.h"


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

private:
    void configChanged();
    void updateNv();
    uint16_t getAverageSensorReading();

private:
    String _name;
    ConfigBase* _pHwConfig;
    static const int NUM_SENSOR_VALUES = 100;
    bool _isSetup;
    bool _isSleeping;
    int _ledPin;
    int _sensorPin;

    bool _ledOn;

    bool _autoDim = false;
    bool ledConfigChanged = false;
    
    int sensorReadingCount = 0;

    // Store the settings for LED in NV Storage
    ConfigBase& _ledNvValues;

    // Store a number of sensor readings for smoother transitions
    uint16_t sensorValues[NUM_SENSOR_VALUES];


    uint8_t _ledValue = -1;
    uint8_t _redVal;
    uint8_t _blueVal;
    uint8_t _greenVal;
    uint8_t _numLeds;

    // LEDC library controls
    static const int LED_STRIP_PWM_FREQ = 7000;
    static const int LED_STRIP_LEDC_CHANNEL = 0;
    static const int LED_STRIP_LEDC_RESOLUTION = 8;

    ulong idleTime;

    //Adafruit_NeoPixel strip;
};