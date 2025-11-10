#ifndef MMA8452Q_H
#define MMA8452Q_H

#include <xc.h>
#include <stdint.h>


#define MMA8452Q_ADDR_W  0x3A   // Write
#define MMA8452Q_ADDR_R  0x3B   // Read

// Registros del dispositivo
#define MMA_REG_STATUS       0x00
#define MMA_REG_OUT_X_MSB    0x01
#define MMA_REG_XYZ_DATA_CFG 0x0E
#define MMA_REG_WHO_AM_I     0x0D
#define MMA_REG_CTRL1        0x2A
#define MMA_REG_CTRL2        0x2B

// Funciones principales
void MMA8452Q_Init(void);
void MMA8452Q_ReadRaw(int16_t *x, int16_t *y, int16_t *z);
void MMA8452Q_Read_mg(int16_t *xmg, int16_t *ymg, int16_t *zmg);

#endif