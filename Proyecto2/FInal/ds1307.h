#ifndef DS1307_H
#define DS1307_H

#include <xc.h>
#include <stdint.h>

// Dirección I2C del DS1307 (7 bits: 0x68)
#define DS1307_ADDR_W  0xD0   // 8-bit write address
#define DS1307_ADDR_R  0xD1   // 8-bit read address

// --- Prototipos ---
void DS1307_Init(void);

void DS1307_SetTime(uint8_t hour, uint8_t min, uint8_t sec);
void DS1307_SetDate(uint8_t day, uint8_t month, uint8_t year);

void DS1307_GetTime(uint8_t *hour, uint8_t *min, uint8_t *sec);
void DS1307_GetDate(uint8_t *day, uint8_t *month, uint8_t *year);

// Conversión entre BCD y decimal
uint8_t decToBcd(uint8_t val);
uint8_t bcdToDec(uint8_t val);

#endif