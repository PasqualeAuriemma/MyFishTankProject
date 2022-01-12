/*
  Aquarium Project Pasquale
*/

//Getting the PH from Gravity: Analog pH Sensor / Meter Pro Kit V2
float getPH(float temperature) {
  static unsigned long timepoint1 = millis();
    if(millis()-timepoint1>1000U){                  //time interval: 1s
        timepoint1 = millis();
        //temperature = readTemperature();         // read your temperature sensor to execute temperature compensation
        voltage = analogRead(PH_PIN)/1024.0*5000;  // read the voltage
        phValue = ph.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
        Serial.print("voltage:");
        Serial.print(voltage);
        Serial.print(" temperature:");
        Serial.print(temperature,1);
        Serial.print("^C  pH:");
        Serial.println(phValue,2);
    }
    ph.calibration(voltage,temperature);  
    return (float)phValue;
}
