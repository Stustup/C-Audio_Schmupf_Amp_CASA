#ifndef PCA9536_H_
#define PCA9536_H_

#include "stm32f0xx_hal.h"  //needed for I2C

//Defines
#define PCA9536_address (0x41 << 1)
//PCA9536 Registers
#define PCA9536_inputRegister 0x00
#define PCA9536_outputRegister 0x01
#define PCA9536_configRegister 0x03

//Main struct
typedef struct {
    I2C_HandleTypeDef *i2cHandle;

    uint8_t _port0;
    uint8_t _port1;
} PCA9536;

//Function prototypes
uint8_t PCA9536_Init(PCA9536 *_device, I2C_HandleTypeDef *_i2c_handle);
uint8_t PCA9536_write(PCA9536 *_device, uint8_t _register, uint8_t *value);
uint8_t PCA9536_read(PCA9536 *_device, uint8_t _register, uint8_t *_data);
uint8_t PCA9536_configPinOutput(PCA9536 *_device);
uint8_t PCA9536_writeLOW(PCA9536 *_device);
uint8_t PCA9536_writeHIGH(PCA9536 *_device);
uint8_t PCA9536_switchPort(PCA9536 *_device, uint8_t _port);

#endif