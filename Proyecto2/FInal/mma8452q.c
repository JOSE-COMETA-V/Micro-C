#include "mma8452q.h"
#include "i2c.h"
#include "Config.h"

// --- Funciones auxiliares internas ---
static void mma_write(uint8_t reg, uint8_t val) {
    I2C_Master_Start();
    I2C_Master_Write(MMA8452Q_ADDR_W);
    I2C_Master_Write(reg);
    I2C_Master_Write(val);
    I2C_Master_Stop();
}

static uint8_t mma_read_u8(uint8_t reg) {
    uint8_t v;
    I2C_Master_Start();
    I2C_Master_Write(MMA8452Q_ADDR_W);
    I2C_Master_Write(reg);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(MMA8452Q_ADDR_R);
    v = I2C_Master_Read(0); 
    I2C_Master_Stop();
    return v;
}

// --- Inicialización del acelerómetro ---
void MMA8452Q_Init(void) {
    __delay_ms(5);

    
    uint8_t ctrl1 = mma_read_u8(MMA_REG_CTRL1);
    mma_write(MMA_REG_CTRL1, ctrl1 & ~0x01);

    
    mma_write(MMA_REG_XYZ_DATA_CFG, 0x00);

    
    ctrl1 = mma_read_u8(MMA_REG_CTRL1);
    mma_write(MMA_REG_CTRL1, ctrl1 | 0x01);

    __delay_ms(2);
}

// --- Función interna para extender signo 12 bits ---
static inline int16_t sign_extend_12(uint16_t v) {
    v &= 0x0FFF;
    if (v & 0x0800) v |= 0xF000;
    return (int16_t)v;
}

// --- Lectura cruda de los 3 ejes ---
void MMA8452Q_ReadRaw(int16_t *x, int16_t *y, int16_t *z) {
    uint8_t b[6];

    I2C_Master_Start();
    I2C_Master_Write(MMA8452Q_ADDR_W);
    I2C_Master_Write(MMA_REG_OUT_X_MSB);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(MMA8452Q_ADDR_R);

    b[0] = I2C_Master_Read(1);
    b[1] = I2C_Master_Read(1);
    b[2] = I2C_Master_Read(1);
    b[3] = I2C_Master_Read(1);
    b[4] = I2C_Master_Read(1);
    b[5] = I2C_Master_Read(0); 

    I2C_Master_Stop();

    *x = sign_extend_12(((uint16_t)b[0] << 8 | b[1]) >> 4);
    *y = sign_extend_12(((uint16_t)b[2] << 8 | b[3]) >> 4);
    *z = sign_extend_12(((uint16_t)b[4] << 8 | b[5]) >> 4);
}

// --- Conversión a miligales (mg) ---
void MMA8452Q_Read_mg(int16_t *xmg, int16_t *ymg, int16_t *zmg) {
    int16_t rx, ry, rz;
    MMA8452Q_ReadRaw(&rx, &ry, &rz);
    
    *xmg = (int16_t)((rx * 1000) / 1024);
    *ymg = (int16_t)((ry * 1000) / 1024);
    *zmg = (int16_t)((rz * 1000) / 1024);
}