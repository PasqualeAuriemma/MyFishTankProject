/*
  Aquarium Project Pasquale
*/

// Loads the configuration from a file
void loadConfiguration(const char *filename, Config &config) {
  // Open file for reading
  File file = SD.open(filename);

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<512> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println(F("Failed to read file, using default configuration in loadConfiguration!"));
    Serial.println(error.f_str());
    return;
  }

  // Copy values from the JsonDocument to the Config
  config.startHour = doc["startHour"];
  config.startMinutes = doc["startMinutes"];
  config.endHour = doc["endHour"];
  config.endMinutes = doc["endMinutes"];
  config.tempMax = doc["tempMax"];
  config.tempMin = doc["tempMin"];
  config.autoEnabled = doc["autoEnabled"];
  config.manteinEnabled = doc["manteinEnabled"];
  config.onOffLightAuto = doc["onOffLightAuto"];
  config.onOffHeater = doc["onOffHeater"];
  config.onOffEC = doc["onOffEC"];
  config.onOffPH = doc["onOffPH"];
  config.onOffTemperature = doc["onOffTemperature"];
  config.onOffFilter = doc["onOffFilter"];
  config.onOffTemperatureSending = doc["onOffTemperatureSending"];
  config.onOffECSending = doc["onOffECSending"];
  config.onOffPhSending = doc["onOffPhSending"];
  config.freqUpdateWebTemperature = doc["freqUpdateWebTemperature"];
  config.freqUpdateWebEC = doc["freqUpdateWebEC"];
  config.freqUpdateWebPH = doc["freqUpdateWebPH"];
  config.hourLoading = doc["hourLoading"];
  config.minLoading = doc["minLoading"];
  strlcpy(config.hostname,                  // <- destination
          doc["hostname"] | "example.com",  // <- source
          sizeof(config.hostname));         // <- destination's capacity

  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();
}

// Saves the configuration to a file
void saveConfiguration(const char *filename, const Config &config) {
  // Delete existing file, otherwise the configuration is appended to the file
  SD.remove(filename);

  // Open file for writing
  File file = SD.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file in saveConfiguration!"));
    return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<256> doc;

  // Set the values in the document
  doc["startHour"] = config.startHour;
  doc["startMinutes"] = config.startMinutes;
  doc["endHour"] = config.endHour;
  doc["endMinutes"] = config.endMinutes;
  doc["tempMax"] = config.tempMax;
  doc["tempMin"] = config.tempMin;
  doc["autoEnabled"] = config.autoEnabled;
  doc["manteinEnabled"] = config.manteinEnabled;
  doc["onOffLightAuto"] = config.onOffLightAuto;
  doc["onOffHeater"] = config.onOffHeater;
  doc["onOffEC"] = config.onOffEC;
  doc["onOffPH"] = config.onOffPH;
  doc["onOffTemperature"] = config.onOffTemperature;
  doc["onOffFilter"] = config.onOffFilter;
  doc["onOffTemperatureSending"] = config.onOffTemperatureSending;
  doc["onOffECSending"] = config.onOffECSending;
  doc["onOffPhSending"] = config.onOffPhSending;
  doc["freqUpdateWebTemperature"] = config.freqUpdateWebTemperature;
  doc["freqUpdateWebEC"] = config.freqUpdateWebEC;
  doc["freqUpdateWebPH"] = config.freqUpdateWebPH;
  doc["hourLoading"] = config.hourLoading;
  doc["minLoading"] = config.minLoading;
  doc["hostname"] = config.hostname;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file in saveConfiguration!"));
    return;
  }

  // Close the file
  file.close();
}

// Saves the configuration to a file
void setConfiguration(const char *filename, Config &config) {
  // Delete existing file, otherwise the configuration is appended to the file
  SD.remove(filename);

  // Open file for writing
  File file = SD.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file in setConfiguration!"));
    return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<256> doc;

  // Set the values in the document
  doc["startHour"] = 0;
  doc["startMinutes"] = 0;
  doc["endHour"] = 0;
  doc["endMinutes"] = 0;
  doc["tempMax"] = 0;
  doc["tempMin"] = 0;
  doc["autoEnabled"] = true;
  doc["manteinEnabled"] = false;
  doc["onOffLightAuto"] = false;
  doc["onOffHeater"] = false;
  doc["onOffEC"] = false;
  doc["onOffPH"] = false;
  doc["onOffTemperature"] = false;
  doc["onOffFilter"] = false;
  doc["onOffTemperatureSending"] = false;
  doc["onOffECSending"] = false;
  doc["onOffPhSending"] = false;
  doc["freqUpdateWebTemperature"] = 0;
  doc["freqUpdateWebEC"] = 0;
  doc["freqUpdateWebPH"] = 0;
  doc["hourLoading"] = 0;
  doc["minLoading"] = 3;
  doc["hostname"] = "myfishtank.altervista.org";
  config.startHour = doc["startHour"];
  config.startMinutes = doc["startMinutes"];
  config.endHour = doc["endHour"];
  config.endMinutes = doc["endMinutes"];
  config.tempMax = doc["tempMax"];
  config.tempMin = doc["tempMin"];
  config.autoEnabled = doc["autoEnabled"];
  config.manteinEnabled = doc["manteinEnabled"];
  config.onOffLightAuto = doc["onOffLightAuto"];
  config.onOffHeater = doc["onOffHeater"];
  config.onOffEC = doc["onOffEC"];
  config.onOffPH = doc["onOffPH"];
  config.onOffTemperature = doc["onOffTemperature"];
  config.onOffFilter = doc["onOffFilter"];
  config.onOffTemperatureSending = doc["onOffTemperatureSending"];
  config.onOffECSending = doc["onOffECSending"];
  config.onOffPhSending = doc["onOffPhSending"];
  config.freqUpdateWebTemperature = doc["freqUpdateWebTemperature"];
  config.freqUpdateWebEC = doc["freqUpdateWebEC"];
  config.freqUpdateWebPH = doc["freqUpdateWebPH"];
  config.hourLoading = doc["hourLoading"];
  config.minLoading = doc["minLoading"];  
  strlcpy(config.hostname,                  // <- destination
          doc["hostname"] | "example.com",  // <- source
          sizeof(config.hostname));         // <- destination's capacity

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file in setConfiguration!"));
    return;
  }

  // Close the file
  file.close();
}

// Prints the content of a file to the Serial
void printFile(const char *filename) {
  // Open file for reading
  File file = SD.open(filename);
  if (!file) {
    Serial.println(F("Failed to read file in printFile!"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) {
    Serial.print((char)file.read());
  }

  // Close the file
  file.close();
}

// Prints the content of a file to the Serial
void loadingNotSendedMeasure() {
  // Open file for reading
  String filename = "temp_" + String(D) + ".txt";
  String j2;
  File file = SD.open(filename);
  Serial.println(filename);
  if (!file) {
    Serial.println(F("Failed to read file in loadingNotSendedMeasure!"));
    return;
  }
  // Extract each characters by one by one
  while (file.available()) {
    char c =(char)file.read();
    if (c==';'){
      StaticJsonDocument<200> doc;
      DeserializationError err = deserializeJson(doc, j2);
      if (err) {
          Serial.println("dese() failed!");
          return;
      }
      j2 = "";
      const char* key = doc["key"];
      const char* value = doc["value"];
      const char* timest = doc["timestamp"]; 
      //Serial.println(key);
      //Serial.println(value);
      //Serial.println(timest);
      resendValueToWeb(value, key, timest);
      delay(10);
     }else{
       j2.concat(c);}
  }      
  //Serial.print((char)file.read());
  // Close the file
  file.close();
  Serial.println(F("romove loadingNotSendedMeasure!"));
  SD.remove(filename);
}

// Saves the configuration to a file
void saveValue(const String filename, const String value, const String timeStamp, const String key) {
  // Delete existing file, otherwise the configuration is appended to the file
  //SD.remove(filename);

  // Open file for writing
  //  File file = SD.open(filename, FILE_WRITE);

  // Try and append
  File file = SD.open(filename, O_RDWR | O_APPEND);
  if (!file) {
      // It failed, so try and make a new file.
      file = SD.open(filename, O_RDWR | O_CREAT);
      if (!file) {
          // It failed too, so give up.
          Serial.println("Failed to open" + filename + ".txt in saveValue!");
          return;
      }
  }
//  if (!file) {
//    Serial.println(F("Failed to create file"));
//    return;
//  }
  //Serial.println("STO SALVANDO!!!");
  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<256> doc;

  // Set the values in the document
  doc["key"] = key;
  doc["value"] = value;
  doc["timestamp"] = timeStamp;

  // Serialize JSON to file
  if (file) {
    WriteBufferingStream bufferedFile(file, 64);
    serializeJson(doc, bufferedFile);
    bufferedFile.flush();
    file.println(";");
    // close the file:
    file.close();
  } else {
    Serial.println(F("Failed to write to file! in saveValue"));
    return ;
  }
}
