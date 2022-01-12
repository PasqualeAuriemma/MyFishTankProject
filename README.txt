contain channels of 101 subjects after download them

set scheda on LOLIN(WEMOS) D1 R2&mini
inside the directory ESP8266_flasher_V00170901_00_Cloud Update Ready
flash the firmware with the esp8266_flasher.exe 
choose AiThinker_ESP8266_DIO_8M_8M_20160615_V1.5.4.bin inside At_firmware_bin1.54

https://wiki.dfrobot.com/Gravity__Analog_pH_Sensor_Meter_Kit_V2_SKU_SEN0161-V2
Calibration
To ensure accuracy, the probe needs to be calibrated for its first use and after not being used for an extended period of time (once a month ideally). This tutorial uses two-point calibration and therefore requires two standard buffer solutions of 4.0 and 7.0. The following steps show how to operate two-point calibration.

1.Upload the sample code to the Arduino board, then open the serial monitor, after you can see the temperature and pH. If you added a temperature sensor, be sure to write the corresponding function and call it.

1572.27
02:35:09.948 -> voltage:1562.50 temperature:14.0^C  pH:7.00

02:37:26.740 -> voltage:2011.72 temperature:14.0^C  pH:4.00

2.Wash the probe with distilled water, then absorb the residual water-drops with paper. Insert the pH probe into the standard buffer solution of 7.0, stir gently, until the values are stable.

3.After the values are stable, the first point can be calibrated. Specific steps are as follows:

  1. Input enterph command in the serial monitor to enter the calibration mode.

  2.Input calph commands in the serial monitor to start the calibration. The program will automatically identify which of the two standard buffer solutions is present: either 4.0 and 7.0. In this step, the standard buffer solution of 7.0 will be identified.

  3. After the calibration, input exitph command in the serial monitor to save the relevant parameters and exit the calibration mode. Note: Only after inputing exitph command in the serial monitor can the relevant parameters be saved.

  4.After the above steps, the first point calibration is completed. The second point calibration will be performed below.

4. Wash the probe with distilled water, then absorb the residual water-drops with paper. Insert the pH probe into the standard buffer solution of 4.0, stir gently, until the values are stable.

5.After the values are stable, the second point can be calibrated. These steps are the same as the first calibration step. The specific steps are as follows:

  1. Input enterph command in the serial monitor to enter the calibration mode.

  2.Input calph commands in the serial monitor to start the calibration. The program will automatically identify which of the two standard buffer solutions is present: either 4.0 and 7.0 In this step, the standard buffer solution of 4.0 will be identified.

  3. After the calibration, input the exitph command in the serial monitor to save the relevant parameters and exit the calibration mode. Note: Only after inputing exitph command in the serial monitor can the relevant parameters be saved.

  4.After the above steps, the second point calibration is completed.

6.After completing the above steps, the two-point calibration is completed, and then the sensor can be used for actual measurement. The relevant parameters in the calibration process have been saved to the EEPROM of the main control board.


https://wiki.dfrobot.com/Gravity__Analog_Signal_Isolator_SKU_DFR0504

https://win.adrirobot.it/wemos_mega/wemos_mega.htm



temp_tab
1	id	        bigint			No	               Nessuno	            AUTO_INCREMENT		
2	temperature	float			No	               Nessuno		
3   data_send	text	        utf8mb4_0900_ai_ci		                Sì	NULL		
4	data_arrive	timestamp		No	               CURRENT_TIMESTAMP	DEFAULT_GENERATED

ec_tab
1	id	        bigint			No	               Nessuno	            AUTO_INCREMENT		
2	ec      	float			No	               Nessuno		
3   data_send	text	        utf8mb4_0900_ai_ci		                Sì	NULL		
4	data_arrive	timestamp		No	               CURRENT_TIMESTAMP	DEFAULT_GENERATED

ph_tab
1	id	        bigint			No	               Nessuno	            AUTO_INCREMENT		
2	ph      	float			No	               Nessuno		
3   data_send	text	        utf8mb4_0900_ai_ci		                Sì	NULL		
4	data_arrive	timestamp		No	               CURRENT_TIMESTAMP	DEFAULT_GENERATED
