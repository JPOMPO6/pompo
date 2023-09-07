#include "app_error.h"
#include "app_util_platform.h"
#include "boards.h"
#include "nrf_delay.h"
#include "nrf_drv_twi.h"
#include <stdio.h>

#include "H3LIS331DL.h"
#include "LSM6DSOX.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

/* TWI instance ID. */
#define TWI_INSTANCE_ID 0

#define SR 104     // Sample rate. Options are: 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333 and 6667 Hz.
#define WTM_LV 199 // Watermark threshold level. Max samples in this FIFO configuration is 512 (accel and gyro only).

//LSM6DSOX_t lsm6dsoxSensor = LSM6DSOXSensor(, LSM6DSOX_I2C_ADD_L);
/* Indicates if operation on TWI has ended. */
static volatile bool m_xfer_done = false;

/* TWI instance. */
const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

/**
 * @brief TWI events handler.
 */
void twi_handler(nrf_drv_twi_evt_t const *p_event, void *p_context) {
  switch (p_event->type) {
  case NRF_DRV_TWI_EVT_DONE:
    if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX) {
      // handle data
    }
    m_xfer_done = true;
    break;
  default:
    break;
  }
}

/**
 * @brief TWI initialization.
 */
void twi_init(void) {
  ret_code_t err_code;

  const nrf_drv_twi_config_t twi_config = {
      .scl = ARDUINO_SCL_PIN,
      .sda = ARDUINO_SDA_PIN,
      .frequency = NRF_DRV_TWI_FREQ_100K,
      .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
      .clear_bus_init = false};

  err_code = nrf_drv_twi_init(&m_twi, &twi_config, twi_handler, NULL);
  APP_ERROR_CHECK(err_code);

  nrf_drv_twi_enable(&m_twi);
}

void H3LIS331DL_Init() {
  setAddr_H3LIS331DL(H3LIS331DL_DEFAULT_ADDRESS);

  H3LIS331DL_setAccelDataRate(ACCEL_DATARATE_50HZ);

  H3LIS331DL_setAccelRange(ACCEL_RANGE_100G);

  H3LIS331DL_begin(&m_twi);

  nrf_delay_ms(500);
}

void LSM6DSOXSENSOR_Init() {
  LSM6DSOX_begin();

  if (LSM6DSOX_Enable_G() == LSM6DSOX_OK && LSM6DSOX_Enable_X() == LSM6DSOX_OK) {
    printf("Success enabling accelero and gyro");
  } else {
    printf("Error enabling accelero and gyro");
    ASSERT(false);
  }

  uint8_t id;
  LSM6DSOX_ReadID(&id);
  if (id != LSM6DSOX_ID) {
    printf("Wrong id for LSM6DSOX sensor. Check that device is plugged");
    ASSERT(false);
  } else {
    printf("Success checking id for LSM6DSOX sensor");
  }

  LSM6DSOX_Set_X_FS(2);
  LSM6DSOX_Set_G_FS(250);
  LSM6DSOX_Set_X_ODR(SR);
  LSM6DSOX_Set_G_ODR(SR);
  LSM6DSOX_Set_FIFO_X_BDR(SR);
  LSM6DSOX_Set_FIFO_G_BDR(SR);
  LSM6DSOX_Set_FIFO_Mode(LSM6DSOX_BYPASS_MODE);
  LSM6DSOX_Set_FIFO_Mode(LSM6DSOX_STREAM_MODE);
  LSM6DSOX_Set_FIFO_Watermark_Level(WTM_LV);
  nrf_delay_ms(500);
}

/**
 * @brief Function for main application entry.
 */
int main(void) {
  printf("\r\nProgram started.");
  
  // INITIALIZE TWI(I2C) DRIVER
  twi_init();

  // INITIALIZE LSM6DSO SENSOR
  LSM6DSOXSENSOR_Init();

  // INITIALIZE H3LIS331DL SENSOR
  H3LIS331DL_Init();

  while (true) // MAIN TASK, INFINITE LOOP
  {
    nrf_delay_ms(500);

    do {
      __WFE();
    } while (m_xfer_done == false);

    // main task loop INSIDE here!

  }
}

/** @} */