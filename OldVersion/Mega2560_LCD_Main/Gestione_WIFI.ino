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
      } else if (inString.indexOf("[ON_5]") > 0) {
        digitalWrite(rele[4], LOW);
      } else if (inString.indexOf("[OFF_5]") > 0) {
        digitalWrite(rele[4], HIGH);
      } else if (inString.indexOf("Configured:[SSID]") > 0) {
        connStatus = "Connection SSID";
      } else if (inString.indexOf("Connection:[OK]") > 0) {
        connStatus = "Connection OK";
      } else if (inString.indexOf("Connection:[KO]") > 0) {
        connStatus = "Connection KO";
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
