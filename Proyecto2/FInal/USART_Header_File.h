#ifndef USART_HEADER_FILE_H
#define USART_HEADER_FILE_H

#include <xc.h>


#define F_CPU 8000000UL  // 8 MHz oscilador interno


#define Baud_value(baud_rate) ((unsigned int)(((F_CPU)/(64UL * (unsigned long)(baud_rate))) - 1))

// Prototipos de funciones
void USART_Init(unsigned long baud_rate);
void USART_TxChar(char data);

// ============================================
// INICIALIZACIÓN USART
// ============================================
void USART_Init(unsigned long baud_rate)
{
    unsigned int spbrg_val;
    
    
    TRISCbits.TRISC6 = 0;  
    TRISCbits.TRISC7 = 1;  
    
    
    spbrg_val = Baud_value(baud_rate);
    SPBRG = (unsigned char)(spbrg_val & 0xFF);
    
    
    TXSTA = 0x20;
    
    
    RCSTA = 0x90;
    
    
    PIR1bits.RCIF = 0;
    PIR1bits.TXIF = 0;
}

// ============================================
// TRANSMITIR CARÁCTER (Opcional para debug)
// ============================================
void USART_TxChar(char data)
{
    while(!PIR1bits.TXIF);  
    TXREG = data;           
}

#endif
