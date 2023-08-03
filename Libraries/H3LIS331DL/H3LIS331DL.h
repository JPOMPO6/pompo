#ifndef _H3LIS331DL_H_
#define _H3LIS331DL_H_

#include <stdint.h>
#include <stdbool.h>
#include "nrf_drv_twi.h" // pca10040 nRF52832 device compatible

/**************************************************************************
    I2C ADDRESS/BITS
**************************************************************************/
    #define H3LIS331DL_DEFAULT_ADDRESS                      (0x18)      // 3C >> 1 = 7-bit default
    #define H3LIS331DL_ADDRESS_UPDATED                      (0x19)      // 3D >> 1 = 7-bit

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define H3LIS331DL_CONVERSIONDELAY                      (100)

/**************************************************************************
    ACCELEROMETER REGISTERS
**************************************************************************/
    #define H3LIS331DL_REG_ACCEL_WHO_AM_I                   (0x0F)      // Accelerometer Control Register 1
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1                  (0x20)      // Accelerometer Control Register 1
    #define H3LIS331DL_REG_ACCEL_CTRL_REG2                  (0x21)      // Accelerometer Control Register 2
    #define H3LIS331DL_REG_ACCEL_CTRL_REG3                  (0x22)      // Accelerometer Control Register 3
    #define H3LIS331DL_REG_ACCEL_CTRL_REG4                  (0x23)      // Accelerometer Control Register 4
    #define H3LIS331DL_REG_ACCEL_CTRL_REG5                  (0x24)      // Accelerometer Control Register 5
    #define H3LIS331DL_REG_ACCEL_HP_FILTER_RST              (0x25)      // Dummy Register
    #define H3LIS331DL_REG_ACCEL_REFERENCE                  (0x26)      // Reference/Datacapture Register
    #define H3LIS331DL_REG_ACCEL_STATUS                     (0x27)      // Status Register
    #define H3LIS331DL_REG_ACCEL_OUT_X_L                    (0x28)      // X-Axis Acceleration Data Low Register
    #define H3LIS331DL_REG_ACCEL_OUT_X_H                    (0x29)      // X-Axis Acceleration Data High Register
    #define H3LIS331DL_REG_ACCEL_OUT_Y_L                    (0x2A)      // Y-Axis Acceleration Data Low Register
    #define H3LIS331DL_REG_ACCEL_OUT_Y_H                    (0x2B)      // Y-Axis Acceleration Data High Register
    #define H3LIS331DL_REG_ACCEL_OUT_Z_L                    (0x2C)      // Z-Axis Acceleration Data Low Register
    #define H3LIS331DL_REG_ACCEL_OUT_Z_H                    (0x2D)      // Z-Axis Acceleration Data High Register
    #define H3LIS331DL_REG_ACCEL_INT1_CFG                   (0x30)      // Interrupt 1 Configuration Register
    #define H3LIS331DL_REG_ACCEL_INT1_SOURCE                (0x31)      // Interrupt 1 Source Register
    #define H3LIS331DL_REG_ACCEL_INT1_THS                   (0x32)      // Interrupt 1 Threshold Register
    #define H3LIS331DL_REG_ACCEL_INT1_DURATION              (0x33)      // Interrupt 1 Duration Register
    #define H3LIS331DL_REG_ACCEL_INT2_CFG                   (0x34)      // Interrupt 2 Configuration Register
    #define H3LIS331DL_REG_ACCEL_INT2_SOURCE                (0x35)      // Interrupt 2 Source Register
    #define H3LIS331DL_REG_ACCEL_INT2_THS                   (0x36)      // Interrupt 2 Threshold Register
    #define H3LIS331DL_REG_ACCEL_INT2_DURATION              (0x37)      // Interrupt 2 Duration Register

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 1 CONFIGURATION
**************************************************************************/
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_PM_MASK          (0xE0)      // Power mode and Low-Power Output Data Rate Configurations
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_PM_PWRDWN        (0x00)      // Power-Down Mode
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_PM_NORMAL        (0x20)      // Normal Mode, ODRLP = ODR
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_PM_LOW_0_5       (0x40)      // Low Power Mode, ODRLP: 0.5 Hz
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_PM_LOW_1         (0x60)      // Low Power Mode, ODRLP: 1 Hz
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_PM_LOW_2         (0x80)      // Low Power Mode, ODRLP: 2 Hz
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_PM_LOW_5         (0xA0)      // Low Power Mode, ODRLP: 5 Hz
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_PM_LOW_10        (0xC0)      // Low Power Mode, ODRLP: 10 Hz

    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_AODR_MASK        (0x18)      // Normal-Mode Output Data Rate Configurations and Low-Pass Cut-Off Frequencies
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_AODR_50          (0x00)      // ODR: 50 Hz, Low-pass Filter Cut-Off Frequency [Hz]: 37
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_AODR_100         (0x08)      // ODR: 100 Hz, Low-pass Filter Cut-Off Frequency [Hz]: 74
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_AODR_400         (0x10)      // ODR: 400 Hz, Low-pass Filter Cut-Off Frequency [Hz]: 292
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_AODR_1000        (0x18)      // ODR: 1000 Hz, Low-pass Filter Cut-Off Frequency [Hz]: 780

    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_AZEN_MASK        (0x04)      // Acceleration Z-Axis Enable
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_AZEN_DISABLE     (0x00)      // Acceleration Z-Axis Disabled
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_AZEN_ENABLE      (0x04)      // Acceleration Z-Axis Enabled

    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_AYEN_MASK        (0x02)      // Acceleration Y-Axis Enable
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_AYEN_DISABLE     (0x00)      // Acceleration Y-Axis Disabled
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_AYEN_ENABLE      (0x02)      // Acceleration Y-Axis Enabled

    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_AXEN_MASK        (0x01)      // Acceleration X-Axis Enable
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_AXEN_DISABLE     (0x00)      // Acceleration X-Axis Disabled
    #define H3LIS331DL_REG_ACCEL_CTRL_REG1_AXEN_ENABLE      (0x01)      // Acceleration X-Axis Enabled

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 4
**************************************************************************/
    #define H3LIS331DL_REG_ACCEL_CTRL_REG4_BDU_MASK         (0x80)      // Block Data Update
    #define H3LIS331DL_REG_ACCEL_CTRL_REG4_BDU_CONTINUOUS   (0x00)      // Continuous Update
    #define H3LIS331DL_REG_ACCEL_CTRL_REG4_BDU_NOTUPDATED   (0x80)      // Output Registers Not Updated until MSB and LSB Read

    #define H3LIS331DL_REG_ACCEL_CTRL_REG4_BLE_MASK         (0x40)      // Big/Little Endian Data Selection
    #define H3LIS331DL_REG_ACCEL_CTRL_REG4_BLE_LSB          (0x00)      // Data LSB @ lower address
    #define H3LIS331DL_REG_ACCEL_CTRL_REG4_BLE_MSB          (0x40)      // Data MSB @ lower address

    #define H3LIS331DL_REG_ACCEL_CTRL_REG4_FS_MASK          (0x30)      // Full-Scale Selection
    #define H3LIS331DL_REG_ACCEL_CTRL_REG4_FS_100G          (0x00)      // +/- 100G
    #define H3LIS331DL_REG_ACCEL_CTRL_REG4_FS_200G          (0x10)      // +/- 200G
    #define H3LIS331DL_REG_ACCEL_CTRL_REG4_FS_400G          (0x30)      // +/- 400G

    #define H3LIS331DL_REG_ACCEL_CTRL_REG4_SIM_MASK         (0x01)      // SPI Serial Interface Mode Selection
    #define H3LIS331DL_REG_ACCEL_CTRL_REG4_SIM_4WIRE        (0x00)      // 4-Wire Interface
    #define H3LIS331DL_REG_ACCEL_CTRL_REG4_SIM_3WIRE        (0x01)      // 3-Wire Interface


typedef enum
{
    POWER_MODE_DOWN                 = H3LIS331DL_REG_ACCEL_CTRL_REG1_PM_PWRDWN,
    POWER_MODE_NORMAL               = H3LIS331DL_REG_ACCEL_CTRL_REG1_PM_NORMAL,
    POWER_MODE_LOW_0_5              = H3LIS331DL_REG_ACCEL_CTRL_REG1_PM_LOW_0_5,
    POWER_MODE_LOW_1                = H3LIS331DL_REG_ACCEL_CTRL_REG1_PM_LOW_1,
    POWER_MODE_LOW_2                = H3LIS331DL_REG_ACCEL_CTRL_REG1_PM_LOW_2,
    POWER_MODE_LOW_5                = H3LIS331DL_REG_ACCEL_CTRL_REG1_PM_LOW_5,
    POWER_MODE_LOW_10               = H3LIS331DL_REG_ACCEL_CTRL_REG1_PM_LOW_10
    
} AccelerometerPowerMode;

typedef enum
{
    ACCEL_DATARATE_50HZ             = H3LIS331DL_REG_ACCEL_CTRL_REG1_AODR_50,
    ACCEL_DATARATE_100HZ            = H3LIS331DL_REG_ACCEL_CTRL_REG1_AODR_100,
    ACCEL_DATARATE_400HZ            = H3LIS331DL_REG_ACCEL_CTRL_REG1_AODR_400,
    ACCEL_DATARATE_1000HZ           = H3LIS331DL_REG_ACCEL_CTRL_REG1_AODR_1000
    
} AccelerometerDataRate;

typedef enum
{
    ACCEL_Z_AXIS_DISABLED           = H3LIS331DL_REG_ACCEL_CTRL_REG1_AZEN_DISABLE,
    ACCEL_Z_AXIS_ENABLED            = H3LIS331DL_REG_ACCEL_CTRL_REG1_AZEN_ENABLE
    
} AccelerometerEnableZ;

typedef enum
{
    ACCEL_Y_AXIS_DISABLED           = H3LIS331DL_REG_ACCEL_CTRL_REG1_AYEN_DISABLE,
    ACCEL_Y_AXIS_ENABLED            = H3LIS331DL_REG_ACCEL_CTRL_REG1_AYEN_ENABLE
    
} AccelerometerEnableY;

typedef enum
{
    ACCEL_X_AXIS_DISABLED           = H3LIS331DL_REG_ACCEL_CTRL_REG1_AXEN_DISABLE,
    ACCEL_X_AXIS_ENABLED            = H3LIS331DL_REG_ACCEL_CTRL_REG1_AXEN_ENABLE
    
} AccelerometerEnableX;

typedef enum
{
    ACCEL_CONTINUOUS                = H3LIS331DL_REG_ACCEL_CTRL_REG4_BDU_CONTINUOUS,
    ACCEL_NOT_UPDATED               = H3LIS331DL_REG_ACCEL_CTRL_REG4_BDU_NOTUPDATED
    
} AccelerometerBlockData;

typedef enum
{
    GACCEL_DATA_LSB                 = H3LIS331DL_REG_ACCEL_CTRL_REG4_BLE_LSB,
    ACCEL_DATA_MSB                  = H3LIS331DL_REG_ACCEL_CTRL_REG4_BLE_MSB
    
} AccelerometerEndianData;

typedef enum
{
    ACCEL_RANGE_100G                  = H3LIS331DL_REG_ACCEL_CTRL_REG4_FS_100G,
    ACCEL_RANGE_200G                 = H3LIS331DL_REG_ACCEL_CTRL_REG4_FS_200G,
    ACCEL_RANGE_400G                 = H3LIS331DL_REG_ACCEL_CTRL_REG4_FS_400G
    
} AccelerometerRange;

typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
} AccelerometerData;

typedef struct H3LIS331DL_s
{
    // Instance-specific properties
    uint8_t conversionDelay;
    AccelerometerPowerMode powerMode;
    AccelerometerDataRate dataRate;
    AccelerometerEnableZ enableZ;
    AccelerometerEnableY enableY;
    AccelerometerEnableX enableX;
    AccelerometerBlockData blockDataUpdate;
    AccelerometerEndianData endianData;
    AccelerometerRange accelRange;
    
    const nrf_drv_twi_t *m_twi;

    uint8_t i2cAddress;
    AccelerometerData accelData;
} H3LIS331DL_t;

static void H3LIS331DL_writeRegister(uint8_t reg, uint8_t value);
static uint8_t H3LIS331DL_readRegister(uint8_t reg);

H3LIS331DL_t setAddr_H3LIS331DL(uint8_t i2cAddress);
bool H3LIS331DL_begin(const nrf_drv_twi_t *m_twi);
void H3LIS331DL_setupAccelerometer(void);
void H3LIS331DL_Measure_Accelerometer(void);

void H3LIS331DL_setAccelPowerMode(AccelerometerPowerMode powerMode);
AccelerometerPowerMode H3LIS331DL_getAccelPowerMode(void);

void H3LIS331DL_setAccelDataRate(AccelerometerDataRate dataRate);
AccelerometerDataRate H3LIS331DL_getAccelDataRate(void);

void H3LIS331DL_setAccelZen(AccelerometerEnableZ enableZ);
AccelerometerEnableZ H3LIS331DL_getAccelZen(void);

void H3LIS331DL_setAccelYen(AccelerometerEnableY enableY);
AccelerometerEnableY H3LIS331DL_getAccelYen(void);

void H3LIS331DL_setAccelXen(AccelerometerEnableX enableX);
AccelerometerEnableX H3LIS331DL_getAccelXen(void);

void H3LIS331DL_setAccelBlockData(AccelerometerBlockData blockDataUpdate);
AccelerometerBlockData H3LIS331DL_getAccelBlockData(void);

void H3LIS331DL_setAccelEndianData(AccelerometerEndianData endianData);
AccelerometerEndianData H3LIS331DL_getAccelEndianData(void);

void H3LIS331DL_setAccelRange(AccelerometerRange accelRange);
AccelerometerRange H3LIS331DL_getAccelRange(void);

#endif /*_H3LIS331DL_H_*/