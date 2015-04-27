#include "WalkeraParser.h"

//#define DEBUGFAKEVALS

WalkeraParser::WalkeraParser()
{
    gpsLatitude    = 0;
    gpsLongitude   = 0;
    gpsAltitude    = 0;
    unknownVal1    = 0;
    gpsGroundSpeed = 0;
	batteryVoltage = 0;

	_crc8 = 0x00;
	_rxlen = 0;
	_decode_state = DEVOM_DECODE_STATE_UNSYNCED;
}

WalkeraParser::~WalkeraParser() {
}

uint8_t WalkeraParser::devom_crc8(uint8_t crc, uint8_t value)
{
	crc += value;
	return crc;
}

int WalkeraParser::parseByte(uint8_t byte)
{
	int ret = 1;
	switch (_decode_state) {
	case DEVOM_DECODE_STATE_UNSYNCED:
		if (byte == DEVOM_SYNC_BYTE) {
			_rxpacket.header = byte;
			_rxlen = 0;
			_crc8 = 0x00;
			_crc8 = devom_crc8(_crc8, byte);
			_decode_state = DEVOM_DECODE_STATE_GOT_HEADER;
			//printf( "Header   byte: %x \n", byte) ;
		} else {
			ret = 2;
		}

		break;


	case DEVOM_DECODE_STATE_GOT_HEADER:
		_rxpacket.data[_rxlen] = byte;
		_crc8 = devom_crc8(_crc8, byte);
		_rxlen++;

		//printf( "cnt: %d   byte: %x \n", _rxlen, byte) ;

		if (_rxlen == DEVOM_DATA_LENGTH) {
			_decode_state = DEVOM_DECODE_STATE_GOT_DATA;
		}

		break;


	case DEVOM_DECODE_STATE_GOT_DATA:
		_rxpacket.crc8 = byte;

		if (_crc8 == _rxpacket.crc8) {
			// valid devo-m packet
			// handle value and call output usart functions

			int32_t lat = ((int32_t)_rxpacket.data[3] << 24) + ((int32_t)_rxpacket.data[2] << 16) + ((int32_t)_rxpacket.data[1] << 8) + ((int32_t)_rxpacket.data[0]);
		    int32_t lon = ((int32_t)_rxpacket.data[7] << 24) + ((int32_t)_rxpacket.data[6] << 16) + ((int32_t)_rxpacket.data[5] << 8) + ((int32_t)_rxpacket.data[4]);

			gpsLatitude    = lat / 10000000.0f;
			gpsLongitude   = lon / 10000000.0f;

			if ((gpsLatitude < -180.0f) || (gpsLatitude > 180.0f))
				gpsLatitude = 0.0f;

			if ((gpsLongitude < -90.0f) || (gpsLongitude > 90.0f))
				gpsLongitude = 0.0f;

			gpsAltitude    = ((int32_t)_rxpacket.data[11] << 24) + ((int32_t)_rxpacket.data[10] << 16) + ((int32_t)_rxpacket.data[9] << 8) + ((int32_t)_rxpacket.data[8]);

			gpsGroundSpeed = ((uint16_t)_rxpacket.data[13] << 8) + (uint16_t)_rxpacket.data[12];
			unknownVal1    = ((uint16_t)_rxpacket.data[15] << 8) + (uint16_t)_rxpacket.data[14];

			batteryVoltage = ((uint16_t)_rxpacket.data[17] << 8) + (uint16_t)_rxpacket.data[16];

#ifdef DEBUGFAKEVALS
			gpsLatitude    = 51.49257f; // = 51°29.3325 N
    		gpsLongitude   = 7.45174f;  // =  7°27.0626 E

			gpsAltitude    = 20000; // 200m
			gpsGroundSpeed = 1230; // 12,3m/s // Speed only works, when coordinates are set!

			unknownVal1    = 0;
			batteryVoltage = 12600; // 12.6V
#endif

			/* decoding success */
			ret = 0;
		} else {
			/* decoding failed */
			//printf( "CRC failed\n") ;
			ret = 3;
		}

		_decode_state = DEVOM_DECODE_STATE_UNSYNCED;
		break;
	}

	return ret;
}
