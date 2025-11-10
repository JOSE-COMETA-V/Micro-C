#ifndef USART_HEADER_FILE_H
#define USART_HEADER_FILE_H

#include <xc.h>

#define F_CPU 8000000UL  

#define Baud_value(baud_rate) ((unsigned int)(((F_CPU)/(64UL * (unsigned long)(baud_rate))) - 1))

void USART_Init(unsigned long baud_rate);
void USART_TxChar(char data);

void USART_Init(unsigned long baud_rate)
{
    unsigned int spbrg_val;
    
    
    TRISCbits.TRISC6 = 0;  
    TRISCbits.TRISC7 = 1;  
    
    // Calcular SPBRG
    spbrg_val = Baud_value(baud_rate);
    SPBRG = (unsigned char)(spbrg_val & 0xFF);
    
    
    TXSTA = 0x20;
    
    
    RCSTA = 0x90;
    
    
    PIR1bits.RCIF = 0;
    PIR1bits.TXIF = 0;
}

void USART_TxChar(char data)
{
    while(!PIR1bits.TXIF);  // Esperar que el buffer esté listo
    TXREG = data;           // Enviar dato
}

#endif

