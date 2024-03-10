/*
 * BNO08x_I2C.h
 *
 *  Created on: Jan 23, 2024
 *      Author: acsr2004
 */

#include "main.h"


#ifndef INC_BNO08X_I2C_BNO08X_I2C_H_
#define INC_BNO08X_I2C_BNO08X_I2C_H_

//#define BNO080_SPI_CHANNEL		SPI2

//#define BNO080_SPI_SCLK_PIN		LL_GPIO_PIN_13
//#define BNO080_SPI_SCLK_PORT	GPIOB
//#define BNO080_SPI_SCLK_CLK		LL_AHB1_GRP1_PERIPH_GPIOB
//
//#define BNO080_SPI_MISO_PIN		LL_GPIO_PIN_14
//#define BNO080_SPI_MISO_PORT	GPIOB
//#define BNO080_SPI_MISO_CLK		LL_AHB1_GRP1_PERIPH_GPIOB
//
//#define BNO080_SPI_MOSI_PIN		LL_GPIO_PIN_15
//#define BNO080_SPI_MOSI_PORT	GPIOB
//#define BNO080_SPI_MOSI_CLK		LL_AHB1_GRP1_PERIPH_GPIOB
//
//#define BNO080_SPI_CS_PIN		LL_GPIO_PIN_12
//#define BNO080_SPI_CS_PORT		GPIOB
//#define BNO080_SPI_CS_CLK		LL_AHB1_GRP1_PERIPH_GPIOB
//
//#define BNO080_PS0_WAKE_PIN		LL_GPIO_PIN_8
//#define BNO080_PS0_WAKE_PORT	GPIOA
//#define BNO080_PS0_WAKE_CLK		LL_AHB1_GRP1_PERIPH_GPIOA


#define BNO08x_ADDRESS 		0x4A

//extern I2C_HandleTypeDef	hi2c2;
//#define Bno08x_hi2c			hi2c2

//Registers
enum Registers
{
	CHANNEL_COMMAND = 0,
	CHANNEL_EXECUTABLE = 1,
	CHANNEL_CONTROL = 2,
	CHANNEL_REPORTS = 3,
	CHANNEL_WAKE_REPORTS = 4,
	CHANNEL_GYRO = 5
};

//All the ways we can configure or talk to the BNO080, figure 34, page 36 reference manual
//These are used for low level communication with the sensor, on channel 2
#define SHTP_REPORT_COMMAND_RESPONSE 0xF1
#define SHTP_REPORT_COMMAND_REQUEST 0xF2
#define SHTP_REPORT_FRS_READ_RESPONSE 0xF3
#define SHTP_REPORT_FRS_READ_REQUEST 0xF4
#define SHTP_REPORT_PRODUCT_ID_RESPONSE 0xF8
#define SHTP_REPORT_PRODUCT_ID_REQUEST 0xF9
#define SHTP_REPORT_BASE_TIMESTAMP 0xFB
#define SHTP_REPORT_SET_FEATURE_COMMAND 0xFD

//All the different sensors and features we can get reports from
//These are used when enabling a given sensor
#define SENSOR_REPORTID_ACCELEROMETER 0x01
#define SENSOR_REPORTID_GYROSCOPE 0x02
#define SENSOR_REPORTID_MAGNETIC_FIELD 0x03
#define SENSOR_REPORTID_LINEAR_ACCELERATION 0x04
#define SENSOR_REPORTID_ROTATION_VECTOR 0x05
#define SENSOR_REPORTID_GRAVITY 0x06
#define SENSOR_REPORTID_GAME_ROTATION_VECTOR 0x08
#define SENSOR_REPORTID_GEOMAGNETIC_ROTATION_VECTOR 0x09
#define SENSOR_REPORTID_TAP_DETECTOR 0x10
#define SENSOR_REPORTID_STEP_COUNTER 0x11
#define SENSOR_REPORTID_STABILITY_CLASSIFIER 0x13
#define SENSOR_REPORTID_PERSONAL_ACTIVITY_CLASSIFIER 0x1E

//Record IDs from figure 29, page 29 reference manual
//These are used to read the metadata for each sensor type
#define FRS_RECORDID_ACCELEROMETER 0xE302
#define FRS_RECORDID_GYROSCOPE_CALIBRATED 0xE306
#define FRS_RECORDID_MAGNETIC_FIELD_CALIBRATED 0xE309
#define FRS_RECORDID_ROTATION_VECTOR 0xE30B

//Command IDs from section 6.4, page 42
//These are used to calibrate, initialize, set orientation, tare etc the sensor
#define COMMAND_ERRORS 1
#define COMMAND_COUNTER 2
#define COMMAND_TARE 3
#define COMMAND_INITIALIZE 4
#define COMMAND_DCD 6
#define COMMAND_ME_CALIBRATE 7
#define COMMAND_DCD_PERIOD_SAVE 9
#define COMMAND_OSCILLATOR 10
#define COMMAND_CLEAR_DCD 11

#define CALIBRATE_ACCEL 0
#define CALIBRATE_GYRO 1
#define CALIBRATE_MAG 2
#define CALIBRATE_PLANAR_ACCEL 3
#define CALIBRATE_ACCEL_GYRO_MAG 4
#define CALIBRATE_STOP 5

#define MAX_PACKET_SIZE 512 //Packets can be up to 32k but we don't have that much RAM.
#define MAX_METADATA_SIZE 9 //This is in words. There can be many but we mostly only care about the first 9 (Qs, range, etc)


#define accelerometer_Q1  8

typedef struct {
   uint8_t rep_id;   // report id 0xF8
   uint8_t r_cause;  // report 0xF8 byte 1 Reset Cause
   uint8_t sw_vmaj;  // report 0xF8 byte 2 SW Version Major
   uint8_t sw_vmin;  // report 0xF8 byte 3 SW Version Minor
   uint32_t sw_pnm;  // report 0xF8 byte 4-7 SW Part Number
   uint32_t sw_bnm;  // report 0xF8 byte 8-11 SW Build Number
   uint16_t sw_vpn;  // report 0xF8 byte 12-13 SW Version Patch
} prodid;

typedef struct{
	I2C_HandleTypeDef*	hi2c2;

	GPIO_TypeDef * rst_port;
	uint16_t rst_pin;

	GPIO_TypeDef * p0_port;
	uint16_t p0_pin;

	GPIO_TypeDef * p1_port;
	uint16_t p1_pin;

	GPIO_TypeDef * bootn_port;
	uint16_t bootn_pin;

	GPIO_TypeDef * int_port;
	uint16_t int_pin;

	uint8_t sequence_number[6];
	uint8_t header[4];
	uint8_t data[MAX_PACKET_SIZE];


	uint16_t rawAccelX, rawAccelY, rawAccelZ, accelAccuracy;
	uint16_t rawLinAccelX, rawLinAccelY, rawLinAccelZ, accelLinAccuracy;
	uint16_t rawGyroX, rawGyroY, rawGyroZ, gyroAccuracy;
	uint16_t rawMagX, rawMagY, rawMagZ, magAccuracy;
	uint16_t rawQuatI, rawQuatJ, rawQuatK, rawQuatReal, rawQuatRadianAccuracy, quatAccuracy;
	uint16_t stepCount;
	uint8_t stabilityClassifier;
	uint8_t activityClassifier;
	uint8_t *_activityConfidences; //Array that store the confidences of the 9 possible activities

	//These Q values are defined in the datasheet but can also be obtained by querying the meta data records
	//See the read metadata example for more info
	//int16_t rotationVector_Q1 = 14;
	//int16_t accelerometer_Q1 = 8;
	//int16_t linear_accelerometer_Q1 = 8;
	//int16_t gyro_Q1 = 9;
	//int16_t magnetometer_Q1 = 4;

} BNO08x;


uint8_t bno08x_hardware_reset(BNO08x* bno);
uint8_t bno08x_initialization(BNO08x* bno);


uint8_t bno08x_data_ready(BNO08x* bno);



uint8_t bno08x_wait_for_data(BNO08x* bno);
int16_t bno08x_receive_packet(BNO08x* bno);
HAL_StatusTypeDef bno08x_send_packet(BNO08x* bno, uint8_t channelNumber, uint8_t* data, uint16_t dataLength);

HAL_StatusTypeDef bno08x_set_feature_command(BNO08x* bno, uint8_t report_ID, uint16_t time_between_reports, uint32_t specific_config);

HAL_StatusTypeDef bno08x_enable_accelerometer(BNO08x* bno, uint16_t time_between_reports);
HAL_StatusTypeDef bno08x_enable_gyro(BNO08x* bno, uint16_t time_between_reports);
HAL_StatusTypeDef bno08x_enable_rotation_vector(BNO08x* bno, uint16_t time_between_reports);

void bno08x_parse_report(BNO08x* bno);

void bno08x_get_acc(BNO08x* bno, float* acc);

void bno08x_get_prodid(BNO08x * bno,prodid* prodid_list);

float q_to_float(int16_t fixedPointValue, uint8_t qPoint);
int16_t bno085x_get_errors(BNO08x* bno);
void bno085x_print_errors(BNO08x* bno);

#endif /* INC_BNO08X_I2C_BNO08X_I2C_H_ */