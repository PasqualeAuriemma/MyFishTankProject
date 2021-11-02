/*
  Aquarium Project Pasquale
*/

//The function argument pivot is very important because it decide how many times to send
//the value to web site. For example, if the pivot is four the value will be sent to web
//site eight time. The seconds should be equal to result of division between hour
//and pivot but different from last second when it is sent the value. The division and
//the last second value are used to run the function once per second, because the function
//is called four time in a second.  The result is to choose the right time to sent the
//value on the web site.
void sendValueToWeb(float value, String key, DateTime now) {
  String timeStamp = String(now.unixtime());
  String payload = "";
  String fileName = key + "_v.jso";
  saveValue(fileName, value, timeStamp, key);
  payload += key;
  payload += "=";
  payload += value;
  payload += ":";
  payload += "Date=";
  payload +=  timeStamp;
  payload += ";";
  //Serial.println(payload);
  Serial3.println(payload);
}

void chackIfSendTempValue(byte pivot, byte Minute, float value, String key, DateTime now) {
  if (key == keyTemp && value != -1000.00 && value != 0.0) {
    if (H % pivot == 0 && M == Minute && S == int(H / pivot) && S != previousSecSend) {
      sendValueToWeb(value, key, now);
      previousSecSend = S;
    }
  }
}

void chackIfSendTDSValue(byte pivot, byte Minute, float value, String key, DateTime now) {
  if (key == keyTDS && int(value) != 1) {
    if (H % pivot == 0 && M == Minute && S == int(H / pivot) && S != previousSecSendTDS) {
      previousSecSendTDS = S;
      sendValueToWeb(value, key, now);
    }
  }
}

float activateTDSMeasurement(byte pivot, byte Minute, float temper) {
  if (H % pivot == 0 && M >= Minute && M <= (Minute + 2)) {
    float p = getTDS(temper);
    //Serial.println("TDS = " + String(p));
    return p;
    
  }
}


void manageAutomationProcessAndMaintenance(int item, int colItem) {
  switch (item) {
    case 0:
      if (colItem == 0) {
        config.autoEnabled = true;
        config.manteinEnabled = false;
        onAutomaticProcess();
      } else {
        config.autoEnabled = false;
        offAutomaticProcess();
      }
      break;
    case 2:
      if (colItem == 0) {
        config.manteinEnabled = true; config.autoEnabled = false; offAutomaticProcess();
      } else {
        config.manteinEnabled = false;
        onAutomaticProcess();
      }
      break;
    default: break;
  }
  saveConfiguration(filename, config);
}

void manageSendingSettings(int item, int colItem) {
  switch (item) {
    case 6:
      if (colItem == 0) {
        config.onOffTemperatureSending = true;
      } else {
        config.onOffTemperatureSending = false;
      }
      break;
    case 7:
      if (colItem == 0) {
        config.onOffTDSSending = true;
      } else {
        config.onOffTDSSending = false;
      }
      break;
    case 8:
      if (colItem == 0) {
        config.onOffPhSending = true;
      } else {
        config.onOffPhSending = false;
      }
      break;
    default: break;
  }
  saveConfiguration(filename, config);
}

//
void onAutomaticProcess() {
  config.onOffLightAuto = true;
  config.onOffHeater = true;
  config.onOffTDS = true;
  config.onOffPH = true;
  config.onOffTemperature = true;
  config.onOffTemperatureSending = true;
  config.onOffTDSSending = true;
  config.onOffPhSending = true;
  //Turning on Fish Feeder and Filter
  manageReleSymbolAndAction(0, 0);
  delay(500);
  manageReleSymbolAndAction(1, 0);
  delay(500);
}

//
void offAutomaticProcess() {
  config.onOffLightAuto = false;
  config.onOffHeater = false;
  config.onOffTDS = false;
  config.onOffPH = false;
  config.onOffTemperature = false;
  config.onOffTemperatureSending = false;
  config.onOffTDSSending = false;
  config.onOffPhSending = false;
  //Turning off Fish Feeder, Filter, Termometer, Ossigeno and turning on Lights
  if (releSymbol[0] == 1) {
    manageReleSymbolAndAction(0, 1);
    delay(500);
  }
  if (releSymbol[1] == 1) {
    manageReleSymbolAndAction(1, 1);
    delay(500);
  }
  if (releSymbol[2] == 1) {
    manageReleSymbolAndAction(2, 1);
    delay(500);
  }
  if (releSymbol[3] == 1) {
    manageReleSymbolAndAction(3, 1);
    delay(500);
  }
  if (releSymbol[4] == 0) {
    manageReleSymbolAndAction(4, 0);
    delay(500);
  }
  //for(int i=0; i<numRele; i++){
  //  Serial.println(releSymbol[i]);
  //}
}

//Turning On or Turning Off the selected rele and show them on screen LCD
void manageReleSymbolAndAction(byte index, int onOff) {
  lcd.clear();
  lcd.setCursor((startReleSimbolsOnLCD + index ), 0);
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
