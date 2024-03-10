/*
 * BNO08x_I2C.c
 *
 *  Created on: Jan 23, 2024
 *      Author: acsr2004
 */

#include "BNO08x_I2C.h"
#include "utility.h"
#include <string.h>
//uint8_t shtpHeader[4]; //Each packet has a header of 4 bytes
//uint8_t shtpData[MAX_PACKET_SIZE];
//static uint8_t _sequence_number[6] = {0, 0, 0, 0, 0, 0}; //There are 6 com channels. Each channel has its own seqnum
//uint8_t commandSequenceNumber = 0;				//Commands have a seqNum as well. These are inside command packet, the header uses its own seqNum per channel
//uint32_t metaData[MAX_METADATA_SIZE];


uint8_t bno08x_hardware_reset(BNO08x* bno){

	if(bno->rst_port==NULL)
		return 0;

	if(bno->p0_port!=NULL){
		HAL_GPIO_WritePin(bno->p0_port, bno->p0_pin, GPIO_PIN_RESET);
	}
	if(bno->p1_port!=NULL){
		HAL_GPIO_WritePin(bno->p1_port, bno->p1_pin, GPIO_PIN_RESET);
	}

	if(bno->bootn_port!=NULL){
		HAL_GPIO_WritePin(bno->bootn_port, bno->bootn_pin, GPIO_PIN_SET);
	}

	HAL_Delay(100);

	HAL_GPIO_WritePin(bno->rst_port,bno->rst_pin,GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(bno->rst_port,bno->rst_pin,GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(bno->rst_port,bno->rst_pin,GPIO_PIN_SET);
	HAL_Delay(10);

	return 1;
}

uint8_t bno08x_data_ready(BNO08x* bno){
	GPIO_PinState state = HAL_GPIO_ReadPin(bno->int_port, bno->int_pin);
	return !(uint8_t)state;
}

uint8_t bno08x_wait_for_data(BNO08x* bno){
	for (uint32_t counter = 0; counter < 1000; counter++)
	{
		if(bno08x_data_ready(bno)){
			return 1;
		}
		HAL_Delay(1);
	}
	return 0;
}

int16_t bno08x_receive_packet(BNO08x* bno)
{
	//bno08x_wait_for_data(bno);

	HAL_StatusTypeDef state = HAL_I2C_Master_Receive(bno->hi2c2, BNO08x_ADDRESS<<1, bno->header, 4, 10);
	if(state!=HAL_OK){
		return -1;
	}

	uint16_t data_length = ((uint16_t)(bno->header[1]) << 8 | bno->header[0]);
	data_length &= ~(1 << 15);

	if (data_length == 0)
	{
		return 0; //All done
	}
	data_length -= 4; //Remove the header bytes from the data count

	HAL_Delay(1);


	uint8_t temp_data[data_length+4];
	state = HAL_I2C_Master_Receive(bno->hi2c2, BNO08x_ADDRESS<<1, temp_data, data_length+4, 100);
	if(state!=HAL_OK){
		return 0;
	}

	bno->sequence_number[temp_data[2]]=temp_data[3];

	memcpy(bno->header,temp_data,4);
	memcpy(bno->data,&(temp_data[4]),data_length);

	return data_length;
}

uint8_t bno08x_initialization(BNO08x* bno){


	bno08x_receive_packet(bno);
	HAL_Delay(100);
	bno08x_receive_packet(bno);
	HAL_Delay(100);

	memset(bno->data,0,MAX_PACKET_SIZE);

	uint8_t e_data[1];
	e_data[0] = 1;
	bno08x_send_packet(bno, CHANNEL_EXECUTABLE, e_data, 1); //Transmit packet on channel 1, 1 byte
	HAL_Delay(100);


	bno08x_wait_for_data(bno);
	bno08x_receive_packet(bno);
	//printf("%d\n",data_length);
	HAL_Delay(100);

	bno08x_send_packet(bno, CHANNEL_EXECUTABLE, e_data, 1);
	HAL_Delay(100);
	bno08x_wait_for_data(bno);
	bno08x_receive_packet(bno);
	//printf("%d\n",data_length);
	HAL_Delay(100);

	/*while(data_length!=0){
		data_length = BNO08x_receive_packet(bno_header, bno_data);
		printf("%d\n",data_length);
	}

	//printf("%u\n",data_length);
	HAL_Delay(100);
	while(data_length!=0){
		data_length = BNO08x_receive_packet(bno_header, bno_data);
		printf("%d\n",data_length);
	}*/

	//BNO08x_wait_for_data();
	//data_length = BNO08x_receive_packet(bno_header, bno_data);
	//printf("%u\n",data_length);
	//HAL_Delay(500);

	bno08x_wait_for_data(bno);
	prodid prodids[2];
	bno08x_get_prodid(bno, prodids);
	/*
	uint8_t data[2];
	data[0] = SHTP_REPORT_PRODUCT_ID_REQUEST;
	data[1] = 0;

	if(bno08x_send_packet(bno,CHANNEL_CONTROL, data, 2)!=HAL_OK){
		return 0;
	}
	//BNO08x_wait_for_data();
	if (bno08x_receive_packet(bno) >0)
	{
		//printf("header: %d %d %d %d\n", bno_header[0], bno_header[1], bno_header[2], bno_header[3]);
		if (bno->data[0] == SHTP_REPORT_PRODUCT_ID_RESPONSE)
		{
			printf("BNO08x who_am_i = 0x%02x...ok\n\n", bno->data[0]);
			return 1;
		}
	}*/
	return 0;


}

HAL_StatusTypeDef bno08x_send_packet(BNO08x* bno, uint8_t channel_number, uint8_t* data, uint16_t data_length){

	//if (BNO08x_wait_for_data() == 0)
	//	return HAL_TIMEOUT; //Data is not available

	uint16_t packet_length = data_length + 4; //Add four bytes for the header
	uint8_t send_data[packet_length];
	send_data[0]=(uint8_t)packet_length & 0xFF;
	send_data[1]=(uint8_t)(packet_length >> 8);
	send_data[2]=channel_number;
	send_data[3]=bno->sequence_number[channel_number]++;

	memcpy(&(send_data[4]),data,data_length);

	return HAL_I2C_Master_Transmit(bno->hi2c2,BNO08x_ADDRESS<<1, send_data, packet_length,100);
}

HAL_StatusTypeDef bno08x_set_feature_command(BNO08x* bno, uint8_t report_ID, uint16_t time_between_reports, uint32_t specific_config)
{
  long microsBetweenReports = (long)time_between_reports * 1000L;
  uint8_t data[17];

  data[0] = SHTP_REPORT_SET_FEATURE_COMMAND; //Set feature command. Reference page 55
  data[1] = report_ID; //Feature Report ID. 0x01 = Accelerometer, 0x05 = Rotation vector
  data[2] = 0; //Feature flags
  data[3] = 0; //Change sensitivity (LSB)
  data[4] = 0; //Change sensitivity (MSB)
  data[5] = (microsBetweenReports >> 0) & 0xFF; //Report interval (LSB) in microseconds. 0x7A120 = 500ms
  data[6] = (microsBetweenReports >> 8) & 0xFF; //Report interval
  data[7] = (microsBetweenReports >> 16) & 0xFF; //Report interval
  data[8] = (microsBetweenReports >> 24) & 0xFF; //Report interval (MSB)
  data[9] = 0; //Batch Interval (LSB)
  data[10] = 0; //Batch Interval
  data[11] = 0; //Batch Interval
  data[12] = 0; //Batch Interval (MSB)
  data[13] = (specific_config >> 0) & 0xFF; //Sensor-specific config (LSB)
  data[14] = (specific_config >> 8) & 0xFF; //Sensor-specific config
  data[15] = (specific_config >> 16) & 0xFF; //Sensor-specific config
  data[16] = (specific_config >> 24) & 0xFF; //Sensor-specific config (MSB)

  //Transmit packet on channel 2, 17 bytes
  return bno08x_send_packet(bno, CHANNEL_CONTROL,data, 17);
}

HAL_StatusTypeDef bno08x_enable_accelerometer(BNO08x* bno, uint16_t time_between_reports)
{
  return bno08x_set_feature_command(bno, SENSOR_REPORTID_ACCELEROMETER, time_between_reports,0);
}

HAL_StatusTypeDef bno08x_enable_gyro(BNO08x* bno, uint16_t time_between_reports)
{
  return bno08x_set_feature_command(bno, SENSOR_REPORTID_GYROSCOPE, time_between_reports,0);
}

HAL_StatusTypeDef bno08x_enable_rotation_vector(BNO08x* bno, uint16_t time_between_reports){
	return bno08x_set_feature_command(bno, SENSOR_REPORTID_ROTATION_VECTOR, time_between_reports,0);
}

void bno08x_parse_report(BNO08x* bno)
{
	//Calculate the number of data bytes in this packet
	int16_t dataLength = ((uint16_t)bno->header[1] << 8 | bno->header[0]);
	dataLength &= ~(1 << 15); //Clear the MSbit. This bit indicates if this package is a continuation of the last.


	dataLength -= 4; //Remove the header bytes from the data count

    uint8_t status = bno->data[5 + 2] & 0x03; //Get status bits
	uint16_t data1 = (uint16_t)bno->data[5 + 5] << 8 | bno->data[5 + 4];
    uint16_t data2 = (uint16_t)bno->data[5 + 7] << 8 | bno->data[5 + 6];
    uint16_t data3 = (uint16_t)bno->data[5 + 9] << 8 | bno->data[5 + 8];
	uint16_t data4 = 0;
	uint16_t data5 = 0;

	if(dataLength - 5 > 9)
	{
		data4= (uint16_t)bno->data[5 + 11] << 8 | bno->data[5 + 10];
	}
	if(dataLength - 5 > 11)
	{
		data5 = (uint16_t)bno->data[5 + 13] << 8 | bno->data[5 + 12];
	}

	//Store these generic values to their proper global variable
	if(bno->data[5] == SENSOR_REPORTID_ACCELEROMETER)
	{
		bno->accelAccuracy = status;
		bno->rawAccelX = data1;
		bno->rawAccelY = data2;
		bno->rawAccelZ = data3;
	}
	else if(bno->data[5] == SENSOR_REPORTID_LINEAR_ACCELERATION)
	{
		bno->accelLinAccuracy = status;
		bno->rawLinAccelX = data1;
		bno->rawLinAccelY = data2;
		bno->rawLinAccelZ = data3;
	}
	else if(bno->data[5] == SENSOR_REPORTID_GYROSCOPE)
	{
		bno->gyroAccuracy = status;
		bno->rawGyroX = data1;
		bno->rawGyroY = data2;
		bno->rawGyroZ = data3;
	}
	else if(bno->data[5] == SENSOR_REPORTID_MAGNETIC_FIELD)
	{
		bno->magAccuracy = status;
		bno->rawMagX = data1;
		bno->rawMagY = data2;
		bno->rawMagZ = data3;
	}
	else if(bno->data[5] == SENSOR_REPORTID_ROTATION_VECTOR || bno->data[5] == SENSOR_REPORTID_GAME_ROTATION_VECTOR)
	{
		bno->quatAccuracy = status;
		bno->rawQuatI = data1;
		bno->rawQuatJ = data2;
		bno->rawQuatK = data3;
		bno->rawQuatReal = data4;
		bno->rawQuatRadianAccuracy = data5; //Only available on rotation vector, not game rot vector
	}
	else if(bno->data[5] == SENSOR_REPORTID_STEP_COUNTER)
	{
		bno->stepCount = data3; //Bytes 8/9
	}
	else if(bno->data[5] == SENSOR_REPORTID_STABILITY_CLASSIFIER)
	{
		bno->stabilityClassifier = bno->data[5 + 4]; //Byte 4 only
	}
	else if(bno->data[5] == SENSOR_REPORTID_PERSONAL_ACTIVITY_CLASSIFIER)
	{
		bno->activityClassifier = bno->data[5 + 5]; //Most likely state

		//Load activity classification confidences into the array
		for(uint8_t x = 0 ; x < 9 ; x++) //Hardcoded to max of 9. TODO - bring in array size
			bno->_activityConfidences[x] = bno->data[5 + 6 + x]; //5 bytes of timestamp, byte 6 is first confidence byte
	}
	else
	{
		printf ("Error: sensor report ID [%02X] is unhandled.\n", bno->data[5]);
	}

	//TODO additional feature reports may be strung together. Parse them all.
}

float q_to_float(int16_t fixedPointValue, uint8_t qPoint)
{
  float qFloat = fixedPointValue;
  qFloat *= pow(2, qPoint * -1);
  return (qFloat);
}


void bno08x_get_acc(BNO08x* bno, float* acc){
	acc[0] = q_to_float(bno->rawAccelX, accelerometer_Q1);
	acc[1] = q_to_float(bno->rawAccelY, accelerometer_Q1);
	acc[2] = q_to_float(bno->rawAccelZ, accelerometer_Q1);

}

void bno08x_get_prodid(BNO08x * bno,prodid* prodid_list) {
   /* --------------------------------------------------------- *
    * SHTP communication channel 2: write 0xF9 request          *
    * --------------------------------------------------------- */
   uint8_t data[] = {SHTP_REPORT_PRODUCT_ID_REQUEST,0};
   bno08x_send_packet(bno, CHANNEL_CONTROL, data, 2);
   HAL_Delay(1);

   /* --------------------------------------------------------- *
    * SHTP communication channel 2: read 1st 0xF8 response      *
    * --------------------------------------------------------- */
   uint16_t count = 0;
   uint16_t datalen = 0;
   while ((datalen = bno08x_receive_packet(bno)) >0) {
      if(count > 3) break;
      if(bno->header[2] == CHANNEL_CONTROL
         && bno->data[0] == SHTP_REPORT_PRODUCT_ID_RESPONSE) break;
      HAL_Delay(1);           // Delay 100 microsecs befoer next I2C
      count++;
   }

   if(bno->data[0] != SHTP_REPORT_PRODUCT_ID_RESPONSE) {
      printf("Error: Not getting 1st SHTP product-ID report\n");
      return;
   }

   /* --------------------------------------------------------- *
    * Assign 1st product report data to info structure          *
    * --------------------------------------------------------- */
   prodid_list[0].rep_id  = bno->data[0] ;  // report 0xF8 byte 0 Report ID
   prodid_list[0].r_cause = bno->data[1];   // report 0xF8 byte 1 Reset Cause
   prodid_list[0].sw_vmaj = bno->data[2];   // report 0xF8 byte 2 SW Version Major
   prodid_list[0].sw_vmin = bno->data[3];   // report 0xF8 byte 2 SW Version Minor
   prodid_list[0].sw_pnm  = bno->data[4] | (bno->data[5] << 8) | (bno->data[6] << 16) | (bno->data[7] << 24);// report 0xF8 byte 4-7 SW Part Number
   prodid_list[0].sw_bnm  = bno->data[8] | (bno->data[9] << 8) | (bno->data[10] << 16) | (bno->data[11] << 24); // report 0xF8 byte 8-11 SW Build Number
   prodid_list[0].sw_vpn  = bno->data[12] | (bno->data[13] << 8); // report 0xF8 byte 12-13 SW Version Patch

   /* --------------------------------------------------------- *
    * SHTP communication channel 2: read 2nd 0xF8 response      *
    * --------------------------------------------------------- */
   HAL_Delay(200);             // Delay 100 microsecs befoer next I2C
   count = 0;
   datalen = 0;
   while ((datalen = bno08x_receive_packet(bno)) >0) {
      if(count > 3) break;
      if(bno->header[2] == CHANNEL_CONTROL
         && bno->data[0] == SHTP_REPORT_PRODUCT_ID_RESPONSE) break;
      HAL_Delay(1);                  // Delay 100 microsecs befoer next I2C
      count++;
   }
     // printf("Test %d data-0 %d\n", count, shtpData[0]);

   if(bno->data[0] != SHTP_REPORT_PRODUCT_ID_RESPONSE) {
      printf("Error: Not getting 2nd SHTP product-ID report\n");
      return;
   }

   /* --------------------------------------------------------- *
    * Assign 2nd product report data to info structure          *
    * --------------------------------------------------------- */
   prodid_list[1].rep_id  = bno->data[0];  // report 0xF8 byte 0 Report ID
   prodid_list[1].r_cause = bno->data[1];   // report 0xF8 byte 1 Reset Cause
   prodid_list[1].sw_vmaj = bno->data[2];   // report 0xF8 byte 2 SW Version Major
   prodid_list[1].sw_vmin = bno->data[3];   // report 0xF8 byte 2 SW Version Minor
   prodid_list[1].sw_pnm  = bno->data[4] | (bno->data[5] << 8) | (bno->data[6] << 16) | (bno->data[7] << 24); // report 0xF8 byte 4-7 SW Part Number
   prodid_list[1].sw_bnm  = bno->data[8] | (bno->data[9] << 8) | (bno->data[10] << 16) | (bno->data[11] << 24); // report 0xF8 byte 8-11 SW Build Number
   prodid_list[1].sw_vpn  = bno->data[12] | (bno->data[13] << 8); // report 0xF8 byte 12-13 SW Version Patch


}

int16_t bno085x_get_errors(BNO08x* bno) {

   uint8_t data[] = {0x01};                // CMD 0x01 gets error list
   bno08x_send_packet(bno, CHANNEL_COMMAND,data, 1);    // Write 1 byte to chan CMD
   HAL_Delay(100);             // 100 msecs before next I2C

   /* --------------------------------------------------------- *
    * Get the SHTP error list, after reset it should be clean   *
    *  RX   5 bytes HEAD 05 80 00 03 CARGO 01 ST [0]            *
    * --------------------------------------------------------- */
   // Wait for answer packet
   int16_t datalen;
   while ((datalen = bno08x_receive_packet(bno)) >0) {
      if(bno->header[2] == CHANNEL_COMMAND && bno->data[0] == 0x01) break;
      HAL_Delay(1);              // Delay 100 microsecs before next I2C
   }

   if(bno->header[2] != CHANNEL_COMMAND || bno->data[0] != 1) {
      printf("Error: can't get SHTP error list\n");
      return(-1);
   }

   /* --------------------------------------------------------- *
    * Calculate the error counter                               *
    * --------------------------------------------------------- */
   //int16_t errcount = datalen - 1; // datalen minus 1 report byte
   //if(verbose == 1) printf("Debug: OK  Error list %d entries\n", errcount);

   return datalen - 1;
}

void bno085x_print_errors(BNO08x* bno) {
const static char* shtpErrorStr[] = {
   "No Error",
   "Hub application attempted to exceed maximum read cargo length",
   "Hub application attempted to exceed maximum read cargo length",
   "Host wrote a header with length greater than maximum write cargo length",
   "Host wrote a header with length <= header length (invalid or no payload)",
   "Host tried to fragment cargo (transfer length < full cargo length)",
   "Host wrote continuation of fragmented cargo (continuation bit set)",
   "Unrecognized command on control channel (2)",
   "Unrecognized parameter to get-advertisement command",
   "Host wrote to unrecognized channel",
   "Advertisement request received while Advertisement Response was pending",
   "Host write before the hub finished sending advertisement response",
   "Error list too long to send, truncated"
};

   /* --------------------------------------------------------- *
    * Check if shtpHeader and Data contain the error list       *
    * --------------------------------------------------------- */
   if(bno->header[2] != CHANNEL_COMMAND || bno->data[0] != 0x01) {
      printf("Debug: Error list not read\n");
      return 1;
   }

   /* --------------------------------------------------------- *
    * Calculate the error counter                               *
    * --------------------------------------------------------- */
   short packetlen = ((short) (bno->header[1]) << 8 | bno->header[0]);
   packetlen &= ~(1 << 15);        // Clear the MSbit.
   short errcount = packetlen - 5; // minus 4 header and 1 report

   printf("SHTP Errors # : %d entries\n", errcount);
   if(errcount == 0) return;

   for(int i=0; i<errcount; i++) {
      printf("SHTP Error %2d : %d = %s\n", i, bno->data[1+i], shtpErrorStr[bno->data[1+i]]);
   }

}
