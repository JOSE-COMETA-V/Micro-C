#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <pic18f4550.h>
#include "Config.h"
#include "LCD.h"

#define DHT11_PIN PORTBbits.RB0
#define DHT11_TRIS TRISBbits.TRISB0

uint8_t humidity_int, humidity_dec, temp_int, temp_dec, checksum;

// Función para iniciar la comunicación con el DHT11
void DHT11_Start() {
    DHT11_TRIS = 0;             // Configurar como salida
    LATBbits.LATB0 = 0;         // Poner en bajo
    __delay_ms(18);             // Mantener por al menos 18ms
    LATBbits.LATB0 = 1;         // Subir la línea
    __delay_us(20);
    DHT11_TRIS = 1;             // Configurar como entrada
}

// Espera a que el DHT11 responda
uint8_t DHT11_CheckResponse() {
    while(DHT11_PIN & 1);   /* wait till bus is High */     
    while(!(DHT11_PIN & 1)); /* wait till bus is Low */
    while(DHT11_PIN & 1);  /* wait till bus is High */
    return 1;
}

// Lee desde el DHT11
char DHT11_ReadData()
{
  char i,data = 0;  
    for(i=0;i<8;i++)
    {
        while(!(DHT11_PIN & 1));  /* wait till 0 pulse, this is start of data pulse */
        __delay_us(30);         
        if(DHT11_PIN & 1)  /* check whether data is 1 or 0 */    
          data = ((data<<1) | 1); 
        else
          data = (data<<1);  
        while(DHT11_PIN & 1);
    }
  return data;
}

void main(void) {
    char value[10]; 
    OSCCONbits.IRCF = 0b111;       // 8 MHz
    OSCCONbits.SCS = 0b10;
    ADCON1 = 0x0F;                 // Digital I/O

    TRISBbits.TRISB0 = 1;          // Pin como entrada inicialmente
    
    LCD_Init();                         /* initialize LCD16x2 in 4-bit mode */
    LCD_String_xy(0, 0, "WELCOME");
    LCD_String_xy(1, 0, "MICROCONTROLADOR");
    __delay_ms(1000);
    LCD_Clear(); 
    while (1) {
        DHT11_Start();
        if (DHT11_CheckResponse()) {
            humidity_int = DHT11_ReadData();
            humidity_dec = DHT11_ReadData();
            temp_int = DHT11_ReadData();
            temp_dec = DHT11_ReadData();
            checksum = DHT11_ReadData();

            if ((humidity_int + humidity_dec + temp_int + temp_dec) == checksum) {
                
                LCD_Clear(); // Limpia antes de escribir nuevos datos

                /* Mostrar humedad */
                LCD_String_xy(0, 0, "RH: ");
                sprintf(value, "%d.%d %%", humidity_int, humidity_dec);
                LCD_String(value);

                /* Mostrar temperatura */
                LCD_String_xy(1, 0, "TEMP: ");
                sprintf(value, "%d.%d ", temp_int, temp_dec);
                LCD_String(value);
                LCD_Char(0xdf);   // Símbolo de grado °
                LCD_Char('C');
            }
            else
            {
                __delay_ms(1000);
                LCD_Clear(); 
                LCD_String_xy(0,0,"CHECKSUM!!");
                LCD_String_xy(1,0,"ERROR!!");

            }
        }
        else
        {
            __delay_ms(1000);
            LCD_Clear(); 
            LCD_String_xy(0,0,"UNRESPONSIVE");
            LCD_String_xy(1,0,"ERROR!!");
        }
        __delay_ms(2000); // Espera 2s entre lecturas (recomendado por el DHT11)
    }
}
