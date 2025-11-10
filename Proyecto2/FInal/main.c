#include <xc.h>
#include <stdio.h>
#include "Config.h"
#include "LCD.h"
#include "i2c.h"
#include "ds1307.h"
#include "mma8452q.h"
#include "dht11.h"
#include "gps.h"

#define BUTTON_TRIS  TRISBbits.TRISB4
#define BUTTON_PORT  PORTBbits.RB4

static unsigned char Button_WasPressed(void);
static void Wait_While_Paused(void);



char buf[17];
int16_t xmg, ymg, zmg;
uint8_t hh, mm, ss, dd, mo, yy;

void ShowRTC(void) {
    DS1307_GetTime(&hh, &mm, &ss);
    DS1307_GetDate(&dd, &mo, &yy);
    LCD_Clear();
    snprintf(buf, sizeof(buf), "%02u:%02u:%02u", hh, mm, ss);
    LCD_String_xy(0, 4, buf);
    snprintf(buf, sizeof(buf), "%02u/%02u/20%02u", dd, mo, yy);
    LCD_String_xy(1, 3, buf);
}

void ShowACC(void) {
    MMA8452Q_Read_mg(&xmg, &ymg, &zmg);
    LCD_Clear();
    snprintf(buf, sizeof(buf), "X:%5dmg", (int)xmg);
    LCD_String_xy(0, 0, buf);
    snprintf(buf, sizeof(buf), "Y:%4d Z:%4d", (int)ymg, (int)zmg);
    LCD_String_xy(1, 0, buf);
}

void ShowDHT(void) {
    LCD_Clear();
    if (DHT11_ReadValues()) {
        LCD_String_xy(0, 0, "DHT11 SENSOR");
        snprintf(buf, sizeof(buf), "T:%2dC H:%2d%%", temp_int, humidity_int);
        LCD_String_xy(1, 1, buf);
    } else {
        LCD_String_xy(0, 1, "Error DHT11");
        LCD_String_xy(1, 1, "Sin respuesta");
    }
}

void ShowGPS(void) {
    gps_data_t g = {0};
    LCD_Clear();
    
    if (GPS_Update(&g) && g.valid) {
        
        snprintf(buf, sizeof(buf), "Lat:%7.4f%c", (g.lat_deg>=0? g.lat_deg:-g.lat_deg), g.ns);
        LCD_String_xy(0, 0, buf);
        snprintf(buf, sizeof(buf), "Lon:%7.4f%c", (g.lon_deg>=0? g.lon_deg:-g.lon_deg), g.ew);
        LCD_String_xy(1, 0, buf);
        __delay_ms(3000);

        LCD_Clear();
        
        unsigned int th = (unsigned int)(g.utc_hhmmss/10000UL);
        unsigned int tm = (unsigned int)((g.utc_hhmmss%10000UL)/100UL);
        unsigned int ts = (unsigned int)(g.utc_hhmmss%100UL);
        snprintf(buf, sizeof(buf), "Alt: %.1f m", g.alt_m);
        LCD_String_xy(0, 0, buf);
        snprintf(buf, sizeof(buf), "UTC %02u:%02u:%02u", th, tm, ts);
        LCD_String_xy(1, 0, buf);
    } else {
        LCD_String_xy(0, 1, "GPS: esperando");
        LCD_String_xy(1, 3, "GPGGA...");
    }
}

// Devuelve 1 exactamente una vez por pulsación (con debounce)
static unsigned char Button_WasPressed(void) {
    static unsigned char last = 1;     // 1 = suelto (pull-up)
    unsigned char now = BUTTON_PORT;

    if (last == 1 && now == 0) {       // flanco de bajada
        __delay_ms(25);                // debounce
        if (BUTTON_PORT == 0) {
            last = 0;
            return 1;                  // se detectó pulsación
        }
    } else if (now == 1) {
        last = 1;                      // botón suelto
    }
    return 0;
}

static void Wait_While_Paused(void) {
    // Mostrar una sola vez
    LCD_Clear();
    LCD_String_xy(0, 1, "Sistema detenido");
    LCD_String_xy(1, 2, "Presione boton");
    // Quedarse aquí hasta próxima pulsación
    while (1) {
        if (Button_WasPressed()) break;
        __delay_ms(30);
    }
    LCD_Clear();
}

void main(void)
{
    
    OSCCONbits.IRCF = 0b111; OSCCONbits.SCS = 0b10;
    ADCON1 = 0x0F;          

    LCD_Init();             
    LCD_Clear();

    I2C_Master_Init(100000);     
    DS1307_Init();
    MMA8452Q_Init();

    GPS_Init(9600);              

    LCD_String_xy(0, 0, "Sistema 4 modulos");
    LCD_String_xy(1, 0, "RTC-ACC-DHT-GPS");
    __delay_ms(1500);
    
    // --- Botón en RB4 con pull-up interno ---
    BUTTON_TRIS = 1;                 // RB4 como entrada
    INTCON2bits.RBPU = 0;            // Habilita pull-ups en PORTB (RB4?RB7)
/* Con esto el pin RB4 queda '1' en reposo y '0' al presionar */

    while (1) {
        if (Button_WasPressed()) { Wait_While_Paused(); }
        ShowRTC();  __delay_ms(3000);
        if (Button_WasPressed()) { Wait_While_Paused(); }
        ShowACC();  __delay_ms(3000);
        if (Button_WasPressed()) { Wait_While_Paused(); }
        ShowDHT();  __delay_ms(3000);
        if (Button_WasPressed()) { Wait_While_Paused(); }
        ShowGPS();  __delay_ms(3000);
        LCD_Clear();
    }
}