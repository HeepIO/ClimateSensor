#include <Heep_API.h>
#include "ESP8266_HeepComms.h"
#include "ESP8266_NonVolatileMemory.h"
#include "Arduino_Timer.h" 
heepByte deviceIDByte [STANDARD_ID_SIZE] = {0x00, 0x00, 0x00, 0x00};
uint8_t mac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char clearMemory = 1;
