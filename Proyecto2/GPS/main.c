#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Configuration_Header_File.h"
#include "LCD_16x2_Header_File.h"
#include "USART_Header_File.h"

// Prototipos de funciones GPS
void parse_GPGGA(void);
unsigned long int extract_time(void);
float extract_latitude(unsigned char comma_pos);
float extract_longitude(unsigned char comma_pos);
float extract_altitude(unsigned char comma_pos);
float convert_to_degrees(float raw_value, char direction);

// Buffer y variables para GPS - VOLATILE para uso en ISR
#define GGA_BUFFER_SIZE 80
#define MAX_COMMA_POSITIONS 15

volatile char GGA_Buffer[GGA_BUFFER_SIZE];
volatile unsigned char GGA_Index = 0;
volatile unsigned char GGA_Ready = 0;
volatile unsigned char IsGGAString = 0;

// Variables para parsing
char GGA_Code[3] = {0, 0, 0};
unsigned char Comma_Positions[MAX_COMMA_POSITIONS];
unsigned char Comma_Count = 0;

// Variables para almacenar datos GPS
unsigned long int GPS_Time = 0;
float GPS_Latitude = 0.0;
float GPS_Longitude = 0.0;
float GPS_Altitude = 0.0;
char N_S = 'N';
char E_W = 'E';

void main(void) 
{
    char display_buffer[16];
    unsigned int hour, min, sec;
    
    // ============================================
    // CONFIGURACIÓN OSCILADOR INTERNO A 8MHz
    // ============================================
    OSCCONbits.IRCF2 = 1;  
    OSCCONbits.IRCF1 = 1;  
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS1 = 1;   
    OSCCONbits.SCS0 = 0;   
    
    
    while(!OSCCONbits.IOFS);  
    
    // ============================================
    // CONFIGURAR PUERTOS COMO DIGITALES
    // ============================================
    ADCON1 = 0x0F;  
    
    // ============================================
    // INICIALIZACIÓN DE PERIFÉRICOS
    // ============================================
    
    
    LCD_Init();
    LCD_String_xy(1, 0, "GPS Init...");
    LCD_String_xy(2, 0, "8MHz Internal");
    MSdelay(1500);
    
    // Inicializar USART a 9600 baudios
    USART_Init(9600);
    
    
    INTCONbits.GIE = 1;  
    INTCONbits.PEIE = 1;  
    PIE1bits.RCIE = 1;    
    
    LCD_Clear();
    LCD_String_xy(1, 0, "Waiting GPS...");
    LCD_String_xy(2, 0, "Searching...");
    
    while(1) 
    {
        
        if(GGA_Ready == 1) 
        {
            
            parse_GPGGA();
            
            
            LCD_Clear();
            
            // Línea 1: Latitud
            memset(display_buffer, 0, 16);
            if(GPS_Latitude >= 0)
                sprintf(display_buffer, "Lat:%7.4f%c", GPS_Latitude, N_S);
            else
                sprintf(display_buffer, "Lat:%7.4f%c", -GPS_Latitude, N_S);
            LCD_String_xy(1, 0, display_buffer);
            
            // Línea 2: Longitud
            memset(display_buffer, 0, 16);
            if(GPS_Longitude >= 0)
                sprintf(display_buffer, "Lon:%7.4f%c", GPS_Longitude, E_W);
            else
                sprintf(display_buffer, "Lon:%7.4f%c", -GPS_Longitude, E_W);
            LCD_String_xy(2, 0, display_buffer);
            
            MSdelay(3000);  // Mostrar por 3 segundos
            
            
            LCD_Clear();
            
            // Línea 1: Altitud
            memset(display_buffer, 0, 16);
            sprintf(display_buffer, "Alt: %.1f m", GPS_Altitude);
            LCD_String_xy(1, 0, display_buffer);
            
            // Línea 2: Hora UTC
            memset(display_buffer, 0, 16);
            hour = (unsigned int)(GPS_Time / 10000UL);
            min = (unsigned int)((GPS_Time % 10000UL) / 100UL);
            sec = (unsigned int)((GPS_Time % 10000UL) % 100UL);
            sprintf(display_buffer, "UTC %02d:%02d:%02d", hour, min, sec);
            LCD_String_xy(2, 0, display_buffer);
            
            MSdelay(3000);  
            
            
            GGA_Ready = 0;
        }
    }
}

// ============================================
// RUTINA DE INTERRUPCIÓN USART
// ============================================
void __interrupt() Serial_ISR(void)
{
    if(RCIF) 
    {
        unsigned char received_char;
        
        // Verificar overrun error
        if(RCSTAbits.OERR) 
        {
            // Limpiar error de overrun
            CREN = 0;
            NOP();
            CREN = 1;
        }
        
        
        received_char = RCREG;
        
        
        if(received_char == '$') 
        {
        
            GGA_Index = 0;
            IsGGAString = 0;
            Comma_Count = 0;
            memset((void*)GGA_Buffer, 0, GGA_BUFFER_SIZE);
        }
        else if(IsGGAString == 1) 
        {
            
            if(received_char == ',') 
            {
                if(Comma_Count < MAX_COMMA_POSITIONS) 
                {
                    Comma_Positions[Comma_Count++] = GGA_Index;
                }
            }
            
            GGA_Buffer[GGA_Index++] = received_char;
            
            
            if(received_char == '\n' && GGA_Index > 1) 
            {
                if(GGA_Buffer[GGA_Index - 2] == '\r') 
                {
                    GGA_Ready = 1; 
                    IsGGAString = 0;
                }
            }
            
            
            if(GGA_Index >= GGA_BUFFER_SIZE - 1) 
            {
                GGA_Index = 0;
                IsGGAString = 0;
            }
        }
        else 
        {
            
            GGA_Code[0] = GGA_Code[1];
            GGA_Code[1] = GGA_Code[2];
            GGA_Code[2] = received_char;
            
            if(GGA_Code[0] == 'G' && GGA_Code[1] == 'G' && GGA_Code[2] == 'A') 
            {
                IsGGAString = 1;
                GGA_Code[0] = 0;
                GGA_Code[1] = 0;
                GGA_Code[2] = 0;
            }
        }
        
        RCIF = 0;  
    }
}


void parse_GPGGA(void)
{
    if(Comma_Count < 10) return; 
    
    
    GPS_Time = extract_time();
    
    
    GPS_Latitude = extract_latitude(Comma_Positions[0]);
    GPS_Latitude = convert_to_degrees(GPS_Latitude, N_S);
    
    
    GPS_Longitude = extract_longitude(Comma_Positions[2]);
    GPS_Longitude = convert_to_degrees(GPS_Longitude, E_W);
    
    
    if(Comma_Count >= 9) 
    {
        GPS_Altitude = extract_altitude(Comma_Positions[7]);
    }
}

unsigned long int extract_time(void)
{
    char time_buffer[12];
    unsigned char i;
    unsigned long int time_value;
    
    memset(time_buffer, 0, 12);
    
    for(i = 0; i < Comma_Positions[0] && i < 11; i++) 
    {
        time_buffer[i] = GGA_Buffer[i];
    }
    
    time_value = (unsigned long int)atol(time_buffer);
    return time_value;
}

float extract_latitude(unsigned char comma_pos)
{
    unsigned char start = comma_pos + 1;
    unsigned char end = Comma_Positions[1];
    unsigned char i, j = 0;
    char lat_buffer[15];
    float latitude;
    
    memset(lat_buffer, 0, 15);
    
    for(i = start; i < end && j < 14; i++, j++) 
    {
        lat_buffer[j] = GGA_Buffer[i];
    }
    
    if(Comma_Positions[1] + 1 < GGA_BUFFER_SIZE) 
    {
        N_S = GGA_Buffer[Comma_Positions[1] + 1];
    }
    
    latitude = atof(lat_buffer);
    return latitude;
}

float extract_longitude(unsigned char comma_pos)
{
    unsigned char start = comma_pos + 1;
    unsigned char end = Comma_Positions[3];
    unsigned char i, j = 0;
    char lon_buffer[15];
    float longitude;
    
    memset(lon_buffer, 0, 15);
    
    for(i = start; i < end && j < 14; i++, j++) 
    {
        lon_buffer[j] = GGA_Buffer[i];
    }
    
    if(Comma_Positions[3] + 1 < GGA_BUFFER_SIZE) 
    {
        E_W = GGA_Buffer[Comma_Positions[3] + 1];
    }
    
    longitude = atof(lon_buffer);
    return longitude;
}

float extract_altitude(unsigned char comma_pos)
{
    unsigned char start = comma_pos + 1;
    unsigned char end = Comma_Positions[8];
    unsigned char i, j = 0;
    char alt_buffer[12];
    float altitude;
    
    memset(alt_buffer, 0, 12);
    
    for(i = start; i < end && j < 11; i++, j++) 
    {
        alt_buffer[j] = GGA_Buffer[i];
    }
    
    altitude = atof(alt_buffer);
    return altitude;
}

float convert_to_degrees(float raw_value, char direction)
{
    int degrees;
    float minutes;
    float decimal;
    
    degrees = (int)(raw_value / 100.0);
    minutes = raw_value - (degrees * 100.0);
    decimal = degrees + (minutes / 60.0);
    
    if(direction == 'S' || direction == 'W') 
    {
        decimal *= -1.0;
    }
    
    return decimal;
}
