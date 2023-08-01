#include "H3LIS331DL.h"


H3LIS331DL_t h3lis;

/**************************************************************************/
/*
        Instantiates a new H3LIS331DL object with appropriate properties
*/
/**************************************************************************/
H3LIS331DL_t setAddr_H3LIS331DL(uint8_t i2cAddress)
{
    h3lis.i2cAddress = i2cAddress;
    h3lis.conversionDelay = H3LIS331DL_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware, it requires a preinitialized TWI(I2C) driver object
*/
/**************************************************************************/
bool H3LIS331DL_begin(nrf_drv_twi_t m_twi)
{
    // Wire.begin(); will be changed

    h3lis.m_twi = m_twi;
    
    if (readRegister(h3lis.i2cAddress, H3LIS331DL_REG_ACCEL_WHO_AM_I) != 0x32) return false;
    
    // Set up the sensor for Accelerometer
    setUpAccelerometer();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Power mode and Low-Power Output Data Rate Configurations
*/
/**************************************************************************/
void setAccelPowerMode(AccelerometerPowerMode powerMode)
{
    h3lis.powerMode = powerMode;
}

/**************************************************************************/
/*
        Gets the Power Mode and Low-power Output Data Rate Configurations
*/
/**************************************************************************/
AccelerometerPowerMode getAccelPowerMode()
{
    return h3lis.powerMode;
}

/**************************************************************************/
/*
        Sets the Normal-Mode Output Data Rate Configurations and Low-Pass Cut-Off Frequencies
*/
/**************************************************************************/
void setAccelDataRate(AccelerometerDataRate dataRate)
{
    h3lis.dataRate = dataRate;
}
                    
/**************************************************************************/
/*
        Gets the Normal-Mode Output Data Rate Configurations and Low-Pass Cut-Off Frequencies
*/
/**************************************************************************/
AccelerometerDataRate getAccelDataRate()
{
    return h3lis.dataRate;
}

/**************************************************************************/
/*
        Sets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
void setAccelZen(AccelerometerEnableZ enableZ)
{
    h3lis.enableZ = enableZ;
}

/**************************************************************************/
/*
        Gets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
AccelerometerEnableZ getAccelZen()
{
    return h3lis.enableZ;
}

/**************************************************************************/
/*
        Sets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
void setAccelYen(AccelerometerEnableY enableY)
{
    h3lis.enableY = enableY;
}

/**************************************************************************/
/*
        Gets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
AccelerometerEnableY getAccelYen()
{
    return h3lis.enableY;
}

/**************************************************************************/
/*
        Sets the Acceleration X-Axis Enable
*/
/**************************************************************************/
void setAccelXen(AccelerometerEnableX enableX)
{
    h3lis.enableX = enableX;
}

/**************************************************************************/
/*
        Gets the Acceleration X-Axis Enable
*/
/**************************************************************************/
AccelerometerEnableX getAccelXen()
{
    return h3lis.enableX;
}

/**************************************************************************/
/*
        Sets the Block Data Update for Acceleration Data
*/
/**************************************************************************/
void setAccelBlockData(AccelerometerBlockData blockDataUpdate)
{
    h3lis.blockDataUpdate = blockDataUpdate;
}

/**************************************************************************/
/*
        Gets the Block Data Update for Acceleration Data
*/
/**************************************************************************/
AccelerometerBlockData getAccelBlockData()
{
    return h3lis.blockDataUpdate;
}

/**************************************************************************/
/*
        Sets the Big/Little Endian Data Selection
*/
/**************************************************************************/
void setAccelEndianData(AccelerometerEndianData endianData)
{
    h3lis.endianData = endianData;
}

/**************************************************************************/
/*
        Gets the Big/Little Endian Data Selection
*/
/**************************************************************************/
AccelerometerEndianData getAccelEndianData()
{
    return h3lis.endianData;
}

/**************************************************************************/
/*
        Sets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
void setAccelRange(AccelerometerRange accelRange)
{
    h3lis.accelRange = accelRange;
}
                    
/**************************************************************************/
/*
        Gets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
AccelerometerRange getAccelRange()
{
    return h3lis.accelRange;
}

/**************************************************************************/
/*
        Sets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
void setAccelSPIWire(AccelerometerSPIWire spiWire)
{
    h3lis.spiWire = spiWire;
}

/**************************************************************************/
/*
        Gets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
AccelerometerSPIWire getAccelSPIWire()
{
    return h3lis.spiWire;
}

/**************************************************************************/
/*
        Sets up the Accelerometer
*/
/**************************************************************************/
void setUpAccelerometer(void)
{
    // Set Up the Configuration for the Accelerometer Control Register 1
    /*
    // Set the Power mode and Low-Power Output Data Rate Configurations
    uint8_t config1 = h3lis_accelpowermode;
     
    // Set the Acceleration Z-Axis Enable
    config1 |= h3lis_accelzen;
     
    // Set the Acceleration Y-Axis Enable
    config1 |= h3lis_accelyen;
     
    // Set the Acceleration X-Axis Enable
    config1 |= h3lis_accelxen;
    */
    
    uint8_t config1 =   H3LIS331DL_REG_ACCEL_CTRL_REG1_PM_NORMAL          |   // Normal Mode
                        H3LIS331DL_REG_ACCEL_CTRL_REG1_AZEN_ENABLE        |   // Acceleration Z-Axis Enabled
                        H3LIS331DL_REG_ACCEL_CTRL_REG1_AYEN_ENABLE        |   // Acceleration Y-Axis Enabled
                        H3LIS331DL_REG_ACCEL_CTRL_REG1_AXEN_ENABLE;           // Acceleration X-Axis Enabled
    
    // Set the Acceleration Data Rate Configuration
    config1 |= h3lis.dataRate;
        
    // Write the configuration to the Accelerometer Control Register 1
    writeRegister(h3lis.i2cAddress, H3LIS331DL_REG_ACCEL_CTRL_REG1, config1);
    
    // Wait for the configuration to complete
    delay(h3lis.conversionDelay);
    
    // Set Up the Configuration for the Accelerometer Control Register 4
    /*
    // Set the Accelerometer Block Data Update
    uint8_t config4 = h3lis_accelblockdata;
     
    // Set the Big/Little Endian Data Selection
    config4 |= h3lis_accelendiandata;
     
    // Set the SPI Serial Interface Mode Selection
    config4 |= h3lis_accelmagspiwire;
    */
    
    uint8_t config4 =   H3LIS331DL_REG_ACCEL_CTRL_REG4_BDU_CONTINUOUS     |   // Continuous Update
                        H3LIS331DL_REG_ACCEL_CTRL_REG4_BLE_LSB            |   // Data LSB @ lower address
                        H3LIS331DL_REG_ACCEL_CTRL_REG4_SIM_4WIRE;             // 4-Wire Interface
    
    // Set the Acceleration Full-Scale Selection
    config4 |= h3lis.accelRange;
        
    // Write the configuration to the Accelerometer Control Register 4
    writeRegister(h3lis.i2cAddress, H3LIS331DL_REG_ACCEL_CTRL_REG4, config4);
    
    // Wait for the configuration to complete
    delay(h3lis.conversionDelay);
}

/**************************************************************************/
/*
        Reads the results for the Accelerometer H3LIS331DL
*/
/**************************************************************************/
void Measure_Accelerometer(void)
{
    // Read the Accelerometer
    uint8_t xAccelLo, xAccelHi, yAccelLo, yAccelHi, zAccelLo, zAccelHi;
    
    // Read the Data
    // Reading the Low X-Axis Acceleration Data Register
    xAccelLo = readRegister(h3lis.i2cAddress, H3LIS331DL_REG_ACCEL_OUT_X_L);
    // Reading the High X-Axis Acceleration Data Register
    xAccelHi = readRegister(h3lis.i2cAddress, H3LIS331DL_REG_ACCEL_OUT_X_H);
    // Conversion of the result
    // 16-bit signed result for X-Axis Acceleration Data of H3LIS331DL
    h3lis.accelData.X = (int16_t)((xAccelHi << 8) | xAccelLo);
    
    // Reading the Low Y-Axis Acceleration Data Register
    yAccelLo = readRegister(h3lis.i2cAddress, H3LIS331DL_REG_ACCEL_OUT_Y_L);
    // Reading the High Y-Axis Acceleration Data Register
    yAccelHi = readRegister(h3lis.i2cAddress, H3LIS331DL_REG_ACCEL_OUT_Y_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Acceleration Data of H3LIS331DL
    h3lis.accelData.Y = (int16_t)((yAccelHi << 8) | yAccelLo);
    
    // Reading the Low Z-Axis Acceleration Data Register
    zAccelLo = readRegister(h3lis.i2cAddress, H3LIS331DL_REG_ACCEL_OUT_Z_L);
    // Reading the High Z-Axis Acceleration Data Register
    zAccelHi = readRegister(h3lis.i2cAddress, H3LIS331DL_REG_ACCEL_OUT_Z_H);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Acceleration Data of H3LIS331DL
    h3lis.accelData.Z = (int16_t)((zAccelHi << 8) | zAccelLo);
}