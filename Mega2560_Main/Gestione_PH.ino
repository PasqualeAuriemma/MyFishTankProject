/*
  Aquarium Project Pasquale
*/

int analogBufferPH[SCOUNT]; // store the analog value in the array, read from ADC
int analogBufferTempPH[SCOUNT];
int analogBufferIndexPH = 0, copyIndexPH = 0;
float averageVoltagePH = 0;

//Getting the PH from Gravity: Analog pH Sensor / Meter Pro Kit V2
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
        //temperature = readTemperature();         // read your temperature sensor to execute temperature compensation
        voltage = getMedianNum(analogBufferTempPH, SCOUNT)/1024.0*5000;  // read the analog value more stable by the median filtering algorithm, and convert to voltage real value
        phValue = ph.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
        //Serial.print("voltage:");
        //Serial.print(voltage);
        //Serial.print(" temperature:");
        //Serial.print(temperature,1);
        //Serial.print("^C  pH:");
        //Serial.println(phValue,2);
    }
    ph.calibration(voltage, temperature);  
    
    return (float)phValue;
}
