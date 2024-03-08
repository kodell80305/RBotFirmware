RBotFirmware
============

Firmware for an ESP32 or Particle device to control robots of various geometries
More information here http://robdobson.com/2017/02/a-line-in-the-sand/

# MKS DLC32 port

The original software is based on Rob Dobson's work by way of Matt G. (https://alwaystinkering.com/2020/01/14/diy-kinetic-sand-art-table/).   I built the board designed by Matt G., but I wanted something easier for non-hardware people to build.  It turns out that Makerbase has a line of ESP32 based laser/cnc/3d printer boards that are ideal for this purpose.   I've chosen to use the MKS DLC32, which is available (in early 2024) for $35 including a TS35-R 480x320 screen and three TMS2209 stepper driver controllers.   

Project goals (at least initially)

-Add support for this board while maintaining backward compatibility.   The MKS boards use an I2S I/O expander to provide additional GPIO pins for things like stepper motor direction/enable/step pins.   The ESP32 is a very nice chip in a lot of ways, but it lacks in GPIO.  Using the i2s bus (which was originally designed for digital audio, but has since been adapted for LCD's & Camera functions), allows a major expanions of output capability.   In Marlin/GRBL32, DMA is used and the I2S samples are routed to shift registers to set up the output pins.  

This software has the stepping function managed from a timer interrupt (which sets the length of the stepping pulse).   It's a little more overhead than the DMA, but not a problem for my application (Sysiphus table).

This board has only a single 8 bit shift register that sets a common enable bit (for X, Y, Z stepper drivers), 2 bits of step/direction for X, Y, Z (6 bits total) and a beeper output (1 bit).   We'll set I2S0.conf_single_data in the timer interrupt & let this be the static value until it's changed (I think) - I lack access to a scope to verify this, but it seems to be working.

-Modify the LED support to handle a WS2812B LED strip.   We can control the LED from the TTL output (IO32) found on J18 (for a 12 volt LED strip).   

-Add screen support using TFT_eSPI/lvgl for the TS35-R screen 

-Change original software as little as possible & maintain backward compatibility


