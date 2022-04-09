/*
  Aquarium Project Pasquale
*/

//The function argument pivot is very important because it decides how many times to send
//the value to web site. For example, if the pivot is four the value will be sent to web
//site eight time. The seconds should be equal to result of division between hour
//and pivot but different from last second when it is sent the value. The division and
//the last second value are used to run the function once per second, because the function
//is called four time in a second.  The result is to choose the right time to sent the
//value on the web site.
void sendValueToWeb(float value, String key, DateTime now) {
  String timeStamp = String(now.unixtime());
  String payload = "";
  //String fileName = key + "_v.jso";
  //saveValue(fileName, value, timeStamp, key);
  
  payload += key;
  payload += "=";
  payload += value;
  payload += ":";
  payload += "Date=";
  payload +=  timeStamp;
  payload += ";";
  Serial.println(payload);
  Serial3.println(payload);
}

void resendValueToWeb(String value, String key,  String timeStamp) {
  String payload = "";
  payload += key;
  payload += "=";
  payload += value;
  payload += ":";
  payload += "Date=";
  payload +=  timeStamp;
  payload += ";";
  Serial.println(payload);
  Serial3.println(payload);
}

void chackWhenResendMeasure(byte _hour, byte _minute) {
    if (H == _hour && M == _minute && S != previousSecResend) {
    loadingNotSendedMeasure();
    previousSecResend = S;
    }
}

void chackIfSendTempValue(byte pivot, byte Minute, float value, String key, DateTime now) {
  if (key == keyTemp && value != -1000.00 && value != 0.0) {
    if (H % pivot == 0 && M == Minute && S == int(H / pivot) && S != previousSecSend) {
      sendValueToWeb(value, key, now);
      previousSecSend = S;
    }
  }
}

void chackIfSendECValue(byte pivot, byte Minute, float value, String key, DateTime now) {
  if (key == keyEC && int(value) != 1) {
    if (H % pivot == 0 && M == Minute && S == int(H / pivot) && S != previousSecSendEC) {
      previousSecSendEC = S;
      sendValueToWeb(value, key, now);
    }
  }
}

void chackIfSendPHValue(byte pivot, byte Minute, float value, String key, DateTime now) {
  if (key == keyPh && int(value) != 1) {
    if (H % pivot == 0 && M == Minute && S == int(H / pivot) && S != previousSecSendPH) {
      previousSecSendPH = S;
      sendValueToWeb(value, key, now);
    }
  }
}

// Get EC value
float activateECMeasurement(byte pivot, byte Minute, float temper) {
  if (H % pivot == 0 && M >= Minute && M <= (Minute + 2)) {
    return getEC(temper);
  }else{
    return ec;
  }
}

// Get PH value
float activatePHMeasurement(byte pivot, byte Minute, float temper) {
  if (H % pivot == 0 && M >= Minute && M <= (Minute + 3)) {
    float phV = getPH(temper);
    //Serial.println("PH = " + String(phV));
    return phV;
  }else{
    return phFinal;
  }
}


void manageAutomationProcessAndMaintenance(int item, int colItem) {
  switch (item) {
    case 1:
      if (colItem == 0) {
        config.autoEnabled = true; config.manteinEnabled = false;
        onAutomaticProcess();
      } else {
        config.autoEnabled = false;
        offAutomaticProcess();
      }
      break;
    case 2:
      if (colItem == 0) {
        config.manteinEnabled = true; config.autoEnabled = false; 
        offAutomaticProcess();
      } else {
        config.manteinEnabled = false;
        onAutomaticProcess();
      }
      break;
    default: break;
  }
  saveConfiguration(filename, config);
}

void manageSettingsSelections(int item, int colItem) {
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

//
void onAutomaticProcess() {
  if(onOffLightAuto){config.onOffLightAuto = true;}
  if(onOffHeater){config.onOffHeater = true;}
  if(onOffEC){config.onOffEC = true;}
  if(onOffPH){config.onOffPH = true;}
  if(onOffTemperature){config.onOffTemperature = true;}
  if(onOffTemperatureSending){config.onOffTemperatureSending = true;}
  if(onOffECSending){config.onOffECSending = true;}
  if(onOffPhSending){config.onOffPhSending = true;}
  manageReleSymbolAndAction(1, 0);
 
}

//
void offAutomaticProcess() {
  config.onOffLightAuto = false;
  config.onOffHeater = false;
  config.onOffEC = false;
  config.onOffPH = false;
  config.onOffTemperature = false;
  config.onOffTemperatureSending = false;
  config.onOffECSending = false;
  config.onOffPhSending = false;
  //Turning off Filter, Termometer, Ossigeno and turning on Lights
  if (releSymbol[0] == 0) {
    manageReleSymbolAndAction(0, 0);
  }
  delay(1000);
  if (releSymbol[1] == 1) {
    manageReleSymbolAndAction(1, 1);
  }
  delay(1000);
  if (releSymbol[2] == 1) {
    manageReleSymbolAndAction(2, 1);
  }
  delay(1000);
  if (releSymbol[3] == 1) {
    manageReleSymbolAndAction(3, 1);
  }
  screen->releSymbolMenu();
}

//Turning On or Turning Off the selected rele and show them on screen
void manageReleSymbolAndAction(byte index, int onOff) {
  if (onOff == 0) {
    digitalWrite(rele[index], LOW);
    releSymbol[index] = 1;
  } else {
    digitalWrite(rele[index], HIGH);
    releSymbol[index] = 0;
  }
}

void manageTemperatureActiv(int rowItemManualMenu, int colItem) {
  if (colItem == 0) {
    config.onOffTemperature = true;
  } else {
    config.onOffTemperature = false;
  }
  saveConfiguration(filename, config);
}
