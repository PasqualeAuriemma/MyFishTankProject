/*
  Aquarium Project Pasquale
*/

//// Controllo di eventi sulla porta Serial3
void serialEvent3() {
  while (Serial3.available()) {
    // Lettura dei dati su porta Serial3
    char inChar = Serial3.read();
    // Trasmissione dei dati letti sulla porta Serial
    Serial.write(inChar);
    // Cerca un comando nei dati ricevuti (il comando deve essere tra parentesi quadre)
    inString += inChar;
    if (inChar == ']') {
      if (inString.indexOf("[ON_1]") > 0) {
        digitalWrite(rele[0], LOW);
      } else if (inString.indexOf("[OFF_1]") > 0) {
        digitalWrite(rele[0], HIGH);
      } else if (inString.indexOf("[ON_2]") > 0) {
        digitalWrite(rele[1], LOW);
      } else if (inString.indexOf("[OFF_2]") > 0) {
        digitalWrite(rele[1], HIGH);
      } else if (inString.indexOf("[ON_3]") > 0) {
        digitalWrite(rele[2], LOW);
      } else if (inString.indexOf("[OFF_3]") > 0) {
        digitalWrite(rele[2], HIGH);
      } else if (inString.indexOf("[ON_4]") > 0) {
        digitalWrite(rele[3], LOW);
      } else if (inString.indexOf("[OFF_4]") > 0) {
        digitalWrite(rele[3], HIGH);
      } else if (inString.indexOf("Configured:[SSID]") > 0) {
        connStatus = "SSID";
      } else if (inString.indexOf("Connection:[OK]") > 0) {
        connStatus = "OK";
      } else if (inString.indexOf("Connection:[KO]") > 0) {
        connStatus = "KO";
      } else if (inString.indexOf("Connection:[LOST]") > 0) {
        connStatus = "LOST";
      }else if (inString.indexOf("[200_ans]") > 0) {
        ("Sended");    
      }else if (inString.indexOf("_ans]") > 0) {
        Serial.println(inString);
        byte dayAfter = D + 1;
        String msg = splitString(inString, '[', 0);
        String keyValue = splitString(msg, ';', 0);
        String timest = splitString(msg, ';', 1);
        String key = splitString(keyValue, ':', 0);
        String value = splitString(keyValue, ':', 1);    
        String filename = "temp_" + String(dayAfter) + ".txt";
        saveValue(filename, value, timest, key);
      } else {
        ("Wrong command");
      }
      inString = "";
    }
  }
}

void reconnectWifi() {
  String payload = "";
  payload += "Restart;";
  //Serial.println(payload);
  Serial3.println(payload);
}

void showInfoWifi() {
  String payload = "";
  payload += "ConInfo;";
  Serial3.println(payload);
  while (Serial3.available()) {
    char inChar = Serial3.read();
    connStatus += inChar;
  }
}

//Split the read message to process it
String splitString(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
