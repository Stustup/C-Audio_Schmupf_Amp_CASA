#include "PCA9536.h"
#include "stm32f0xx_hal_i2c.h"

uint8_t PCA9536_Init(PCA9536 *_device, I2C_HandleTypeDef *_i2c_handle) {
    _device->i2cHandle   = _i2c_handle;
    _device->_port0     = 0;
    _device->_port1    = 0;

    uint8_t _error = 0;
    if(HAL_I2C_IsDeviceReady(_device->i2cHandle, PCA9536_address, 10, 100) == 1) _error++ ;
    if(PCA9536_writeHIGH(_device)) _error++;

    return _error;
}

/**
 * @brief Main write function for outputs of the PCA.
 */
uint8_t PCA9536_write(PCA9536 *_device, uint8_t _register, uint8_t *_value) {
    return HAL_I2C_Mem_Write(_device->i2cHandle, PCA9536_address, _register, I2C_MEMADD_SIZE_8BIT, _value, 1, HAL_MAX_DELAY);
}

/**
 * @brief Main read function for inputs of the PCA.
 */
uint8_t PCA9536_read(PCA9536 *_device, uint8_t _register, uint8_t *_data) {
    return HAL_I2C_Mem_Read(_device->i2cHandle, PCA9536_address, _register, I2C_MEMADD_SIZE_8BIT, _data, 1, HAL_MAX_DELAY);
}

/**
 * Sets all outputs to LOW
 */
uint8_t PCA9536_writeLOW(PCA9536 *_device) {
    uint8_t _data = 0x00;
    return(PCA9536_write(_device, PCA9536_outputRegister, &_data));
}

/**
 * Sets all outputs to HIGH
 */
uint8_t PCA9536_writeHIGH(PCA9536 *_device) {
    uint8_t _data = 0x0F;
    return(PCA9536_write(_device, PCA9536_outputRegister, &_data));
}

/**
 * Configures all pins as outputs
 */
uint8_t PCA9536_configPinOutput(PCA9536 *_device) {
    uint8_t _data = 0x00;
    return PCA9536_write(_device, PCA9536_configRegister, &_data);
}

/**
 * @brief Switches whole port of the expander (first 2 or last 2 bits)
 * @param _port -> Port 0 = Gain-Setting, Port 1 = Input-Setting
 */
uint8_t PCA9536_switchPort(PCA9536 *_device, uint8_t _port) {

    //output data variable
    uint8_t _data = 0;

    /**
     * Port 0 -> switch values of pin 0 and 1
     * Port 1 -> switch values of pin 2 and 3
     */
    switch (_port) {
        case 0:
            _device->_port0 = !_device->_port0;
            break;
        case 1:
            _device->_port1 = !_device->_port1;
            break;
        default:
            break;
    }

    if(_device->_port0) {
        _data |= 0x01;
    } else {
        _data |= 0x02;
    }

    if(_device->_port1) {
        _data |= (0x01<<2);
    } else {
        _data |= (0x02<<2);
    }
    
    return PCA9536_write(_device, PCA9536_outputRegister, &_data);;
}