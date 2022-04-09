/*
  Aquarium Project Pasquale
*/

float ecValueAverage[SCOUNT];  // store the EC values in the array to get an average value
int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, averageIndex = 0, copyIndexEC = 0;
float averageVoltage = 0, ecValue= 0;

//Getting the EC from KS0429 Meter V.1 
float getEC(float temperature) {
  analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin); //read the analog value and store into the buffer
  analogBufferIndex++;
  if (analogBufferIndex == SCOUNT) {
    analogBufferIndex = 0;
  }
  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 1000U)
  {
    if (averageIndex == SCOUNT) {
      averageIndex = 0;
    }
    printTimepoint = millis();
    for (copyIndexEC = 0; copyIndexEC < SCOUNT; copyIndexEC++) {
      analogBufferTemp[copyIndexEC] = analogBuffer[copyIndexEC];
    }
    averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0); //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationVolatge = averageVoltage / compensationCoefficient; //temperature compensation
    ecValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge); //convert voltage value to ec value with european factor
    ecValueAverage[averageIndex] = ecValue;
   
    averageIndex++;
//    Serial.print("voltage:");
//    Serial.print(averageVoltage);
//    Serial.print(" temperature:");
//    Serial.print(temperature,1);
//    Serial.print("^C  EC:");
//    Serial.println(getAverage(ecValueAverage, SCOUNT),2);
    
    return getAverage(ecValueAverage, SCOUNT);
  }else{
    return ec;
    }
}

  float getAverage(float bArray[], int iFilterLen)
  {
    int bTab[iFilterLen];
    for (byte i = 0; i < iFilterLen; i++)
      bTab[i] = bArray[i];
    int j;
    float sum = 0.0;
    for (j = 0; j < iFilterLen; j++)
    {
      sum += bTab[j]; 
    }
    return sum/iFilterLen;
  }
  
  int getMedianNum(int bArray[], int iFilterLen)
  {
    int bTab[iFilterLen];
    for (byte i = 0; i < iFilterLen; i++)
      bTab[i] = bArray[i];
    int i, j, bTemp;
    for (j = 0; j < iFilterLen - 1; j++)
    {
      for (i = 0; i < iFilterLen - j - 1; i++)
      {
        if (bTab[i] > bTab[i + 1])
        {
          bTemp = bTab[i];
          bTab[i] = bTab[i + 1];
          bTab[i + 1] = bTemp;
        }
      }
    }
    if ((iFilterLen & 1) > 0)
      bTemp = bTab[(iFilterLen - 1) / 2];
    else
      bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
    return bTemp;
  }
