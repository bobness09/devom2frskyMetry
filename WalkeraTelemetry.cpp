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

#include "WalkeraTelemetry.h"

#define WALKERA_DATA_PACKET   0xAA
#define WALKERA_GPS_PACKET    0x3B

WalkeraTelemetry::WalkeraTelemetry(void)
{
    resetParse();

    h = 0;
	m = 0;
	s = 0;
}

WalkeraTelemetry::~WalkeraTelemetry(void)
{
}

void WalkeraTelemetry::resetParse()
{
    index = 0;
}

bool WalkeraTelemetry::parseMessage(byte c)
{
	if (index < 99) // One less than the size of the array
	{
	  inData[index] = c; // Store it
	  index++; // Increment where to write next
	}

	// Walkera packet parser
	if (wParser.parseByte(c) == 0) {
		 Serial.print("WalkeraTelemetry parse success! Voltage: ");
		 Serial.print(getBatteryVoltage());
		 Serial.print(" Long: ");
		 Serial.print(getLongitude());
		 Serial.print(" Lat: ");
		 Serial.print(getLatitude());
		 Serial.print(" GpsAlt: ");
		 Serial.print(getGpsAltitude());
		 Serial.print(" Speed: ");
		 Serial.print(getGpsGroundSpeed());

		 calculateTime();

		 Serial.print(" Elapsed time: ");
	     Serial.print(h);
	     Serial.print("h ");
	     Serial.print(m);
	     Serial.print("m ");
	     Serial.print(s);
	     Serial.print("s ");

		 Serial.println();
	}

    return 0;
}

void WalkeraTelemetry::finishParse()
{
  for (int i = 0; i < index; i++) {
	Serial.print(inData[i], HEX);
	Serial.print(" ");
  }

  Serial.println();
}

const float WalkeraTelemetry::getBatteryVoltageFloat()
{
	return (wParser.getBatteryVoltage() / 100.0f);
}

const float WalkeraTelemetry::getBatteryCurrent()
{
	return 0;
}

const int WalkeraTelemetry::getBatteryVoltage()
{
	return round(wParser.getBatteryVoltage() / 100.0f);
}

const float WalkeraTelemetry::getLatitude()
{
	return gpsDdToDmsFormat(wParser.getGpsLatitude());
}

const float WalkeraTelemetry::getLongitude()
{
	return gpsDdToDmsFormat(wParser.getGpsLongitude());
}

const float WalkeraTelemetry::getGpsAltitude()
{
	return wParser.getGpsAltitude() / 100.0f;
}

const int WalkeraTelemetry::getTemp1()
{
	return wParser.getGpsGroundSpeed();
}

const int WalkeraTelemetry::getTemp2()
{
	//return wParser.getBatteryVoltage();
	return wParser.getUnknownVal1();
}

const float WalkeraTelemetry::getGpsGroundSpeed()
{
	return wParser.getGpsGroundSpeed() / 100.0f; // cm/s -> Better use m/s!
}

const float WalkeraTelemetry::getAltitude()
{
	return wParser.getGpsAltitude() / 100.0f;
}

const float WalkeraTelemetry::getCourse()
{
	return 0;
}

const int WalkeraTelemetry::getEngineSpeed()
{
	return 0;
}

const float WalkeraTelemetry::getAccX()
{
	return 0;
}
	
const float WalkeraTelemetry::getAccY()
{
	return 0;
}

const float WalkeraTelemetry::getAccZ()
{
	return 0;
}

const int WalkeraTelemetry::getYear()
{
	return 0;
}

void WalkeraTelemetry::calculateTime()
{
	unsigned long over, elapsed;
	elapsed = millis();
	h = int(elapsed / 3600000);
	over = elapsed % 3600000;
	m = int(over / 60000);
	over = over % 60000;
	s = int(over / 1000);
}

const int WalkeraTelemetry::getSecond()
{
	return s;
}

const int WalkeraTelemetry::getMinute()
{
	return m;
}

const int WalkeraTelemetry::getHour()
{
	return h;
}

const int WalkeraTelemetry::getDate()
{
	return 0;
}

// We receive the GPS coordinates in ddd.dddd format
// FrSky wants the dd mm.mmm format so convert.
float WalkeraTelemetry::gpsDdToDmsFormat(float ddm)
{
	int deg = (int)ddm;
	float min_dec = (ddm - deg) * 60.0f;
	float sec = (min_dec - (int)min_dec) * 60.0f;

	return (float)deg * 100.0f + (int)min_dec + sec / 100.0f;
}

int WalkeraTelemetry::_dehex(char a) {
	// returns base-16 value of chars '0'-'9' and 'A'-'F';
	// does not trap invalid chars!
  if (int(a) >= 65) {
    return int(a)-87;
  }
  else {
    return int(a)-48;
  }
}
