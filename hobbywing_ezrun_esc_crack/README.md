## Get sensor reading from hobbywing ezrun max5
This crack process works on the OTA Programming Device https://www.hobbywingdirect.com/products/ota.
The OTA uses the nRF52832 Module MS50SFA module as the BLE. According to the pin definitions, P0.03 and P0.04 are the UART communications pins. The MPU of the OTA module forwards the sensor data to BLE module via this uart port with the baudrate 115200. We can capture the signal from P0.04(RX).

![./racecar_1_5/imag/nrf52810_pins_def.png](https://github.com/flyback1228/racecar_1_5/blob/main/hobbywing_ezrun_esc_crack/img/nrf52810_pins_def.png)
![./racecar_1_5/imag/ms50sfa_pinout.PNG](https://github.com/flyback1228/racecar_1_5/blob/main/hobbywing_ezrun_esc_crack/img/ms50sfa_pinout.PNG)
![./racecar_1_5/imag/bluetooth_MS50SFA.jpg](https://github.com/flyback1228/racecar_1_5/blob/main/hobbywing_ezrun_esc_crack/img/bluetooth_MS50SFA.jpg)

