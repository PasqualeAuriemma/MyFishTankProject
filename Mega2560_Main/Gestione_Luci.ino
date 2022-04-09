/*
  Aquarium Project Pasquale
*/

bool tempOnOffLight = false;

//Managing the right time to turn on the light in the aquarium.
//Compare the turning on lights time with the real time in minutes to turn on or turn off the
//light of the fish tank. If the start time is greater than the end time, it will  be added a
//day in minutes to end time and real time in order to match the end time with the next day.

void manageAquariumLights(int h, int m) {
  int checkStart = 0, checkEnd = 0, tempTime = 0;

  if ((config.startHour > config.endHour) || (config.startHour == config.endHour &&
      config.startMinutes > config.endMinutes)) {
    checkStart = (config.startHour * 60) + config.startMinutes;
    checkEnd = (config.endHour * 60 + (24 * 60)) + config.endMinutes;
    if (h < config.endHour || (h == config.endHour && m <= config.endMinutes)) {
      tempTime = ((h * 60) + (24 * 60)) + m;
    } else {
      tempTime = (h * 60) + m;
    }
  } else {
    checkStart = (config.startHour * 60) + config.startMinutes;
    checkEnd   = (config.endHour * 60) + config.endMinutes;
    tempTime   = (h * 60) + m;
  }

  if (checkStart <= tempTime && checkEnd > tempTime && !tempOnOffLight) {
    tempOnOffLight = true;
    manageReleSymbolAndAction(0, 0);
    screen->releSymbolMenu();
  } else if (!(checkStart <= tempTime && checkEnd > tempTime ) && tempOnOffLight) {
    tempOnOffLight = false;
    manageReleSymbolAndAction(0, 1);
    screen->releSymbolMenu();
  }

  //  Serial.println("orario timer per illuminazione");
  //  Serial.println("ORA ACCENSIONE = " + String(config.startHour));
  //  Serial.println("MINUTI ACCENSIONE = " + String(config.startMinutes));
  //  Serial.println("ORA SPEGNIMENTO = " + String(config.endHour));
  //  Serial.println("MINUTI SPEGNIMENTO = " + String(config.endMinutes));
  //  Serial.println("------------------------------");

}
