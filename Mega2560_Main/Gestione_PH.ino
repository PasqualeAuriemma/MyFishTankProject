/*
  Aquarium Project Pasquale

 * file DFRobot_PH.ino
 * @ https://github.com/DFRobot/DFRobot_PH
 *
 * This is the sample code for Gravity: Analog pH Sensor / Meter Kit V2, SKU:SEN0161-V2
 * In order to guarantee precision, a temperature sensor such as DS18B20 is needed, to execute automatic temperature compensation.
 * You can send commands in the serial monitor to execute the calibration.
 * Serial Commands:
 *   enterph -> enter the calibration mode
 *   calph   -> calibrate with the standard buffer solution, two buffer solutions(4.0 and 7.0) will be automaticlly recognized
 *   exitph  -> save the calibrated parameters and exit from calibration mode
 *
 * Copyright   [DFRobot](http://www.dfrobot.com), 2018
 * Copyright   GNU Lesser General Public License
 *
 * version  V1.0
 * date  2018-04
 */


int analogBufferPH[SCOUNT]; // store the analog value in the array, read from ADC
int analogBufferTempPH[SCOUNT];
int analogBufferIndexPH = 0, copyIndexPH = 0;
float averageVoltagePH = 0;
float voltagePH;

//Getting the PH from Gravity: Analog pH Sensor / Meter Pro Kit V2

float getPH1(float temperature) {
  static unsigned long timepoint = millis();
  if(millis()-timepoint>1000U){                            //time interval: 1s
      timepoint = millis();
      voltagePH = analogRead(PH_PIN)/1024.0*5000;          // read the ph voltage
      phValue    = ph.readPH(voltagePH,temperature);       // convert voltage to pH with temperature compensation
      //Serial.print("pH:");
      //Serial.print(phValue,2);
      char cmd[10];
      if(readSerial(cmd)){
        strupr(cmd);
        if(strstr(cmd,"PH")){
          ph.calibration(voltagePH,temperature,cmd);       //PH calibration process by Serail CMD
        }
      }
  }  
  
  return phValue;
}


float getPH(float temperature) {
  analogBufferPH[analogBufferIndexPH] = analogRead(PH_PIN); //read the analog value and store into the buffer
  analogBufferIndexPH++;
  if (analogBufferIndexPH == SCOUNT) {
    analogBufferIndexPH = 0;
  }
  static unsigned long timepoint1 = millis();
    if(millis()-timepoint1>1000U){                  //time interval: 1s
        timepoint1 = millis();
        for (copyIndexPH = 0; copyIndexPH < SCOUNT; copyIndexPH++) {
           analogBufferTempPH[copyIndexPH] = analogBufferPH[copyIndexPH];
        }
        voltage = getMedianNum(analogBufferTempPH, SCOUNT)/1024.0*5000;  // read the analog value more stable by the median filtering algorithm, and convert to voltage real value
        phValue = ph.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
        //Serial.print("voltage:");
        //Serial.print(voltage);
        //Serial.print(" temperature:");
        //Serial.print(temperature,1);
        //Serial.print("^C  pH:");
        //Serial.println(phValue,2);
        char cmd[10];
        if(readSerial(cmd)){
          strupr(cmd);
          if(strstr(cmd,"PH")){
            ph.calibration(voltage,temperature,cmd);       //PH calibration process by Serail CMD
          }
        }
    }
    //ph.calibration(voltage, temperature);  
    
    return phValue;
}
