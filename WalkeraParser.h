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

#ifndef WALKERAPARSER_H_
#define WALKERAPARSER_H_

#include <Arduino.h>
#include "defines.h"

/*
 * DEVO-M Dataframe
 * ----------------------------
 *
 * Length                : 20 Bytes
 * Syncbyte              : 0xAA
 * Payload with Checksum : 19 Bytes
 * Checksum              : crc8 accumulate uint8_t
 *
 * 0xAA : byte 01 : <sync header>
 * 0xD7 : byte 02 : <gps longitude int32_t lowest byte>
 * 0x60 : byte 03 : <gps longitude int32_t>
 * 0xF5 : byte 04 : <gps longitude int32_t>
 * 0x1E : byte 05 : <gps longitude int32_t highest byte>
 * 0x0A : byte 06 : <gps lattitude int32_t lowest byte>
 * 0x96 : byte 07 : <gps lattitude int32_t>
 * 0x89 : byte 08 : <gps lattitude int32_t>
 * 0x04 : byte 09 : <gps lattitude int32_t highest byte>
 * 0x76 : byte 10 : <gps altitude int32_t lowest byte>
 * 0x00 : byte 11 : <gps altitude int32_t>
 * 0x00 : byte 12 : <gps altitude int32_t>
 * 0x00 : byte 13 : <gps altitude int32_t highest byte>
 * 0x00 : byte 14 : <gps ground speed uint16_t lowest byte>
 * 0x00 : byte 15 : <gps ground speed uint16_t highest byte>
 * 0x00 : byte 16 : <unknown value uint16_t lowest byte>
 * 0x00 : byte 17 : <unknown value uint16_t highest byte>
 * 0x52 : byte 18 : <voltage in millivolts uint16_t lower byte>
 * 0x30 : byte 19 : <voltage in millivolts uint16_t upper byte>
 * 0x19 : byte 20 : <crc8 checksum over byte 1-19>
 *
 */

#define DEVOM_DATA_LENGTH	18
#define DEVOM_SYNC_BYTE		0xAA

enum DEVOM_DECODE_STATE {
	DEVOM_DECODE_STATE_UNSYNCED = 0,
	DEVOM_DECODE_STATE_GOT_HEADER,
	DEVOM_DECODE_STATE_GOT_DATA
};

#pragma pack(push, 1)
typedef struct {
	uint8_t	header;							///< 0xAA for a valid packet
	uint8_t	data[18];						///< ChannelData
	uint8_t	crc8;							///< CRC8
} DevoMPacket;
#pragma pack(pop)

class WalkeraParser {
private:
    float gpsLatitude;
    float gpsLongitude;
    int32_t gpsAltitude; // Altitude in CM.

    uint16_t gpsGroundSpeed; // Probably speed m/s
    uint16_t unknownVal1;
    uint16_t batteryVoltage;

	uint8_t _crc8;
	uint8_t _rxlen;
	DevoMPacket _rxpacket;
	DEVOM_DECODE_STATE _decode_state;

	/**
	 * CRC8 implementation
	 *
	 * @param crc Initial CRC Value
	 * @param value to accumulate in the checksum
	 * @return the checksum
	 */
	uint8_t devom_crc8(uint8_t crc, uint8_t value);

public:
	WalkeraParser();
	virtual ~WalkeraParser();

	/**
	 * Decoder for DEVO-M protocol
	 *
	 * @param byte current char to read
	 * @return 0 for success (a decoded packet), 1 for no packet yet (accumulating), 3 for out of sync, 3 for checksum error
	 */
	int parseByte(uint8_t byte);

	float getGpsLatitude() const {
		return gpsLatitude;
	}

	float getGpsLongitude() const {
		return gpsLongitude;
	}

	int32_t getGpsAltitude() const {
		return gpsAltitude;
	}

	uint16_t getBatteryVoltage() const {
		return batteryVoltage;
	}

	uint16_t getGpsGroundSpeed() const {
		return gpsGroundSpeed;
	}

	uint16_t getUnknownVal1() const {
		return unknownVal1;
	}
};

#endif /* WALKERAPARSER_H_ */
