contain channels of 101 subjects after download them

set scheda on LOLIN(WEMOS) D1 R2&mini
inside the directory ESP8266_flasher_V00170901_00_Cloud Update Ready
flash the firmware with the esp8266_flasher.exe 
choose AiThinker_ESP8266_DIO_8M_8M_20160615_V1.5.4.bin inside At_firmware_bin1.54

temp_tab
1	id	        bigint			No	               Nessuno	            AUTO_INCREMENT		
2	temperature	float			  No	               Nessuno		
3 data_send	  varchar(10)	utf8mb4_0900_ai_ci Sì	                  NULL		
4	data_arrive	timestamp		No	               CURRENT_TIMESTAMP	  DEFAULT_GENERATED

ec_tab
1	id	        bigint			No	               Nessuno	            AUTO_INCREMENT		
2	ec      	  float			  No	               Nessuno		
3 data_send	  varchar(10)	utf8mb4_0900_ai_ci Sì	                  NULL		
4	data_arrive	timestamp		No	               CURRENT_TIMESTAMP	  DEFAULT_GENERATED

ph_tab
1	id	        bigint			No	               Nessuno	            AUTO_INCREMENT		
2	ph      	  float			  No	               Nessuno		
3 data_send	 	varchar(10) utf8mb4_0900_ai_ci Sì	                  NULL		
4	data_arrive	timestamp		No	               CURRENT_TIMESTAMP	  DEFAULT_GENERATED

Board: 
Mega 2560 + ESP8266 

Moduli: 
DS18B20
LiquidCrystal + I2C 2*16
RTC_DS1307
TDS Meter v 1.0 KS0429
Micro-SD Memory Card Adapter for Arduino with 3.3V-5V
Modulo Rele Relay Relevador 8 Canales 5v
AD Analog Keyboard
