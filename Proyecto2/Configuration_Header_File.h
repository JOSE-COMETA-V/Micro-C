#ifndef CONFIGURATION_HEADER_FILE_H
#define CONFIGURATION_HEADER_FILE_H

// ============================================
// BITS DE CONFIGURACIÓN PARA OSCILADOR INTERNO
// PIC18F4550 @ 8MHz INTERNO
// ============================================

// CONFIG1L
#pragma config PLLDIV = 1       // No PLL prescaler (NOT used with internal osc)
#pragma config CPUDIV = OSC1_PLL2  // CPU divide by 1
#pragma config USBDIV = 1       // USB clock (NOT used)

// CONFIG1H - OSCILADOR INTERNO
#pragma config FOSC = INTOSCIO_EC  // Internal oscillator, port function on RA6
// O también puede usar: INTOSC_XT, INTOSC_HS, INTOSCIO_EC
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF       // Oscillator Switchover mode disabled

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer enabled
#pragma config BOR = ON         // Brown-out Reset enabled
#pragma config BORV = 3         // Brown-out voltage (2.0V)
#pragma config VREGEN = OFF     // USB voltage regulator disabled

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer disabled
#pragma config WDTPS = 32768    // Watchdog postscaler

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 input/output multiplexed with RC1
#pragma config PBADEN = OFF     // PORTB<4:0> digital I/O on Reset
#pragma config LPT1OSC = OFF    // Timer1 low-power oscillator disabled
#pragma config MCLRE = ON       // MCLR pin enabled

// CONFIG4L
#pragma config STVREN = ON      // Stack overflow/underflow reset enabled
#pragma config LVP = OFF        // Low-voltage programming disabled
#pragma config ICPRT = OFF      // ICPORT disabled
#pragma config XINST = OFF      // Extended instruction set disabled

// CONFIG5L - Protección de código
#pragma config CP0 = OFF
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#pragma config CP3 = OFF

// CONFIG5H
#pragma config CPB = OFF        // Boot block code protection
#pragma config CPD = OFF        // Data EEPROM code protection

// CONFIG6L - Protección de escritura
#pragma config WRT0 = OFF
#pragma config WRT1 = OFF
#pragma config WRT2 = OFF
#pragma config WRT3 = OFF

// CONFIG6H
#pragma config WRTC = OFF       // Configuration register write protection
#pragma config WRTB = OFF       // Boot block write protection
#pragma config WRTD = OFF       // Data EEPROM write protection

// CONFIG7L - Protección de lectura de tabla
#pragma config EBTR0 = OFF
#pragma config EBTR1 = OFF
#pragma config EBTR2 = OFF
#pragma config EBTR3 = OFF

// CONFIG7H
#pragma config EBTRB = OFF      // Boot block table read protection

#endif
