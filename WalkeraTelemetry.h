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

#ifndef WalkeraTelemetry_h
#define WalkeraTelemetry_h

#define MAXTERMS 21
#define MAXSENTENCE 110
#define MAXWORD 10

#include <SoftwareSerial.h>
#include "ifrskydataprovider.h"
#include "WalkeraParser.h"
#include <Arduino.h>
#include "defines.h"

class WalkeraTelemetry :	public IFrSkyDataProvider
{
public:
	WalkeraTelemetry(void);
	virtual ~WalkeraTelemetry(void);
	void        resetParse();
	bool		parseMessage(byte c);
	void        finishParse();

	// IFrSkyDataProvider functions
	void calculateTime();
    const float	getGpsAltitude();
	const int	getTemp1();
	const int	getEngineSpeed();
	const int	getBatteryVoltage();
	const int	getTemp2();
	const float	getAltitude();
	const float	getGpsGroundSpeed();
	const float	getLongitude();
	const float	getLatitude();
	const float	getCourse();
	const int   getYear();
	const int   getDate();
	const int	getHour();
	const int	getMinute();
	const int	getSecond();
	const float	getAccX();
	const float	getAccY();
	const float	getAccZ(); 
	const float	getBatteryCurrent();
	const float	getBatteryVoltageFloat();

private:
	int _dehex(char a);
	float gpsDdToDdMmSsFormat(float ddm);
	float gpsDdToDdMmMmmFormat(float ddm);

	SoftwareSerial* debugPort; 
	WalkeraParser wParser;

	int h, m, s;

	byte inData[100]; // Allocate some space for the string
	byte index; // Index into array; where to store the character
};

#endif
