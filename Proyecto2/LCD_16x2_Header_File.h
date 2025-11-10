#ifndef LCD_16X2_HEADER_FILE_H
#define LCD_16X2_HEADER_FILE_H

#include <xc.h>

// ============================================
// DEFINIR FRECUENCIA DEL OSCILADOR PARA DELAYS
// ============================================
#define _XTAL_FREQ 8000000UL  // 8MHz oscilador interno

// ============================================
// DEFINICIONES DE PINES LCD
// ============================================
#define RS LATDbits.LATD0
#define EN LATDbits.LATD1
#define ldata LATB

// Prototipos de funciones
void LCD_Init(void);
void LCD_Command(char cmd);
void LCD_Char(char data);
void LCD_String(const char *msg);
void LCD_String_xy(char row, char pos, const char *msg);
void LCD_Clear(void);
void MSdelay(unsigned int val);

// ============================================
// FUNCIÓN DE DELAY EN MILISEGUNDOS
// ============================================
void MSdelay(unsigned int val)
{
    unsigned int i;
    for(i = 0; i < val; i++)
        __delay_ms(1);
}

// ============================================
// INICIALIZACIÓN DEL LCD
// ============================================
void LCD_Init(void)
{
    // Configurar puertos como salida
    TRISB = 0x00;  // PORTB como salida (datos)
    TRISD = 0x00;  // PORTD como salida (control)
    
    MSdelay(20);   // Esperar Power-on (>15ms)
    
    // Secuencia de inicialización del LCD
    LCD_Command(0x38);   // Modo 8-bit, 2 líneas, 5x8 puntos
    MSdelay(5);
    LCD_Command(0x38);   // Repetir comando
    MSdelay(1);
    LCD_Command(0x38);   // Repetir comando
    
    LCD_Command(0x0C);   // Display ON, Cursor OFF, Blink OFF
    LCD_Command(0x06);   // Entry mode: incrementar, no shift
    LCD_Command(0x01);   // Limpiar display
    MSdelay(2);
}

// ============================================
// ENVIAR COMANDO AL LCD
// ============================================
void LCD_Command(char cmd)
{
    ldata = cmd;   // Poner comando en el bus de datos
    RS = 0;        // RS=0 para comando
    EN = 1;        // Pulso de enable
    __delay_us(1);
    EN = 0;
    MSdelay(3);    // Esperar que el LCD procese el comando
}

// ============================================
// ENVIAR CARÁCTER AL LCD
// ============================================
void LCD_Char(char data)
{
    ldata = data;  // Poner dato en el bus
    RS = 1;        // RS=1 para datos
    EN = 1;        // Pulso de enable
    __delay_us(1);
    EN = 0;
    MSdelay(1);    // Tiempo de escritura
}

// ============================================
// ENVIAR CADENA AL LCD
// ============================================
void LCD_String(const char *msg)
{
    while(*msg != 0)
    {
        LCD_Char(*msg);
        msg++;
    }
}

// ============================================
// POSICIONAR Y ESCRIBIR CADENA
// ============================================
void LCD_String_xy(char row, char pos, const char *msg)
{
    char location = 0;
    
    if(row == 1)
    {
        location = (char)(0x80 | (pos & 0x0F));  // Primera línea (0x80 - 0x8F)
    }
    else if(row == 2)
    {
        location = (char)(0xC0 | (pos & 0x0F));  // Segunda línea (0xC0 - 0xCF)
    }
    
    LCD_Command(location);
    LCD_String(msg);
}

// ============================================
// LIMPIAR PANTALLA LCD
// ============================================
void LCD_Clear(void)
{
    LCD_Command(0x01);  // Comando clear display
    MSdelay(2);         // Esperar que se complete
}

#endif
