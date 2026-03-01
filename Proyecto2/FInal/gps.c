#include <string.h>
#include <stdlib.h>
#include "gps.h"
#include "USART_Header_File.h"   

// Buffers/estados para GPGGA 
#define GGA_BUFFER_SIZE 80
#define MAX_COMMA_POSITIONS 15

static volatile char GGA_Buffer[GGA_BUFFER_SIZE];
static volatile unsigned char GGA_Index = 0;
static volatile unsigned char GGA_Ready = 0;
static volatile unsigned char IsGGAString = 0;

static char GGA_Code[3] = {0,0,0};
static volatile unsigned char Comma_Positions[MAX_COMMA_POSITIONS];
static volatile unsigned char Comma_Count = 0;

static unsigned long GPS_Time = 0;
static float GPS_Latitude = 0.0f, GPS_Longitude = 0.0f, GPS_Altitude = 0.0f;
static char N_S = 'N', E_W = 'E';

static unsigned long extract_time(void);
static float extract_latitude(unsigned char comma_pos);
static float extract_longitude(unsigned char comma_pos);
static float extract_altitude(unsigned char comma_pos);
static float to_degrees(float raw, char dir);
static void parse_GPGGA(void);

void GPS_Init(unsigned long baud)
{
   
    USART_Init(baud);
    INTCONbits.GIE  = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.RCIE   = 1;
    
    GGA_Index = 0; GGA_Ready = 0; IsGGAString = 0; Comma_Count = 0;
}

uint8_t GPS_Update(gps_data_t *out)
{
    if (!GGA_Ready) return 0;           
    parse_GPGGA();                      
    GGA_Ready = 0;                      

    out->utc_hhmmss = GPS_Time;
    out->lat_deg    = to_degrees(GPS_Latitude,  N_S);
    out->lon_deg    = to_degrees(GPS_Longitude, E_W);
    out->alt_m      = GPS_Altitude;
    out->ns         = N_S;
    out->ew         = E_W;
    out->valid      = 1;
    return 1;
}


void __interrupt() GPS_Serial_ISR(void)
{
    if (PIR1bits.RCIF)
    {
        unsigned char c;

        if (RCSTAbits.OERR) { CREN = 0; NOP(); CREN = 1; }

        c = RCREG;

        if (c == '$') {
            GGA_Index = 0; IsGGAString = 0; Comma_Count = 0;
            memset((void*)GGA_Buffer, 0, GGA_BUFFER_SIZE);
        }
        else if (IsGGAString == 1)
        {
            if (c == ',') {
                if (Comma_Count < MAX_COMMA_POSITIONS)
                    Comma_Positions[Comma_Count++] = GGA_Index;
            }
            GGA_Buffer[GGA_Index++] = c;

            if (c == '\n' && GGA_Index > 1) {
                if (GGA_Buffer[GGA_Index-2] == '\r') {
                    GGA_Ready = 1;
                    IsGGAString = 0;
                }
            }
            if (GGA_Index >= (GGA_BUFFER_SIZE-1)) { GGA_Index = 0; IsGGAString = 0; }
        }
        else
        {
            GGA_Code[0] = GGA_Code[1];
            GGA_Code[1] = GGA_Code[2];
            GGA_Code[2] = c;

            if (GGA_Code[0]=='G' && GGA_Code[1]=='G' && GGA_Code[2]=='A') {
                IsGGAString = 1; GGA_Code[0]=GGA_Code[1]=GGA_Code[2]=0;
            }
        }
        PIR1bits.RCIF = 0;
    }
}


static void parse_GPGGA(void)
{
    if (Comma_Count < 10) return;
    GPS_Time      = extract_time();
    GPS_Latitude  = extract_latitude(Comma_Positions[0]);
    GPS_Longitude = extract_longitude(Comma_Positions[2]);
    if (Comma_Count >= 9)
        GPS_Altitude = extract_altitude(Comma_Positions[7]);
}
static unsigned long extract_time(void)
{
    char buf[12]={0}; unsigned char i;
    for (i=0; i<Comma_Positions[0] && i<11; i++) buf[i]=GGA_Buffer[i];
    return (unsigned long)atol(buf);
}
static float extract_latitude(unsigned char comma_pos)
{
    unsigned char s=comma_pos+1, e=Comma_Positions[1], i,j=0; char b[15]={0};
    for (i=s; i<e && j<14; i++,j++) b[j]=GGA_Buffer[i];
    if (Comma_Positions[1]+1 < GGA_BUFFER_SIZE) N_S = GGA_Buffer[Comma_Positions[1]+1];
    return (float)atof(b);
}
static float extract_longitude(unsigned char comma_pos)
{
    unsigned char s=comma_pos+1, e=Comma_Positions[3], i,j=0; char b[15]={0};
    for (i=s; i<e && j<14; i++,j++) b[j]=GGA_Buffer[i];
    if (Comma_Positions[3]+1 < GGA_BUFFER_SIZE) E_W = GGA_Buffer[Comma_Positions[3]+1];
    return (float)atof(b);
}
static float extract_altitude(unsigned char comma_pos)
{
    unsigned char s=comma_pos+1, e=Comma_Positions[8], i,j=0; char b[12]={0};
    for (i=s; i<e && j<11; i++,j++) b[j]=GGA_Buffer[i];
    return (float)atof(b);
}
static float to_degrees(float raw, char dir)
{
    int deg = (int)(raw/100.0f);
    float minutes = raw - (deg*100.0f);
    float dec = deg + (minutes/60.0f);
    if (dir=='S' || dir=='W') dec = -dec;
    return dec;
}
