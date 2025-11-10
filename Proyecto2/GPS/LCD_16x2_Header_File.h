#ifndef LCD_16X2_HEADER_FILE_H
#define LCD_16X2_HEADER_FILE_H

#include <xc.h>

#define _XTAL_FREQ 8000000UL  // 8MHz oscilador interno

#define RS LATDbits.LATD0
#define EN LATDbits.LATD1
#define ldata LATB


void LCD_Init(void);
void LCD_Command(char cmd);
void LCD_Char(char data);
void LCD_String(const char *msg);
void LCD_String_xy(char row, char pos, const char *msg);
void LCD_Clear(void);
void MSdelay(unsigned int val);

void MSdelay(unsigned int val)
{
    unsigned int i;
    for(i = 0; i < val; i++)
        __delay_ms(1);
}

void LCD_Init(void)
{

    TRISB = 0x00; 
    TRISD = 0x00; 
    
    MSdelay(20);  
    
    
    LCD_Command(0x38);   
    MSdelay(5);
    LCD_Command(0x38);   
    MSdelay(1);
    LCD_Command(0x38);   
    
    LCD_Command(0x0C);   
    LCD_Command(0x06);   
    LCD_Command(0x01);   
    MSdelay(2);
}

void LCD_Command(char cmd)
{
    ldata = cmd;   
    RS = 0;        
    EN = 1;        
    __delay_us(1);
    EN = 0;
    MSdelay(3);    
}

void LCD_Char(char data)
{
    ldata = data;  
    RS = 1;        
    EN = 1;        
    __delay_us(1);
    EN = 0;
    MSdelay(1);    
}

void LCD_String(const char *msg)
{
    while(*msg != 0)
    {
        LCD_Char(*msg);
        msg++;
    }
}

void LCD_String_xy(char row, char pos, const char *msg)
{
    char location = 0;
    
    if(row == 1)
    {
        location = (char)(0x80 | (pos & 0x0F));  
    }
    else if(row == 2)
    {
        location = (char)(0xC0 | (pos & 0x0F));  
    }
    
    LCD_Command(location);
    LCD_String(msg);
}

void LCD_Clear(void)
{
    LCD_Command(0x01);  
    MSdelay(2);         
}

#endif

