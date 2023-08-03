#include "LSM6DSOX.h"

LSM6DSOX_t lsm6dso;

/**
 * @brief Utility function to read data.
 * @param  pBuffer: pointer to data to be read.
 * @param  RegisterAddr: specifies internal address register to be read.
 * @param  NumByteToRead: number of bytes to be read.
 * @retval 0 if ok, an error code otherwise.
 */
static uint8_t IO_Read(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToRead)
{             
  if (lsm6dso.m_twi) {
    //dev_i2c->beginTransmission(((uint8_t)(((address) >> 1) & 0x7F)));
    //dev_i2c->write(RegisterAddr);
    //dev_i2c->endTransmission(false);

    //dev_i2c->requestFrom(((uint8_t)(((address) >> 1) & 0x7F)), (uint8_t) NumByteToRead);

    //int i=0;
    //while (dev_i2c->available()) {
    //  pBuffer[i] = dev_i2c->read();
    //  i++;
    //}

    return 0;
  }

  return 1;
}

/**
 * @brief Utility function to write data.
 * @param  pBuffer: pointer to data to be written.
 * @param  RegisterAddr: specifies internal address register to be written.
 * @param  NumByteToWrite: number of bytes to write.
 * @retval 0 if ok, an error code otherwise.
 */
static uint8_t IO_Write(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToWrite)
{  
  if (lsm6dso.m_twi) {
    //dev_i2c->beginTransmission(((uint8_t)(((address) >> 1) & 0x7F)));

    //dev_i2c->write(RegisterAddr);
    //for (uint16_t i = 0 ; i < NumByteToWrite ; i++) {
    //  dev_i2c->write(pBuffer[i]);
    //}

    //dev_i2c->endTransmission(true);

    return 0;
  }
  return 1;
}