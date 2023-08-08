#ifndef _LSM6DSOX_H_
#define _LSM6DSOX_H_

#include <stdint.h>
#include "nrf_drv_twi.h" // pca10040 nRF52832 device compatible
#include "lsm6dsox_reg.h"

#define LSM6DSOX_ACC_SENSITIVITY_FS_2G   0.061f
#define LSM6DSOX_ACC_SENSITIVITY_FS_4G   0.122f
#define LSM6DSOX_ACC_SENSITIVITY_FS_8G   0.244f
#define LSM6DSOX_ACC_SENSITIVITY_FS_16G  0.488f

#define LSM6DSOX_GYRO_SENSITIVITY_FS_125DPS    4.375f
#define LSM6DSOX_GYRO_SENSITIVITY_FS_250DPS    8.750f
#define LSM6DSOX_GYRO_SENSITIVITY_FS_500DPS   17.500f
#define LSM6DSOX_GYRO_SENSITIVITY_FS_1000DPS  35.000f
#define LSM6DSOX_GYRO_SENSITIVITY_FS_2000DPS  70.000f

typedef enum
{
  LSM6DSOX_OK = 0,
  LSM6DSOX_ERROR =-1
} LSM6DSOXStatusTypeDef;

typedef enum
{
  LSM6DSOX_INT1_PIN,
  LSM6DSOX_INT2_PIN,
} LSM6DSOX_SensorIntPin_t;

typedef enum
{
  LSM6DSOX_ACC_HIGH_PERFORMANCE_MODE,
  LSM6DSOX_ACC_LOW_POWER_NORMAL_MODE,
  LSM6DSOX_ACC_ULTRA_LOW_POWER_MODE
} LSM6DSOX_ACC_Operating_Mode_t;

typedef enum
{
  LSM6DSOX_GYRO_HIGH_PERFORMANCE_MODE,
  LSM6DSOX_GYRO_LOW_POWER_NORMAL_MODE
} LSM6DSOX_GYRO_Operating_Mode_t;

typedef struct
{
  unsigned int FreeFallStatus : 1;
  unsigned int TapStatus : 1;
  unsigned int DoubleTapStatus : 1;
  unsigned int WakeUpStatus : 1;
  unsigned int StepStatus : 1;
  unsigned int TiltStatus : 1;
  unsigned int D6DOrientationStatus : 1;
  unsigned int SleepStatus : 1;
} LSM6DSOX_Event_Status_t;

typedef struct {
  unsigned int is_mlc1 : 1;
  unsigned int is_mlc2 : 1;
  unsigned int is_mlc3 : 1;
  unsigned int is_mlc4 : 1;
  unsigned int is_mlc5 : 1;
  unsigned int is_mlc6 : 1;
  unsigned int is_mlc7 : 1;
  unsigned int is_mlc8 : 1;
} LSM6DSOX_MLC_Status_t;

typedef struct {
  // Instance-specific properties
  uint8_t i2cAddress;
  const nrf_drv_twi_t *m_twi;

  lsm6dsox_odr_xl_t acc_odr;
  lsm6dsox_odr_g_t gyro_odr;

  uint8_t acc_is_enabled;
  uint8_t gyro_is_enabled;

  lsm6dsox_ctx_t reg_ctx;

}LSM6DSOX_t;

LSM6DSOXStatusTypeDef LSM6DSOXSensor_Init(const nrf_drv_twi_t *m_twi, uint8_t address); // address=LSM6DSOX_I2C_ADD_H as default
LSM6DSOXStatusTypeDef LSM6DSOX_begin();
LSM6DSOXStatusTypeDef LSM6DSOX_end();
LSM6DSOXStatusTypeDef LSM6DSOX_ReadID(uint8_t *Id);
LSM6DSOXStatusTypeDef LSM6DSOX_Enable_X();
LSM6DSOXStatusTypeDef LSM6DSOX_Disable_X();
LSM6DSOXStatusTypeDef LSM6DSOX_Get_X_Sensitivity(float *Sensitivity);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_X_ODR(float *Odr);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_X_ODR(float Odr);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_X_ODR_With_Mode(float Odr, LSM6DSOX_ACC_Operating_Mode_t Mode);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_X_FS(int32_t *FullScale);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_X_FS(int32_t FullScale);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_X_AxesRaw(int16_t *Value);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_X_Axes(int32_t *Acceleration);

LSM6DSOXStatusTypeDef LSM6DSOX_Enable_G();
LSM6DSOXStatusTypeDef LSM6DSOX_Disable_G();
LSM6DSOXStatusTypeDef LSM6DSOX_Get_G_Sensitivity(float *Sensitivity);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_G_ODR(float *Odr);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_G_ODR(float Odr);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_G_ODR_With_Mode(float Odr, LSM6DSOX_GYRO_Operating_Mode_t Mode);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_G_FS(int32_t *FullScale);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_G_FS(int32_t FullScale);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_G_AxesRaw(int16_t *Value);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_G_Axes(int32_t *AngularRate);

LSM6DSOXStatusTypeDef LSM6DSOX_Read_Reg(uint8_t reg, uint8_t *Data);
LSM6DSOXStatusTypeDef LSM6DSOX_Write_Reg(uint8_t reg, uint8_t Data);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_Interrupt_Latch(uint8_t Status);

LSM6DSOXStatusTypeDef LSM6DSOX_Enable_Free_Fall_Detection(LSM6DSOX_SensorIntPin_t IntPin);
LSM6DSOXStatusTypeDef LSM6DSOX_Disable_Free_Fall_Detection();
LSM6DSOXStatusTypeDef LSM6DSOX_Set_Free_Fall_Threshold(uint8_t Threshold);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_Free_Fall_Duration(uint8_t Duration);

LSM6DSOXStatusTypeDef LSM6DSOX_Enable_Pedometer();
LSM6DSOXStatusTypeDef LSM6DSOX_Disable_Pedometer();
LSM6DSOXStatusTypeDef LSM6DSOX_Get_Step_Count(uint16_t *StepCount);
LSM6DSOXStatusTypeDef LSM6DSOX_Step_Counter_Reset();

LSM6DSOXStatusTypeDef LSM6DSOX_Enable_Tilt_Detection(LSM6DSOX_SensorIntPin_t IntPin);
LSM6DSOXStatusTypeDef LSM6DSOX_Disable_Tilt_Detection();

LSM6DSOXStatusTypeDef LSM6DSOX_Enable_Wake_Up_Detection(LSM6DSOX_SensorIntPin_t IntPin);
LSM6DSOXStatusTypeDef LSM6DSOX_Disable_Wake_Up_Detection();
LSM6DSOXStatusTypeDef LSM6DSOX_Set_Wake_Up_Threshold(uint8_t Threshold);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_Wake_Up_Duration(uint8_t Duration);

LSM6DSOXStatusTypeDef LSM6DSOX_Enable_Single_Tap_Detection(LSM6DSOX_SensorIntPin_t IntPin);
LSM6DSOXStatusTypeDef LSM6DSOX_Disable_Single_Tap_Detection();
LSM6DSOXStatusTypeDef LSM6DSOX_Enable_Double_Tap_Detection(LSM6DSOX_SensorIntPin_t IntPin);
LSM6DSOXStatusTypeDef LSM6DSOX_Disable_Double_Tap_Detection();
LSM6DSOXStatusTypeDef LSM6DSOX_Set_Tap_Threshold(uint8_t Threshold);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_Tap_Shock_Time(uint8_t Time);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_Tap_Quiet_Time(uint8_t Time);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_Tap_Duration_Time(uint8_t Time);

LSM6DSOXStatusTypeDef LSM6DSOX_Enable_6D_Orientation(LSM6DSOX_SensorIntPin_t IntPin);
LSM6DSOXStatusTypeDef LSM6DSOX_Disable_6D_Orientation();
LSM6DSOXStatusTypeDef LSM6DSOX_Set_6D_Orientation_Threshold(uint8_t Threshold);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_6D_Orientation_XL(uint8_t *XLow);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_6D_Orientation_XH(uint8_t *XHigh);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_6D_Orientation_YL(uint8_t *YLow);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_6D_Orientation_YH(uint8_t *YHigh);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_6D_Orientation_ZL(uint8_t *ZLow);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_6D_Orientation_ZH(uint8_t *ZHigh);

LSM6DSOXStatusTypeDef LSM6DSOX_Get_X_DRDY_Status(uint8_t *Status);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_X_Event_Status(LSM6DSOX_Event_Status_t *Status);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_X_SelfTest(uint8_t Status);

LSM6DSOXStatusTypeDef LSM6DSOX_Get_G_DRDY_Status(uint8_t *Status);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_G_SelfTest(uint8_t Status);

LSM6DSOXStatusTypeDef LSM6DSOX_Get_FIFO_Num_Samples(uint16_t *NumSamples);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_FIFO_Full_Status(uint8_t *Status);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_FIFO_Overrun_Status(uint8_t *Status);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_FIFO_Watermark_Status(uint8_t *Status);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_FIFO_INT1_FIFO_Full(uint8_t Status);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_FIFO_INT1_FIFO_Overrun(uint8_t Status);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_FIFO_INT1_FIFO_Threshold(uint8_t Status);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_FIFO_INT2_FIFO_Full(uint8_t Status);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_FIFO_INT2_FIFO_Overrun(uint8_t Status);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_FIFO_INT2_FIFO_Threshold(uint8_t Status);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_FIFO_Watermark_Level(uint16_t Watermark);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_FIFO_Stop_On_Fth(uint8_t Status);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_FIFO_Mode(uint8_t Mode);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_FIFO_Tag(uint8_t *Tag);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_FIFO_Data(uint8_t *Data);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_FIFO_Sample(uint8_t *Sample, uint16_t Count); // Count=1 as default
LSM6DSOXStatusTypeDef LSM6DSOX_Get_FIFO_X_Axes(int32_t *Acceleration);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_FIFO_X_BDR(float Bdr);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_FIFO_G_Axes(int32_t *AngularVelocity);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_FIFO_G_BDR(float Bdr);

LSM6DSOXStatusTypeDef LSM6DSOX_Get_MLC_Status(LSM6DSOX_MLC_Status_t *Status);
LSM6DSOXStatusTypeDef LSM6DSOX_Get_MLC_Output(uint8_t *Output);

LSM6DSOXStatusTypeDef LSM6DSOX_Get_Timestamp_Status(uint8_t *Status);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_Timestamp_Status(uint8_t Status);

LSM6DSOXStatusTypeDef LSM6DSOX_Set_FIFO_Timestamp_Decimation(uint8_t Decimation);

LSM6DSOXStatusTypeDef LSM6DSOX_Set_FIFO_Compression_Algo_Init(uint8_t Status);

LSM6DSOXStatusTypeDef LSM6DSOX_Set_FIFO_Compression_Algo_Enable(uint8_t Status);

LSM6DSOXStatusTypeDef LSM6DSOX_Set_FIFO_Compression_Algo_Set(uint8_t Compression);

LSM6DSOXStatusTypeDef LSM6DSOX_Set_FIFO_Compression_Algo_Real_Time_Set(uint8_t Status);

LSM6DSOXStatusTypeDef LSM6DSOX_Set_X_ODR_When_Enabled(float Odr);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_X_ODR_When_Disabled(float Odr);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_G_ODR_When_Enabled(float Odr);
LSM6DSOXStatusTypeDef LSM6DSOX_Set_G_ODR_When_Disabled(float Odr);

static uint8_t LSM6DSOX_IO_Read(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToRead);
static uint8_t LSM6DSOX_IO_Write(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToWrite);

int32_t LSM6DSOX_io_write(uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite );
int32_t LSM6DSOX_io_read(uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead );

#endif /*_LSM6DSOX_H_*/