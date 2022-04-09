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
    Serial.println(F("Failed to read file, using default configuration"));
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
    Serial.println(F("Failed to create file"));
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
  doc["hostname"] = config.hostname;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
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
    Serial.println(F("Failed to create file"));
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
  strlcpy(config.hostname,                  // <- destination
          doc["hostname"] | "example.com",  // <- source
          sizeof(config.hostname));         // <- destination's capacity

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();
}

// Prints the content of a file to the Serial
void printFile(const char *filename) {
  // Open file for reading
  File file = SD.open(filename);
  if (!file) {
    Serial.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) {
    Serial.print((char)file.read());
  }

  // Close the file
  file.close();
}

// Saves the configuration to a file
void saveValue(const String filename, const float value, const String timeStamp, const String key) {
  // Delete existing file, otherwise the configuration is appended to the file
  //SD.remove(filename);

  // Open file for writing
  File file = SD.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }
  //Serial.println("STO SALVANDO!!!");
  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<256> doc;

  // Set the values in the document
  doc[key] = value;
  doc["timestamp"] = timeStamp;

  // Serialize JSON to file
  if (file) {
    WriteBufferingStream bufferedFile(file, 64);
    serializeJson(doc, bufferedFile);
    bufferedFile.flush();
    file.println("\r\n");
    // close the file:
    file.close();
  } else {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  //file.close();
}
