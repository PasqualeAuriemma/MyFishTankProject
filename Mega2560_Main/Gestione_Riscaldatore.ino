/*
  Aquarium Project Pasquale
*/

bool tempOnOffHeater = false;

//Checking when turn on or turn off the thermometer into aquarioum
void manageAcquarioumHeater(int temperature) {
  if (temperature <= config.tempMin && !tempOnOffHeater &&
      temperature != -1000 && temperature != 0) {
    tempOnOffHeater = true;
    manageReleSymbolAndAction(2, 0);
    screen->releSymbolMenu();
  } else if ((temperature > config.tempMax && tempOnOffHeater)) {
    tempOnOffHeater = false;
    manageReleSymbolAndAction(2, 1);
    screen->releSymbolMenu();
  }
}
