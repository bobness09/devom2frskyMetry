#ifndef _Devom2FrSky_H_
#define _Devom2FrSky_H_
#include "Arduino.h"

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();

void processData();
void updateHeartbeat();
void sendFrSkyData();
int freeRam();

#ifdef __cplusplus
} // extern "C"
#endif

//Do not add code below this line
#endif /* _Devom2FrSky_H_ */
