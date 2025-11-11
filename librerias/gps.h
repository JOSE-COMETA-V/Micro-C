#ifndef GPS_H
#define GPS_H

#include <xc.h>
#include <stdint.h>

typedef struct {
    unsigned long utc_hhmmss; // hhmmss (UTC)
    float lat_deg;            // grados decimales (+N/-S)
    float lon_deg;            // grados decimales (+E/-W)
    float alt_m;              // metros
    char  ns, ew;             // 'N'/'S', 'E'/'W'
    uint8_t valid;            // 1 si hay datos parseados recientes
} gps_data_t;

void GPS_Init(unsigned long baud);
uint8_t GPS_Update(gps_data_t *out);  // devuelve 1 si hay datos nuevos

// ISR de recepción UART (declaración, implementación en gps.c)
void __interrupt() GPS_Serial_ISR(void);

#endif