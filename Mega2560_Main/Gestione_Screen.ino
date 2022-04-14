/*
  Aquarium Project Pasquale
*/

//Check if the becklight of the screen should be on or off.
//if keyPad is different from zero, it will turn on the light, set the screenBeckLightOnOff
//variable 'true' and will set _timerLightDisplay with the same timerOfLight's parameter
//value. When screenBeckLightOnOff is true and _timerLightDisplay is different from zero or
//one, this value will count back until _timerLightDisplay will be equal to one in order
//to turn off screen backlight and to set screenBeckLightOnOff to false.
void Screen::checkScreenBeckLight(int keyPad, byte noTouch) {
  if (keyPad != noTouch) {
    screenBeckLightOnOff = true;
    screen->turnOnLight();
    _timerLightDisplay = timerOfLight;
  } else if (screenBeckLightOnOff && _timerLightDisplay == 1) {
    screen->turnOffLight();
    screenBeckLightOnOff = false;
    _timerLightDisplay--;
  } else if (screenBeckLightOnOff) {
    _timerLightDisplay--;
  } else {
    return ;
  }
}

//Turning On or Turning Off the selected rele and show them on screen
void Screen::manageReleSymbolAndAction(int index, int onOff) {
    if (onOff == 0){ 
      digitalWrite(rele[index], LOW);
      releSymbol[index] = 1;
    } else {
      digitalWrite(rele[index], HIGH);
      releSymbol[index] = 0;
    }
}

void Screen::releSymbolMenu(){
  showReleSymbol(releSymbol, numRele);
}

void Screen::manageAutomationProcessAndMaintenance(int itemSelected, int onOff){
  if(itemSelected == 1){
      if (onOff == 0) {
        config.autoEnabled = true; config.manteinEnabled = false;
        onAutomaticProcess();
      } else {
        config.autoEnabled = false;
        offAutomaticProcess();
      }
  }else{
      if (onOff == 0) {
        config.manteinEnabled = true; config.autoEnabled = false; 
        offAutomaticProcess();
      } else {
        config.manteinEnabled = false;
        onAutomaticProcess();
      }
  }
  saveConfiguration(filename, config);
}

void Screen::lightsTimerSetting(){
  setTimerLights(config.startHour, config.startMinutes, config.endHour, config.endMinutes);  
}

void Screen::timeSetting(){
  setHourAndMinutes(RTC.now().hour(), RTC.now().minute());
}

void Screen::recoverySetting(){
  setRecoveryHourAndMinutes(config.hourLoading, config.minLoading);
}
void Screen::dateSetting(){
  setDayMonthYear(RTC.now().day(), RTC.now().month(), RTC.now().year());
}     

void Screen::saveTime(byte *hourEMin) {
  char timeBuffer[8];
  char timeBufferSub[8];
  char dateBuffer[11];
  sprintf(timeBuffer, "%02d:%02d:00", hourEMin[0], hourEMin[1]);
  sprintf(dateBuffer, "%s %02d %04d",  monthName(RTC.now().month()), RTC.now().day(), RTC.now().year());
  substring(timeBuffer, timeBufferSub, 1,8);
  //Serial.print("Time buffer: "+String(dateBuffer)+ " "+String(timeBufferSub));
  RTC.adjust(DateTime(dateBuffer, timeBufferSub));
}

void Screen::saveDate(int *date){
  char dateBuffer[11];
  char timeBuffer[8];
  char timeBufferSub[8];
  sprintf(timeBuffer, "%02d:%02d:00", RTC.now().hour(), RTC.now().minute());
  sprintf(dateBuffer, "%s %02d %04d",  monthName(date[1]), date[0], date[2]);
  substring(timeBuffer, timeBufferSub, 1,8);
  //Serial.println("Date buffer: " + String(dateBuffer) + " " + String(timeBuffer));
  RTC.adjust(DateTime(dateBuffer, timeBufferSub));
}

void Screen::saveTimerLights(byte *hourAndMin) {    
  config.startHour = hourAndMin[0];
  config.startMinutes = hourAndMin[1];
  config.endHour = hourAndMin[2];
  config.endMinutes = hourAndMin[3];
  saveConfiguration(filename, config);
}

void Screen::saveRecoveryTime(byte *hourAndMin) {    
  config.hourLoading = hourAndMin[0];
  config.minLoading = hourAndMin[1];
  saveConfiguration(filename, config);
}
void Screen::manageSettingsSelections(int item, int colItem){
  switch (item) {
    case 7: if (colItem == 0) {
        config.onOffTemperatureSending = true;
        config.onOffTemperature = true;
      } else {
        config.onOffTemperatureSending = false;
        config.onOffTemperature = false;
      }
      break;
    case 8: if (colItem == 0) {
        config.onOffECSending = true;
        config.onOffEC = true;
      } else {
          config.onOffECSending = false;
          config.onOffEC = false;
      }
      break;
    case 9: if (colItem == 0) {
        config.onOffPhSending = true;
        config.onOffPH = true;
      } else {
        config.onOffPhSending = false;
        config.onOffPH = false;
      }
      break;
    case 10:
      if (colItem == 0 && !config.onOffTemperatureSending) {
        config.onOffTemperatureSending = true;
      } else if (colItem == 0 && config.onOffTemperatureSending) {
        config.onOffTemperatureSending = false;
      } 
      break;
    case 11:
      if (colItem == 0 && !config.onOffECSending) {
        config.onOffECSending = true;
      } else if (colItem == 0 && config.onOffECSending) {
        config.onOffECSending = false;
      } 
      break;
    case 12:
      if (colItem == 0 && !config.onOffPhSending) {
        config.onOffPhSending = true;
      } else if (colItem == 0 && config.onOffPhSending) {
        config.onOffPhSending = false;
      } 
      break;
    default: break;
  }
  saveConfiguration(filename, config);
}

void Screen::actualTemperatureRange(){
  setTemperatureRange(config.tempMin, config.tempMax);
}

void Screen::yesOrNoSelection4(byte selectedItem, int rowNumber, String *listMenu){
  showYesOrNoSelection4(colItem, rowItemSettingMenu, 
                        config.onOffTemperatureSending, config.onOffECSending, config.onOffPhSending); 
}

void Screen::saveMinMaxTemperature(byte *minAndMax) {
  config.tempMin = minAndMax[0];
  config.tempMax = minAndMax[1];
  saveConfiguration(filename, config);
}

void Screen::saveFreqUpdateWeb() {
  Serial.println(freqUpdateWebTemperatureIndex);
  Serial.println(setFreq(freqUpdateWebTemperatureIndex));
        
  config.freqUpdateWebTemperature = setFreq(freqUpdateWebTemperatureIndex);
  config.freqUpdateWebEC = setFreq(freqUpdateWebECIndex);
  config.freqUpdateWebPH = setFreq(freqUpdateWebPHIndex);
  saveConfiguration(filename, config);
}

void Screen::freqNumberIndex(){
  Serial.println(config.freqUpdateWebTemperature);
  Serial.println(indexNumber(config.freqUpdateWebTemperature));
  freqUpdateWebTemperatureIndex = indexNumber(config.freqUpdateWebTemperature);
  freqUpdateWebECIndex = indexNumber(config.freqUpdateWebEC);
  freqUpdateWebPHIndex = indexNumber(config.freqUpdateWebPH);
}

void Screen::reconnectMenu(){
  reconnectWifi();
}

void Screen::sentConnectionRequest(){
  showInfoWifi(); 
}
