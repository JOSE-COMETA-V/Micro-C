#ifndef USART_HEADER_FILE_H
#define USART_HEADER_FILE_H

#include <xc.h>

// ============================================
// CONFIGURACIÓN PARA OSCILADOR INTERNO 8MHz
// ============================================
#define F_CPU 8000000UL  // 8 MHz oscilador interno

// Fórmula para SPBRG con BRGH=0 (Low Speed)
// SPBRG = (Fosc / (64 * Baud_Rate)) - 1
// Para 8MHz y 9600 bps: SPBRG = (8000000/(64*9600)) - 1 = 12
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
    
    // Configurar pines TX y RX
    TRISCbits.TRISC6 = 0;  // TX como salida
    TRISCbits.TRISC7 = 1;  // RX como entrada
    
    // Calcular SPBRG
    spbrg_val = Baud_value(baud_rate);
    SPBRG = (unsigned char)(spbrg_val & 0xFF);
    
    // Configurar USART
    // TXSTA: 0x20 = 0010 0000
    // bit 5 TXEN = 1 (Transmit enabled)
    // bit 4 SYNC = 0 (Asynchronous mode)
    // bit 2 BRGH = 0 (Low speed)
    TXSTA = 0x20;
    
    // RCSTA: 0x90 = 1001 0000  
    // bit 7 SPEN = 1 (Serial port enabled)
    // bit 4 CREN = 1 (Continuous receive enabled)
    RCSTA = 0x90;
    
    // Limpiar flags
    PIR1bits.RCIF = 0;
    PIR1bits.TXIF = 0;
}

// ============================================
// TRANSMITIR CARÁCTER (Opcional para debug)
// ============================================
void USART_TxChar(char data)
{
    while(!PIR1bits.TXIF);  // Esperar que el buffer esté listo
    TXREG = data;           // Enviar dato
}

#endif
