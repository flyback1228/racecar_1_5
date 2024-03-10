/*!
 *  @file Adafruit_BNO08x.cpp
 *
 *  @mainpage Adafruit BNO08x 9-DOF Orientation IMU Fusion Breakout
 *
 *  @section intro_sec Introduction
 *
 * 	I2C Driver for the Library for the BNO08x 9-DOF Orientation IMU Fusion
 * Breakout
 *
 * 	This is a library for the Adafruit BNO08x breakout:
 * 	https://www.adafruit.com/product/4754
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *  @section dependencies Dependencies
 *  This library depends on the Adafruit BusIO library
 *
 *  This library depends on the Adafruit Unified Sensor library
 *
 *  @section author Author
 *
 *  Bryan Siepert for Adafruit Industries
 *
 * 	@section license License
 *
 * 	BSD (see license.txt)
 *
 * 	@section  HISTORY
 *
 *     v1.0 - First release
 */


#include "Adafruit_BNO08x.h"

extern I2C_HandleTypeDef hi2c2;

#define _int_pin BNO_INT_Pin
#define _int_port BNO_INT_GPIO_Port

#define _reset_pin BNO_NRST_Pin
#define _reset_port BNO_NRST_GPIO_Port

//static int8_t _int_pin, _reset_pin;

//static Adafruit_I2CDevice *i2c_dev = NULL; ///< Pointer to I2C bus interface

static sh2_SensorValue_t *_sensor_value = NULL;
static uint8_t _reset_occurred = 0;

/**
 * @brief Construct a new Adafruit_BNO08x::Adafruit_BNO08x object
 *
 */

/**
 * @brief Construct a new Adafruit_BNO08x::Adafruit_BNO08x object
 *
 * @param reset_pin The arduino pin # connected to the BNO Reset pin
 */
Adafruit_BNO08x_construct(Adafruit_BNO08x* self)
{
	}


/*!
 *    @brief  Sets up the hardware and initializes I2C
 *    @param  i2c_address
 *            The I2C address to be used.
 *    @param  wire
 *            The Wire object to be used for I2C connections.
 *    @param  sensor_id
 *            The unique ID to differentiate the sensors from others
 *    @return True if initialization was successful, otherwise false.
 */
uint8_t Adafruit_BNO08x_begin_I2C(Adafruit_BNO08x* self, int32_t sensor_id) {


	/*
  if (!i2c_dev->begin()) {
    printf("I2C address not found");
    return false;
  }*/


  self->_HAL.open = i2chal_open;
  self->_HAL.close = i2chal_close;
  self->_HAL.read = i2chal_read;
  self->_HAL.write = i2chal_write;
  self->_HAL.getTimeUs = hal_getTimeUs;

  return Adafruit_BNO08x_init(self,sensor_id);
}



/*!  @brief Initializer for post i2c/spi init
 *   @param sensor_id Optional unique ID for the sensor set
 *   @returns True if chip identified and initialized
 */
uint8_t Adafruit_BNO08x_init(Adafruit_BNO08x* self, int32_t sensor_id) {
  int status;

  Adafruit_BNO08x_hardwareReset(self);

  // Open SH2 interface (also registers non-sensor event handler.)
  status = sh2_open(&(self->_HAL), hal_callback, NULL);
  if (status != SH2_OK) {
    return false;
  }

  // Check connection partially by getting the product id's
  memset(&(self->prodIds), 0, sizeof(self->prodIds));
  status = sh2_getProdIds(&self->prodIds);
  if (status != SH2_OK) {
    return false;
  }

  // Register sensor listener
  sh2_setSensorCallback(sensorHandler, NULL);

  return true;
}

/**
 * @brief Reset the device using the Reset pin
 *
 */
void Adafruit_BNO08x_hardwareReset(Adafruit_BNO08x* self) { hal_hardwareReset(); }

/**
 * @brief Check if a reset has occured
 *
 * @return true: a reset has occured false: no reset has occoured
 */
uint8_t Adafruit_BNO08x_wasReset(Adafruit_BNO08x* self) {
  uint8_t x = _reset_occurred;
  _reset_occurred = 0;

  return x;
}

/**
 * @brief Fill the given sensor value object with a new report
 *
 * @param value Pointer to an sh2_SensorValue_t struct to fil
 * @return true: The report object was filled with a new report
 * @return false: No new report available to fill
 */
uint8_t Adafruit_BNO08x_getSensorEvent(Adafruit_BNO08x* self, sh2_SensorValue_t *value) {
  _sensor_value = value;

  value->timestamp = 0;

  sh2_service();

  if (value->timestamp == 0 && value->sensorId != SH2_GYRO_INTEGRATED_RV) {
    // no new events
    return 0;
  }

  return 1;
}

/**
 * @brief Enable the given report type
 *
 * @param sensorId The report ID to enable
 * @param interval_us The update interval for reports to be generated, in
 * milliseconds
 * @return true: success false: failure
 */
uint8_t Adafruit_BNO08x_enableReport(Adafruit_BNO08x* self, sh2_SensorId_t sensorId, uint32_t interval_us) {
  static sh2_SensorConfig_t config;

  // These sensor options are disabled or not used in most cases
  config.changeSensitivityEnabled = false;
  config.wakeupEnabled = false;
  config.changeSensitivityRelative = false;
  config.alwaysOnEnabled = false;
  config.changeSensitivity = 0;
  config.batchInterval_us = 0;
  config.sensorSpecific = 0;

  config.reportInterval_us = interval_us;
  int status = sh2_setSensorConfig(sensorId, &config);

  if (status != SH2_OK) {
    return false;
  }

  return true;
}

/**************************************** I2C interface
 * ***********************************************************/

static int i2chal_open(sh2_Hal_t *self) {
  // Serial.println("I2C HAL open");

  // send a software reset
  uint8_t softreset_pkt[] = {5, 0, 1, 0, 1};
  // Serial.println("Sending softreset");

  if (HAL_I2C_Master_Transmit(&hi2c2, BNO08x_I2CADDR_DEFAULT<<1, softreset_pkt, 5, 10)!=HAL_OK) {
    return -1;
  }
  // Serial.println("OK!");
  HAL_Delay(100);

  if (HAL_I2C_Master_Transmit(&hi2c2, BNO08x_I2CADDR_DEFAULT<<1, softreset_pkt, 5, 10)!=HAL_OK) {
    return -1;
  }
  // Serial.println("OK!");
  HAL_Delay(100);

  return 0;
}

static void i2chal_close(sh2_Hal_t *self) {
  // Serial.println("I2C HAL close");
}

static int i2chal_read(sh2_Hal_t *self, uint8_t *pBuffer, unsigned len,
                       uint32_t *t_us) {
  // Serial.println("I2C HAL read");

  uint8_t *pBufferOrig = pBuffer;

  uint8_t header[4];
  if (HAL_I2C_Master_Receive(&hi2c2, BNO08x_I2CADDR_DEFAULT<<1, header, 4, 10)!=HAL_OK) {
    return 0;
  }

  // Determine amount to read
  uint16_t packet_size = (uint16_t)header[0] | (uint16_t)header[1] << 8;
  // Unset the "continue" bit
  packet_size &= ~0x8000;

  /*
  Serial.print("Read SHTP header. ");
  Serial.print("Packet size: ");
  Serial.print(packet_size);
  Serial.print(" & buffer size: ");
  Serial.println(len);
  */

  if (HAL_I2C_Master_Receive(&hi2c2, BNO08x_I2CADDR_DEFAULT<<1, pBuffer, packet_size, 10)!=HAL_OK) {
	  return 0;
	}
  /*
  for (int i=0; i<packet_size; i++) {
    Serial.print(pBufferOrig[i], HEX);
    Serial.print(", ");
    if (i % 16 == 15) Serial.println();
  }
  Serial.println();
  */

  return packet_size;
}

static int i2chal_write(sh2_Hal_t *self, uint8_t *pBuffer, unsigned len) {

  if (HAL_I2C_Master_Transmit(&hi2c2, BNO08x_I2CADDR_DEFAULT<<1, pBuffer, len, 10)!=HAL_OK) {
    return 0;
  }

  return len;
}



/**************************************** HAL interface
 * ***********************************************************/

static void hal_hardwareReset(void) {

    // Serial.println("BNO08x Hardware reset");

	  HAL_GPIO_WritePin(_reset_port,_reset_pin,GPIO_PIN_SET);
	  	HAL_Delay(10);
	  	HAL_GPIO_WritePin(_reset_port,_reset_pin,GPIO_PIN_RESET);
	  	HAL_Delay(10);
	  	HAL_GPIO_WritePin(_reset_port,_reset_pin,GPIO_PIN_SET);
	  	HAL_Delay(10);

}

static uint32_t hal_getTimeUs(sh2_Hal_t *self) {


  return micros();
}

static void hal_callback(void *cookie, sh2_AsyncEvent_t *pEvent) {
  // If we see a reset, set a flag so that sensors will be reconfigured.
  if (pEvent->eventId == SH2_RESET) {
    // Serial.println("Reset!");
    _reset_occurred = true;
  }
}

// Handle sensor events.
static void sensorHandler(void *cookie, sh2_SensorEvent_t *event) {
  int rc;
  float t;
  float r, i, j, k, acc_deg, x, y, z;

  // Serial.println("Got an event!");

  rc = sh2_decodeSensorEvent(_sensor_value, event);
  if (rc != SH2_OK) {
    printf("BNO08x - Error decoding sensor event");
    _sensor_value->timestamp = 0;
    return;
  }
}