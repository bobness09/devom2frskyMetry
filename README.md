# devom2frskyMetry

Author bobness09, Mail bobnessdev@gmail.com
Contact me, if you have any questions!

This project is a telemetry converter for using the DEVO-M (Walkera X350 Pro) with a FrSky D-Series receiver.
It aims to be as non-intrusive as possible, that's why it uses the white telemetry cable which is normally plugged into the RX705, and leads to the GPS module.
This means the fine soldering on the DEVO-M board isn't necessary anymore, the Arduino virtually only needs to be plugged in
(a bit soldering on the FrSky telemetry cable and the cable to the GPS module is required).

devom2frskyMetry is based on the following project. Thanks a lot, Nils Högberg!
https://github.com/vizual54/APM-Mavlink-to-FrSky

Things needed:
--------------
* Devo-M + D4R-II / D8R-XP & Taranis (obviously)

* Arduino Mini Pro (Clone) 328, 16MHz, 5V
* A few jumper cables
* Hex file and TTL to USB adapter to flash it (can be found in the Doc folder). Here's how to flash the file: https://github.com/grbl/grbl/wiki/Flashing-Grbl-to-an-Arduino

Connecting the 4 cable to the X350 Pro (see wiring diagram picture in Doc folder):
--------------------------------------------------------------------
D4R-II / D8R-XP: telemetry Rx (green) to Arduino pin 3
X350 Pro Telemetry cable (white) (Tali cable set probably not necessary): Arduino pin 5
GND and 5V (eg. AUX3 of DEVO-M) to GND and VCC of the Arduino (be careful to connect them correctly!)

Solder the wires directly to the Arduino (not the flashing cables on the side, there you better use the pins), and put the Arduino in heat shrink tube to avoid shortcuts.

The best way is to lead the 4 cable for the Arduino out of the housing. 
Alternatively, lead out the jumper cables for flashing the Arduino.
This way, you can flash it easier when a new version comes out, using the TTL adapter, without the need to open the housing.


Setup / Values:
---------------

If the Arduino receives data from the DEVO-M, its green LED flashes.

The following telemetry values ​​are currently delivered to the Taranis (see Taranis screenshots in Doc folder):

* GPS coordinates in NMEA/DMS format (chose between one of the two in your Taranis settings)
* Vfas = Lipo voltage of the X350 Pro (rounded to the nearest 1/10)
* A1 = Voltage FrSky receiver (always around 5V)
* Alt = Relative height to start in M, accurate value. Presumably calculated as a combination of GPS and barometer of Devo-M. Can also have negative values ​​(eg flight into the valley)
* Spd = Copter speed in m/s

* Dist = Distance in m from the starting point calculated by the Taranis through the GPS coordinates
* Temp1 = Raw value current speed cm/s
* Temp2 = Last unknown value field, values 0-255

* VSpd = Calculated speed from GPS coordinates
* GAlt = GPS altitude (currently no correct value, seems to be somehow calculated by the taranis)
