/*!
 *  @file Adafruit_BNO08x.h
 *
 * 	I2C Driver for the Adafruit BNO08x 9-DOF Orientation IMU Fusion Breakout
 *
 * 	This is a library for the Adafruit BNO08x breakout:
 * 	https://www.adafruit.com/products/4754
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *
 *	BSD license (see license.txt)
 */

#ifndef _ADAFRUIT_BNO08x_H
#define _ADAFRUIT_BNO08x_H


#include "sh2.h"
#include "sh2_SensorValue.h"
#include "sh2_err.h"
#include "main.h"
#include "utility.h"



#define BNO08x_I2CADDR_DEFAULT 0x4A ///< The default I2C address

/* Additional Activities not listed in SH-2 lib */
#define PAC_ON_STAIRS 8 ///< Activity code for being on stairs
#define PAC_OPTION_COUNT  9 ///< The number of current options for the activity classifier

/*!
 *    @brief  Class that stores state and functions for interacting with
 *            the BNO08x 9-DOF Orientation IMU Fusion Breakout
 */
typedef struct  {

  sh2_ProductIds_t prodIds;
  sh2_Hal_t _HAL;
} Adafruit_BNO08x;


void Adafruit_BNO08x_construct(Adafruit_BNO08x* self, int8_t reset_pin);
uint8_t Adafruit_BNO08x_init(Adafruit_BNO08x* self, int32_t sensor_id);

uint8_t Adafruit_BNO08x_begin_I2C(Adafruit_BNO08x* self, int32_t sensor_id);

void Adafruit_BNO08x_hardwareReset(Adafruit_BNO08x* self );
uint8_t Adafruit_BNO08x_wasReset(Adafruit_BNO08x* self );

uint8_t Adafruit_BNO08x_enableReport(Adafruit_BNO08x* self, sh2_SensorId_t sensor, uint32_t interval_us);
uint8_t Adafruit_BNO08x_getSensorEvent(Adafruit_BNO08x* self, sh2_SensorValue_t *value);

static int i2chal_write(sh2_Hal_t *self, uint8_t *pBuffer, unsigned len);
static int i2chal_read(sh2_Hal_t *self, uint8_t *pBuffer, unsigned len,
                       uint32_t *t_us);
static void i2chal_close(sh2_Hal_t *self);
static int i2chal_open(sh2_Hal_t *self);

static uint32_t hal_getTimeUs(sh2_Hal_t *self);
static void hal_callback(void *cookie, sh2_AsyncEvent_t *pEvent);
static void sensorHandler(void *cookie, sh2_SensorEvent_t *pEvent);
static void hal_hardwareReset(void);

#endif