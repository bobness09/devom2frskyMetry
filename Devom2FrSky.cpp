/*
	Author bobness09, Mail bobnessdev@gmail.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Devom2FrSky.h"

#include <SoftwareSerial.h>
#include <FlexiTimer2.h>
#include "SimpleTelemetry.h"
#include "WalkeraTelemetry.h"
#include "FrSky.h"

#define HEARTBEATLED 13
#define HEARTBEATFREQ 500

// Do not enable both at the same time
//#define DEBUG
//#define DEBUGFRSKY

// Comment this to run simple telemetry protocol
#define WALKERATELEMETRY

#ifdef WALKERATELEMETRY
WalkeraTelemetry *dataProvider;
#else
SimpleTelemetry *dataProvider;
#endif

//FastSerialPort0(Serial);
FrSky *frSky;
SoftwareSerial *frSkySerial;
SoftwareSerial *WalkeraSerial;

#ifdef DEBUG
HardwareSerial *debugSerial;
#elif defined DEBUGFRSKY
SoftwareSerial *frskyDebugSerial;
#endif

int		counter = 0;
unsigned long	hbMillis = 0;
unsigned long	rateRequestTimer = 0;
byte	hbState;
bool	firstParse = false;

void setup() {

	// Open serial communications and wait for port to open:
	Serial.begin(57600);
	while (!Serial) {
		; // wait for serial port to connect. Needed for Leonardo only
	}

	Serial.println("Starting Devo-M 2 FrSky Telemetry 1.0.1!");

// Debug serial port pin 12 tx 11 rx
#ifdef DEBUG
	//debugSerial = new SoftwareSerial(12, 11);
	debugSerial = &Serial;
	//debugSerial->begin(38400);
#elif defined DEBUGFRSKY
	frskyDebugSerial = new SoftwareSerial(12, 11);
	frskyDebugSerial->begin(38400);
#endif

    WalkeraSerial = new SoftwareSerial(5, 6); // 5 RX 6 TX (no need)
	WalkeraSerial->begin(38400);

	// FrSky data port pin 2 rx 3 tx
	frSkySerial = new SoftwareSerial(2, 3, true);
	frSkySerial->begin(9600);

	WalkeraSerial->listen(); //

#ifdef DEBUG
	debugSerial->println("Initializing...");
	debugSerial->print("Free ram: ");
	debugSerial->print(freeRam());
	debugSerial->println(" bytes");
#endif
#ifdef WALKERATELEMETRY
	dataProvider = new WalkeraTelemetry();
#else
	dataProvider = new SimpleTelemetry();
#endif


	frSky = new FrSky();

	digitalWrite(HEARTBEATLED, HIGH);
	hbState = HIGH;

	FlexiTimer2::set(200, 1.0/1000, sendFrSkyData); // call every 200 1ms "ticks"
	FlexiTimer2::start();

#ifdef DEBUG
	debugSerial->println("Initialization done.");
	debugSerial->print("Free ram: ");
	debugSerial->print(freeRam());
	debugSerial->println(" bytes");
#endif

}

void loop() {

	dataProvider->resetParse();

	while (!WalkeraSerial->available()); // Wait until Data comes in
	while (WalkeraSerial->available() > 0)
	{
		byte c = WalkeraSerial->read();
		dataProvider->parseMessage(c);
	}

	dataProvider->finishParse();

	processData();
	updateHeartbeat();
}

void updateHeartbeat()
{
	long currentMilillis = millis();
	if(currentMilillis - hbMillis > HEARTBEATFREQ) {
		hbMillis = currentMilillis;
		if (hbState == LOW)
		{
			hbState = HIGH;
		}
		else
		{
			hbState = LOW;
		}
		digitalWrite(HEARTBEATLED, hbState);
	}
}

void sendFrSkyData()
{
	counter++;

	if (counter >= 25)			 // Send 5000 ms frame
	{
		frSky->sendFrSky05Hz(frSkySerial, dataProvider);
		counter = 0;
	}
	else if ((counter % 5) == 0) // Send 1000 ms frame
	{
		frSky->sendFrSky1Hz(frSkySerial, dataProvider);
#ifdef DEBUG
		debugSerial->println("Begin frSky->printValues");
		frSky->printValues(debugSerial, dataProvider);
#endif
	}
	else						 // Send 200 ms frame
	{
		frSky->sendFrSky5Hz(frSkySerial, dataProvider);
	}
}



void processData()
{
}

int freeRam () {
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
