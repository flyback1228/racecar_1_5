## Get sensor reading from hobbywing ezrun max5
This crack process works on the OTA Programming Device https://www.hobbywingdirect.com/products/ota.
The OTA uses the nRF52832 MS50SFA module as the BLE. According to the pin definitions, P0.03 and P0.04 are the UART communications pins. The MPU of the OTA module forwards the sensor data to BLE module via this uart port with the baudrate 115200. We can capture the signal from P0.04(RX).

![./racecar_1_5/imag/nrf52810_pins_def.png](https://github.com/flyback1228/racecar_1_5/blob/main/hobbywing_ezrun_esc_crack/img/nrf52810_pins_def.png)
![./racecar_1_5/imag/ms50sfa_pinout.PNG](https://github.com/flyback1228/racecar_1_5/blob/main/hobbywing_ezrun_esc_crack/img/ms50sfa_pinout.PNG)
![./racecar_1_5/imag/bluetooth_MS50SFA.jpg](https://github.com/flyback1228/racecar_1_5/blob/main/hobbywing_ezrun_esc_crack/img/bluetooth_MS50SFA.jpg)

A group of transmit data contains 32 bytes

![./racecar_1_5/imag/received_bytes.png](https://github.com/flyback1228/racecar_1_5/blob/main/hobbywing_ezrun_esc_crack/img/received_bytes.png)


1. Throttle, 1 byte, percentage
2. Throttle direction, 1 byte (1--forward, 2--reverse)
3. RPM, 2 bytes, the second byte reprensents the high byte and the first byte is the low byte (0x16<<8 | 0x3B), the real RPM should be multiplied by 10. RPM = (0x16<<8 | 0x3B)*10
4. Voltage, 1 byte, should be divided by 10. V = 0xE3/10
5. Current, 2 bytes, the first byte reprensents the high byte and the second byte is the low byte, should be divided by 10. I = (0x00<<8 | 0x15)/10
6. Temperature, 1 byte.
