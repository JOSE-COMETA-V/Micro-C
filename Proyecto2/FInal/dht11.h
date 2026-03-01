#ifndef DHT11_H
#define DHT11_H

#include <xc.h>
#include <stdint.h>
#include "Config.h"

#define DHT11_PIN PORTBbits.RB2
#define DHT11_TRIS TRISBbits.TRISB2
#define DHT11_LAT LATBbits.LATB2

extern uint8_t humidity_int, humidity_dec;
extern uint8_t temp_int, temp_dec;
extern uint8_t checksum;

void DHT11_Start(void);
uint8_t DHT11_CheckResponse(void);
uint8_t DHT11_ReadData(void);
uint8_t DHT11_ReadValues(void);

#endif