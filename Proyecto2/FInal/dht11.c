#include "dht11.h"

uint8_t humidity_int, humidity_dec, temp_int, temp_dec, checksum;

// Inicia la comunicación con el DHT11
void DHT11_Start(void) {
    DHT11_TRIS = 0;     
    DHT11_LAT = 0;      
    __delay_ms(18);     
    DHT11_LAT = 1;      
    __delay_us(20);
    DHT11_TRIS = 1;     
}

// Espera la respuesta del DHT11
uint8_t DHT11_CheckResponse(void) {
    uint16_t timeout = 0;
    while(DHT11_PIN && timeout++ < 10000);   
    if(timeout >= 10000) return 0;
    timeout = 0;
    while(!DHT11_PIN && timeout++ < 10000);   
    if(timeout >= 10000) return 0;
    timeout = 0;
    while(DHT11_PIN && timeout++ < 10000);    
    if(timeout >= 10000) return 0;
    return 1;
}

// Lee un byte del DHT11
uint8_t DHT11_ReadData(void) {
    uint8_t i, data = 0;
    for (i = 0; i < 8; i++) {
        while (!DHT11_PIN);    
        __delay_us(30);
        if (DHT11_PIN)
            data = (data << 1) | 1;
        else
            data = (data << 1);
        while (DHT11_PIN);     
    }
    return data;
}

// Lee todos los datos del DHT11
uint8_t DHT11_ReadValues(void) {
    DHT11_Start();
    if (!DHT11_CheckResponse()) return 0;

    humidity_int = DHT11_ReadData();
    humidity_dec = DHT11_ReadData();
    temp_int = DHT11_ReadData();
    temp_dec = DHT11_ReadData();
    checksum = DHT11_ReadData();

    if ((humidity_int + humidity_dec + temp_int + temp_dec) != checksum)
        return 0;

    return 1;
}
