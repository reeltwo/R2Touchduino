# R2 Touchduino #

R2 Touchduino is a re-implementation of the [R2 Touch](https://www.curiousmarc.com/r2-d2/r2-touch-app) iPhone application using an Arduino Uno and [Gameduino 3X](https://excamera.com/sphinx/store.html#id7) LCD touchscreen.

Optionally you can add a Sparkfun QRE1113 (analog) sensor to detect if a card was inserted into the radar eye slot. If the card is detected it will send Marcduino commands to open panel 5, play music, and start playing the death star plans movie. This setup assumes you have mounted the Gameduino display behind panel 5 in the dome.

Default baud rate is 2400 to be compatible with JawaLite devices. You may want to change the it 9600.

To bring out the serial UART to the rest of your system the simplest method is to solder wires to PIN 0 and 1 on the back of your Arduino Uno (RX and TX). You will also need to solder a wire to GND so that the UART may share common ground.

To add support for the QRE1113 sensor you need to solder a wires to A3, 5V, and GND.

Optionally you can bring out i2c by soldering wires to A4 and A5.

You need to copy leia.avi, plans1.avi, plans2.avi, r2.gd2, and Splash.jpg from the Assets directory into the root directory of the SD card. The SD card cannot be bigger than 16GB (I'm using 8GB cards).