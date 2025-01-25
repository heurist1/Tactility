This is a first attempt to add the CYD 2432S028R device.

This board is a little odd in that it has 3 SPI devices (display, touch, SD Card) each are on seperate pins.
The ESP32 processor has 4 SPI Masters, but two are used internally to control the cache, leaving only 2 to attach to the 3 devices.
Obviously the ESP32 can control many devices on an SPI bus , but in this case they are all on separaet busses.

V1 is a proof of concept with the display and touch connected up, and the device boots and runs, the menues can be navigated, but obvioudly there is no SD card available.
