#include "ds1307.h"
#include "i2c.h"
#include "Config.h"

uint8_t decToBcd(uint8_t val) {
    return ((val / 10 * 16) + (val % 10));
}

uint8_t bcdToDec(uint8_t val) {
    return ((val / 16 * 10) + (val % 16));
}

void DS1307_Init(void) {
    // Inicia el reloj (pone bit CH=0 en segundos)
    I2C_Master_Start();
    I2C_Master_Write(DS1307_ADDR_W);
    I2C_Master_Write(0x00);      
    I2C_Master_Write(0x00);      
    I2C_Master_Stop();
}

void DS1307_SetTime(uint8_t hour, uint8_t min, uint8_t sec) {
    I2C_Master_Start();
    I2C_Master_Write(DS1307_ADDR_W);
    I2C_Master_Write(0x00);           
    I2C_Master_Write(decToBcd(sec));  
    I2C_Master_Write(decToBcd(min));  
    I2C_Master_Write(decToBcd(hour)); 
    I2C_Master_Stop();
}

void DS1307_SetDate(uint8_t day, uint8_t month, uint8_t year) {
    I2C_Master_Start();
    I2C_Master_Write(DS1307_ADDR_W);
    I2C_Master_Write(0x04);           
    I2C_Master_Write(decToBcd(day));   
    I2C_Master_Write(decToBcd(month)); 
    I2C_Master_Write(decToBcd(year));  
    I2C_Master_Stop();
}

void DS1307_GetTime(uint8_t *hour, uint8_t *min, uint8_t *sec) {
    I2C_Master_Start();
    I2C_Master_Write(DS1307_ADDR_W);
    I2C_Master_Write(0x00); 
    I2C_Master_RepeatedStart();
    I2C_Master_Write(DS1307_ADDR_R);

    *sec  = bcdToDec(I2C_Master_Read(1) & 0x7F); 
    *min  = bcdToDec(I2C_Master_Read(1));
    *hour = bcdToDec(I2C_Master_Read(0));        
    I2C_Master_Stop();
}

void DS1307_GetDate(uint8_t *day, uint8_t *month, uint8_t *year) {
    I2C_Master_Start();
    I2C_Master_Write(DS1307_ADDR_W);
    I2C_Master_Write(0x04); 
    I2C_Master_RepeatedStart();
    I2C_Master_Write(DS1307_ADDR_R);

    *day   = bcdToDec(I2C_Master_Read(1));
    *month = bcdToDec(I2C_Master_Read(1));
    *year  = bcdToDec(I2C_Master_Read(0));
    I2C_Master_Stop();
}