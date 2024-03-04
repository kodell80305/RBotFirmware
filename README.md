RBotFirmware
============

Firmware for an ESP32 or Particle device to control robots of various geometries
More information here http://robdobson.com/2017/02/a-line-in-the-sand/

# MKS-DLC32-PORT

The original software is based on Rob Dobson's work by way of Matt G. (https://alwaystinkering.com/2020/01/14/diy-kinetic-sand-art-table/).   I built the board designed by Matt G., but I wanted something easier for non-hardware people to build.  It turns out that Makerbase has a line of ESP32 based laser/cnc/3d printer boards that are ideal for this purpose.   I've chosen to use the MKS DLC32, which is available (in early 2024) for $35 including a TS35-R 480x320 screen and hree TMS2209 stepper driver controllers.   

Project goals (at least initially)

-Add support for this board while maintaining backword compatibility

-Add screen support using TFT_eSPI/lvgl

-Modify the LED to support addressable LED's such as WS2811/WS2812.  


