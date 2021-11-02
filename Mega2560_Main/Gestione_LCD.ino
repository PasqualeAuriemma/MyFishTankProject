/*
  Aquarium Project Pasquale
*/

char activationString[maxSize] = "Vuoi Attivare?";
char chooseString[maxSize] = "Scegli ON o OFF";
char waitingString[maxSize] = "Attendere...";
char siString[maxSize] = "SI";
char noString[maxSize] = "NO";
char onString[maxSize] = "ON";
char offString[maxSize] = "OFF";
int rowItem = 0, colItem = 0, rowItemMainMenu = 0, rowItemManualMenu = 0, rowItemSettingMenu = 0;

void initScreen() {
  lcd.clear();
  //Set cursor at up left corner + 3 cells.
  lcd.setCursor (3, 0);
  lcd.print("Acquario di");
  //Set cursor at bottom left corner + 4 cells.
  lcd.setCursor ( 4, 1 );
  lcd.print("Pasquale");
}

// LCD screen with time and temperature
void mainScreen() {
  lcd.setCursor(0, 0); lcd.print(buffer);
  lcd.setCursor (5, 1); lcd.print(int(tds));
  lcd.setCursor (8, 1); lcd.print("ppm");
  lcd.setCursor (11, 1); lcd.print(" ");
  lcd.setCursor (12, 1); lcd.print(temperature);
  lcd.setCursor (14, 1); lcd.write(byte(2));
  lcd.setCursor (15, 1); lcd.print(" ");
}

//Showing Menu
// It is the main function to show the menu. The most important variables are 'activeMenu' and 'changingPage'
// that they allow to change menu page. When menuOnOff is true the main menu page is showed ed it is possible
// to select some macro settings. If a macro setting is selected, you can see its section with all the voice
// that you can manage. In little speech, you can change the page changing the activeMenu value. With the use
// of all the variable values, every function will be customable for the specific action. It is very important
// to know what is the row and the column of the menu in each option menu because they led the choice of the
// action.
void menu_(int keyPad) {
  if (keyPad == 0 && !changingPage) {
    return ;
  }
  if (changingPage) {
    changingPage = false;
  }
  switch (activeMenu) {
    case 1: //Serial.println("Main Menu...");
      manageMenuCursors(keyPad, mainMenuSize, -1, 2);
      showRowItemMenu(mainMenuSize, rowItem, mainMenu);
      if (colItem == -1) {
        exitFromMenu();
      } else if (colItem == 1) {
        changingPage = true;
        switch (rowItem) {
          case 0: activeMenu = 5; break;
          case 1: activeMenu = 2; break;
          case 2: activeMenu = 5; break;
          case 3: activeMenu = 3; break;
          default: break;
        }
        rowItemMainMenu = rowItem; rowItem = 0; colItem = 0;
      }
      break;
    case 2: //Serial.println("Menu Manual...");
      manageMenuCursors(keyPad, manualMenuSize, -1, 2);
      showRowItemMenu(manualMenuSize, rowItem, manualMenu);
      if (colItem == -1) {
        setChangingPageVariable(1, rowItemMainMenu);
      } else if (colItem == 1) {
        if (rowItem >= 8 && rowItem <= 10) {
          activeMenu = 9;
        } else {
          activeMenu = 4;
        }
        changingPage = true; rowItemManualMenu = rowItem; rowItem = 0; colItem = 0;
      }
      break;
    case 3: //Serial.println("Menu Setting...");
      manageMenuCursors(keyPad, settingMenuSize, -1, 2);
      showRowItemMenu(settingMenuSize, rowItem, settingMenu);
      if (colItem == -1) {
        setChangingPageVariable(1, rowItemMainMenu);
      } else if (colItem == 1) {
        switch (rowItem) {
          case 0: activeMenu = 6; break;
          case 1: activeMenu = 6; break;
          case 2: activeMenu = 10; showInfoWifi(); break;
          case 3: waitingActionMenu(); reconnectWifi(); exitFromMenu();
          case 4: activeMenu = 11; break;
          default: break;
        }
        changingPage = true; rowItemSettingMenu = rowItem; rowItem = 0; colItem = 0;
      }
      break;
    case 4: //Serial.println("Menu Accendi o Spegni...");
      manageMenuCursors(keyPad, 1, -1, 2);
      if (colItem == -1) {
        setChangingPageVariable(2, rowItemManualMenu);
        break;
      }
      menuPageSelection(colItem, 0, activeMenu);
      if (keyPad == code[4]) {
        manageManualSelection(rowItemManualMenu, colItem);
        break;
      } else {
        break;
      }
    case 5: //Serial.println("Menu Si O No...");
      manageMenuCursors(keyPad, 1, -1, 2);
      if (colItem == -1) {
        setChangingPageVariable(1, rowItemMainMenu);
        break;
      }
      menuPageSelection(colItem, 0, activeMenu);
      if (keyPad == code[4]) {
        waitingActionMenu();
        manageAutomationProcessAndMaintenance(rowItemMainMenu, colItem);
        exitFromMenu();
      }
      break;
    case 6: //Serial.println("Menu show time...");
      manageMenuCursors(keyPad, 1, -1, 2);
      if (colItem == -1) {
        setChangingPageVariable(3, rowItemSettingMenu);
        break;
      }
      if (rowItemSettingMenu == 0) {
        showLightsTimerSetting();
      } else {
        showTime();
      }
      if (colItem == 1) {
        setChangingPageVariable(7, 0);
      }
      break;
    case 7: //Serial.println("Menu set start time or real time...");
      manageMenuCursors(keyPad, 4, 1, 1);
      switch (rowItemSettingMenu) {
        case 0:
          shiftHourAndMinutesMenu(0, 1, keyPad, "Start", "Time", rowItem, oraEMinTemp);
          if (keyPad == code[4]) {
            if (rowItem == 2) {
              setChangingPageVariable(8, rowItemSettingMenu);
              break;
            } else if (rowItem == 3) {
              setChangingPageVariable(3, rowItemSettingMenu);
              break;
            } else {
              break;
            }
          }
          break;
        case 1:
          shiftHourAndMinutesMenu(0, 1, keyPad, "Real", "Time", rowItem, oraEMinTimeClockTemp);
          if (keyPad == code[4]) {
            if (rowItem == 2) {
              saveTime(oraEMinTemp);
            } else if (rowItem == 3) {
              setChangingPageVariable(3, rowItemSettingMenu);
            } else {
              break;
            }
          }
          break;
        default: break;
      }
      break;
    case 8: //Serial.println("Menu set end time...");
      manageMenuCursors(keyPad, 4, 1, 1);
      shiftHourAndMinutesMenu(2, 3, keyPad, "End", "Time", rowItem, oraEMinTemp);
      if (keyPad == code[4]) {
        if (rowItem == 2) {
          saveTimerLights(oraEMinTemp);
        } else if (rowItem == 3) {
          setChangingPageVariable(3, rowItemSettingMenu);
        } else {
          break;
        }
      }
      break;
    case 9: //Serial.println("Menu Si O No Setting...");
      manageMenuCursors(keyPad, 1, -1, 2);
      if (colItem == -1) {
        setChangingPageVariable(2, rowItemManualMenu);
        break;
      }
      menuPageSelection(colItem, rowItemManualMenu, activeMenu);
      if (keyPad == code[4]) {
        waitingActionMenu();
        manageSendingSettings(rowItemManualMenu, colItem);
        exitFromMenu();
      }
      break;
    case 10: //Serial.println("Show connection Info...");
      waitingActionMenu();
      connectionStatusPage();
      exitFromMenu();
      break;
    case 11: //Serial.println("Menu show time...");
      manageMenuCursors(keyPad, 1, -1, 2);
      if (colItem == -1) {
        setChangingPageVariable(2, rowItemManualMenu);
        break;
      }
      showTemperatureSetting();
      if (colItem == 1) {
        setChangingPageVariable(12, 0);
      }
      break;
    case 12: //Serial.println("Menu set temperature...");
      manageMenuCursors(keyPad, 4, 1, 1);
      shiftTemperatureMenu(keyPad, "setGrad", rowItem, temperatureMinAndMaxTemp);
      if (keyPad == code[4]) {
        if (rowItem == 2) {
          saveMinMaxTemperature(temperatureMinAndMaxTemp);
        } else if (rowItem == 3) {
          setChangingPageVariable(2, rowItemManualMenu);
        } else {
          break;
        }
      }
      break;
    default: break;
  }
}

void setChangingPageVariable(int menuPage, int row) {
  activeMenu = menuPage;
  rowItem = row;
  colItem = 0;
  changingPage = true;
}

// Showing row item with selecting symbol in circle way
void showRowItemMenu(int listSize, int rowItem, char **listMenu) {
  if (listSize % 2 == 0) {
    if (rowItem % 2 == 0) {
      menuPage(listMenu[rowItem], listMenu[rowItem + 1], 1);
    } else {
      menuPage(listMenu[rowItem - 1], listMenu[rowItem], 2);
    }
  } else {
    if (rowItem == listSize - 1) {
      menuPage(listMenu[rowItem], listMenu[0], 1);
    } else if (rowItem % 2 == 0) {
      menuPage(listMenu[rowItem], listMenu[rowItem + 1], 1);
    } else {
      menuPage(listMenu[rowItem - 1], listMenu[rowItem], 2);
    }
  }
}

void manageManualSelection(int rowItemManualMenu, int colItem) {
  if (rowItemManualMenu < 5) {
    waitingActionMenu();
    manageReleSymbolAndAction(rowItemManualMenu, colItem);
  } else {
    switch (rowItemManualMenu) {
      case 5: if (colItem == 0) {
          config.onOffTemperature = true;
        } else {
          config.onOffTemperature = false;
        }
      case 6: if (colItem == 0) {
          config.onOffTDS = true;
        } else {
          config.onOffTDS = false;
        }
      case 7: if (colItem == 0) {
          config.onOffPH = true;
        } else {
          config.onOffPH = false;
        }

        break;
      default: break;
    }
  }
  exitFromMenu();
}

void exitFromMenu() {
  rowItem = 0, colItem = 0, rowItemMainMenu = 0, rowItemManualMenu = 0, rowItemSettingMenu = 0;
  menuOnOff = false;
  lcd.setCursor (0, 0);
  lcd.print("          ");
  lcd.setCursor (0, 1);
  lcd.print("                ");
  showReleSymbol();
}

// Managing the cursor to move inside the menu
void manageMenuCursors(int keyPad, int menuSize, int minCol, int maxCol) {
  switch (keyPad) {
    case 1:
      if (colItem != maxCol - 1) {
        colItem++;
      }
      break;
    case 2:
      if (colItem != minCol) {
        colItem--;
      }
      break;
    case 3:
      if (rowItem == 0) {
        rowItem = menuSize - 1;
      } else {
        rowItem--;
      }
      break;
    case 4:
      if (rowItem == menuSize - 1) {
        rowItem = 0;
      } else {
        rowItem++;
      }
      break;
    default: break;
  }
}

//Showing menu page where there are the options to choose
void menuPageSelection(int arrowItem, int row, int whatMenu) {
  lcd.clear();
  if (whatMenu == 5) {
    //Set cursor at up left corner + 1 cells.
    lcd.setCursor(1, 0);
    lcd.print(activationString);
    //  Set cursor at bottom left corner + 2 cells or 7 cells and 5 cells or 10 cells.
    lcd.setCursor((arrowItem * 5) + 2, 1); lcd.write(byte(6));
    lcd.setCursor(((arrowItem + 1) * 5), 1); lcd.write(byte(5));
    //  Set cursor at bottom left corner + 3 cells.
    lcd.setCursor(3, 1); lcd.print(siString);
    //  Set cursor at bottom left corner + 8 cells.
    lcd.setCursor(8, 1); lcd.print(noString);
  } else if (whatMenu == 4) {
    //  Set cursor at up left corner.
    lcd.setCursor(0, 0);
    lcd.print(chooseString);
    //  Set cursor at bottom left corner + 2 cells or 6 cells and 9 cells or 13 cells.
    lcd.setCursor((arrowItem * 7) + 2, 1); lcd.write(byte(6));
    lcd.setCursor((arrowItem * 7) + 6, 1); lcd.write(byte(5));
    //  Set cursor at bottom left corner + 3 cells.
    lcd.setCursor(3, 1); lcd.print(onString);
    //  Set cursor at bottom left corner + 8 cells.
    lcd.setCursor(10, 1); lcd.print(offString);
  } else if (whatMenu == 9) {
    lcd.setCursor(0, 0);
    Serial.println(row);
    if (row == 6) {
      lcd.print("Send Temp is");
    } else if (row == 7) {
      lcd.print("Send EC is");
    } else if (row == 8) {
      lcd.print("Send PH is");
    }
    lcd.setCursor(13, 0);
    if (row == 6) {
      lcd.print(enabledToSend(config.onOffTemperatureSending));
    } else if (row == 7) {
      lcd.print(enabledToSend(config.onOffTDSSending));
    } else if (row == 8) {
      lcd.print(enabledToSend(config.onOffPhSending));
    }
    lcd.setCursor((arrowItem * 7) + 2, 1); lcd.write(byte(6));
    lcd.setCursor((arrowItem * 7) + 6, 1); lcd.write(byte(5));
    //  Set cursor at bottom left corner + 3 cells.
    lcd.setCursor(3, 1); lcd.print(onString);
    //  Set cursor at bottom left corner + 8 cells.
    lcd.setCursor(10, 1); lcd.print(offString);
  }
}

//Used to get the right status in order to insert into LCD Screen
String enabledToSend(bool item) {
  if (item) {
    return "ON";
  } else {
    return "OFF";
  }
}

//Showing connection status on LCD screen
void connectionStatusPage() {
  lcd.clear();
  lcd.setCursor(1, 0); lcd.print("Conn. Status:");
  lcd.setCursor(1, 1); lcd.print(connStatus);
  delay(2000);
}

//Showing menu page with the row items and their selecting symbol
void menuPage(char menuItem1[maxSize], char menuItem2[maxSize], int arrowItem) {
  lcd.clear();
  //Set cursor at up or bottom left corner.
  lcd.setCursor(0, (arrowItem - 1));
  lcd.write(byte(6));
  //Set cursor at up left corner + 2 cells.
  lcd.setCursor(2, 0);
  lcd.print(menuItem1);
  //Set cursor at bottom left corner + 2 cells.
  lcd.setCursor(2, 1);
  lcd.print(menuItem2);
}

//Waiting animaction
void waitingActionMenu() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print(waitingString);
  for (int i = 0; i < maxSize; i++) {
    lcd.setCursor(i, 1);
    lcd.write(7);
    delay(3000 / maxSize);
  }
}

//Check if the becklight of the screen should be on or off.
//if keyPad is different of zero, it will turn on the light, set the screenBeckLightOnOff
//variable 'true' and will set _timerLightDisplay with the same timerOfLight's parameter
//value. When screenBeckLightOnOff is true and _timerLightDisplay is different of zero or
//one, this value will count back until _timerLightDisplay will be equal to one in order
//to turn off screen backlight and to set screenBeckLightOnOff to false.
void checkScreenBeckLight(int keyPad) {
  if (keyPad != code[6]) {
    screenBeckLightOnOff = true;
    lcd.backlight();
    _timerLightDisplay = timerOfLight;
  } else if (screenBeckLightOnOff && _timerLightDisplay == 1) {
    lcd.noBacklight();
    screenBeckLightOnOff = false;
    _timerLightDisplay--;
  } else if (screenBeckLightOnOff) {
    _timerLightDisplay--;
  } else {
    return ;
  }
}
// Show the rele symbols on screen LCD
void showReleSymbol() {
  lcd.clear();
  lcd.setCursor(startReleSimbolsOnLCD, 0);
  for (int p = 0; p < numRele; p++) {
    lcd.write(releSymbol[p]);
  }
  delay(500);
}

void showLightsTimerSetting() {
  oraEMinTemp[0] = config.startHour;
  oraEMinTemp[1] = config.startMinutes;
  oraEMinTemp[2] = config.endHour;
  oraEMinTemp[3] = config.endMinutes;
  showTimeTitle(0, "Set Timer Luci ");
  showStringNumber(2, 1, config.startHour);
  lcd.setCursor(4, 1); lcd.print(":");
  showStringNumber(5, 1, config.startMinutes);
  showStringNumber(9, 1, config.endHour);
  lcd.setCursor (11, 1); lcd.print(":");
  showStringNumber(12, 1, config.endMinutes);
}

void showTemperatureSetting() {
  temperatureMinAndMaxTemp[0] = config.tempMin;
  temperatureMinAndMaxTemp[1] = config.tempMax;
  showTimeTitle(0, "Set Riscalda.");
  lcd.setCursor(1, 1); lcd.print("MIN:");
  showStringNumber(5, 1, config.tempMin);
  lcd.setCursor(8, 1); lcd.print("MAX:");
  showStringNumber(12, 1, config.tempMax);
}

void showStringNumber(byte i, byte j, byte number) {
  lcd.setCursor(i, j);
  if (number < 9) {
    lcd.print("0");
    lcd.setCursor(i + 1, j); lcd.print(number);
  } else {
    lcd.print(number);
  }
}

void showTimeTitle(byte i, String title) {
  lcd.setCursor (0, 0); lcd.print("                ");
  lcd.setCursor (i, 0); lcd.print(title);
  lcd.setCursor (15, 0); lcd.write(6);
  lcd.setCursor(0, 1); lcd.print("                ");
}

void showTime() {
  oraEMinTimeClockTemp[0] = RTC.now().hour();
  oraEMinTimeClockTemp[1] = RTC.now().minute();
  //  Serial.println(RTC.now().hour());
  //  Serial.println(RTC.now().minute());
  //  Serial.println(oraEMinTimeClockTemp[0]);
  //  Serial.println(oraEMinTimeClockTemp[1]);
  showTimeTitle(1, "Set Orario");
  showStringNumber(5, 1, oraEMinTimeClockTemp[0]);
  lcd.setCursor(7, 1); lcd.print(":");
  showStringNumber(8, 1, oraEMinTimeClockTemp[1]);
}

//Managing the cursor that selects the temperature to manage the termometer
void manageTemperatureSetting(int row, String one, int h, int m) {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("MIN");
  if (row == 0) {
    lcd.setCursor (3, 0);
    lcd.write(6);
  }
  lcd.setCursor (4, 0); lcd.print(h);
  if (row == 0) {
    lcd.setCursor (6, 0);
    lcd.write(5);
  }
  lcd.setCursor (0, 1); lcd.print("MAX");
  if (row == 1) {
    lcd.setCursor (3, 1);
    lcd.write(6);
  }
  lcd.setCursor (4, 1); lcd.print(m);
  if (row == 1) {
    lcd.setCursor (6, 1);
    lcd.write(5);
  }
  lcd.setCursor(8, 0); lcd.print(one);
  if (row == 2) {
    lcd.setCursor (7, 1);
    lcd.write(6);
  }
  lcd.setCursor (8, 1); lcd.print("OK");
  if (row == 3) {
    lcd.setCursor (12, 1);
    lcd.write(6);
  }
  lcd.setCursor (13, 1); lcd.print("<-");
}

// Managing the cursor to move inside the time selection menu
void manageLightsTimerSetting(int row, String one, String two, int h, int m) {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("HH");
  if (row == 0) {
    lcd.setCursor (2, 0);
    lcd.write(6);
  }
  lcd.setCursor (3, 0); lcd.print(h);
  if (row == 0) {
    lcd.setCursor (5, 0);
    lcd.write(5);
  }
  lcd.setCursor (0, 1); lcd.print("MM");
  if (row == 1) {
    lcd.setCursor (2, 1);
    lcd.write(6);
  }
  lcd.setCursor (3, 1); lcd.print(m);
  if (row == 1) {
    lcd.setCursor (5, 1);
    lcd.write(5);
  }
  lcd.setCursor(7, 0); lcd.print(one); lcd.print(two);
  if (row == 2) {
    lcd.setCursor (7, 1);
    lcd.write(6);
  }
  lcd.setCursor (8, 1); lcd.print("OK");
  if (row == 3) {
    lcd.setCursor (12, 1);
    lcd.write(6);
  }
  lcd.setCursor (13, 1); lcd.print("<-");
}

void saveTimerLights(byte *oraEMin) {
  waitingActionMenu();
  config.startHour = oraEMin[0];
  config.startMinutes = oraEMin[1];
  config.endHour = oraEMin[2];
  config.endMinutes = oraEMin[3];
  saveConfiguration(filename, config);
  exitFromMenu();
}

void shiftTemperatureMenu(int keyPad, String firstS, int row, byte *minMax) {
  if (row == 0) {
    minMax[0] = manageMenuRangeNumberCursors(keyPad, row , minMax[0], 99);
  } else if (row == 1) {
    minMax[1] = manageMenuRangeNumberCursors(keyPad, row, minMax[1], 99);
  }
  manageTemperatureSetting(row, firstS, minMax[0], minMax[1]);
}

void shiftHourAndMinutesMenu(int i, int j, int keyPad, String firstS, String secondS, int row, byte *oraEMin) {
  if (row == 0) {
    oraEMin[i] = manageMenuRangeNumberCursors(keyPad, row , oraEMin[i], 24);
  } else if (row == 1) {
    oraEMin[j] = manageMenuRangeNumberCursors(keyPad, row, oraEMin[j], 60);
  }
  manageLightsTimerSetting(row, firstS, secondS, oraEMin[i], oraEMin[j]);
}

// Managing the cursor to move inside the menu
int manageMenuRangeNumberCursors(int keyPad, int row, int hourMin, int maxItem) {
  switch (keyPad) {
    case 1: if (hourMin == maxItem - 1) {
        return 0;
      } else {
        return ++hourMin;
      }
    case 2: if (hourMin == 0) {
        return maxItem - 1;
      } else {
        return --hourMin;
      }
    default: return hourMin;
  }
}

void saveTime(byte *oraEMin) {
  waitingActionMenu();
  char timeBuffer[8];
  sprintf(timeBuffer, "%02d:%02d:00", oraEMinTimeClockTemp[0], oraEMinTimeClockTemp[1]);
  //  Serial.println("Time buffer: " + String(timeBuffer));
  RTC.adjust(DateTime(__DATE__, timeBuffer));
  exitFromMenu();
}

void saveMinMaxTemperature(byte *minMax) {
  waitingActionMenu();
  config.tempMin = minMax[0];
  config.tempMax = minMax[1];
  saveConfiguration(filename, config);
  exitFromMenu();
}
