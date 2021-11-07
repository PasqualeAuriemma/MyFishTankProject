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
          case 5: activeMenu = 14; break;
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
          setChangingPageVariable(4, rowItemManualMenu);
        } else {
          break;
        }
      }
      break;
    case 13: //Serial.println("Menu set Freq update web...");
      manageMenuCursors(keyPad, 5, 1, 1);
      shiftFreqUpdateMenu(keyPad, rowItem);
      if (keyPad == code[4]) {
        if (rowItem == 3) {
          saveFreqUpdateWeb();
        } else if (rowItem == 4) {
          setChangingPageVariable(4, rowItemManualMenu);
        } else {
          break;
        }
      }
      break;
    case 14: //Serial.println("Menu show Frequence Updating WEB...");
      manageMenuCursors(keyPad, 1, -1, 2);
      if (colItem == -1) {
        setChangingPageVariable(3, rowItemSettingMenu);
        break;
      }
      showFreqUpdateSetting();
      if (colItem == 1) {
        freqUpdateWebTemperatureIndex = indexNumber(config.freqUpdateWebTemperature);
        freqUpdateWebTDSIndex = indexNumber(config.freqUpdateWebTDS);
        freqUpdateWebPHIndex = indexNumber(config.freqUpdateWebPH);
        setChangingPageVariable(13, 0);
      }
      break;
    default: break;
  }
}

int indexNumber(int freq) {
  if (freq == 23) {
    return indexOf(24, freqNumbber, 8);
  } else {
    return indexOf(freq, freqNumbber, 8);
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
          config.onOffTemperatureSending = true;
          config.onOffTemperature = true;
        } else {
          config.onOffTemperatureSending = true;
          config.onOffTemperature = false;
        }
        break;
      case 6: if (colItem == 0) {
          config.onOffTDSSending = true;
          config.onOffTDS = true;
        } else {
          config.onOffTDSSending = false;
          config.onOffTDS = false;
        }
        break;
      case 7: if (colItem == 0) {
          config.onOffPhSending = true;
          config.onOffPH = true;
        } else {
          config.onOffPhSending = false;
          config.onOffPH = false;
        }
        break;
      default: break;
    }
    saveConfiguration(filename, config);
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
    if (row == 8) {
      lcd.print("Send Temp is");
      lcd.setCursor(13, 0);
      lcd.print(enabledToSend(config.onOffTemperatureSending));
    } else if (row == 9) {
      lcd.print("Send EC is");
      lcd.setCursor(13, 0);
      lcd.print(enabledToSend(config.onOffTDSSending));
    } else if (row == 10) {
      lcd.print("Send PH is");
      lcd.setCursor(13, 0);
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

void showFreqUpdateSetting() {
  showTimeTitle(0, "SET FREQ UPDATE");
  lcd.setCursor(1, 1); lcd.print("T:");
  if (config.freqUpdateWebTemperature == 23) {
    showStringNumber(3, 1, 1);
  } else {
    showStringNumber(3, 1, 24 / config.freqUpdateWebTemperature);
  }
  lcd.setCursor(6, 1); lcd.print("E:");
  if (config.freqUpdateWebTDS == 23) {
    showStringNumber(8, 1, 1);
  } else {
    showStringNumber(8, 1, 24 / config.freqUpdateWebTDS);
  }
  lcd.setCursor (11, 1); lcd.print("P:");
  if (config.freqUpdateWebPH == 23) {
    showStringNumber(13, 1, 1);
  } else {
    showStringNumber(13, 1, 24 / config.freqUpdateWebPH);
  }
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

//Managing the cursor  to manage the updating frequence per item
void showManageFreqSetting(int row, int t, int e, int p) {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("SET");
  lcd.setCursor (5, 0); lcd.print("T:");
  lcd.setCursor (7, 0); lcd.print(24 / freqNumbber[t]);
  if (row == 0) {
    lcd.setCursor (9, 0);
    lcd.write(5);
  }
  lcd.setCursor (11, 0); lcd.print("E:");
  lcd.setCursor (13, 0); lcd.print(24 / freqNumbber[e]);
  if (row == 1) {
    lcd.setCursor (15, 0);
    lcd.write(5);
  }
  lcd.setCursor (0, 1); lcd.print("FREQ");
  lcd.setCursor (5, 1); lcd.print("P:");
  lcd.setCursor (7, 1); lcd.print(24 / freqNumbber[p]);
  if (row == 2) {
    lcd.setCursor (9, 1);
    lcd.write(5);
  }
  lcd.setCursor (10, 1); lcd.print("OK");
  if (row == 3) {
    lcd.setCursor (12, 1);
    lcd.write(6);
  }
  lcd.setCursor (13, 1); lcd.print("<-");
  if (row == 4) {
    lcd.setCursor (15, 1);
    lcd.write(6);
  }
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

void shiftFreqUpdateMenu(int keyPad, int row) {
  if (row == 0) {
    freqUpdateWebTemperatureIndex = manageMenuRangeNumberCursors(keyPad, row , freqUpdateWebTemperatureIndex, 8);
  } else if (row == 1) {
    freqUpdateWebTDSIndex = manageMenuRangeNumberCursors(keyPad, row, freqUpdateWebTDSIndex, 8);
  } else if (row == 2) {
    freqUpdateWebPHIndex = manageMenuRangeNumberCursors(keyPad, row, freqUpdateWebPHIndex, 8);
  }
  showManageFreqSetting(row, freqUpdateWebTemperatureIndex, freqUpdateWebTDSIndex, freqUpdateWebPHIndex);
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
int manageMenuRangeNumberCursors(int keyPad, int row, int number, int maxItem) {
  switch (keyPad) {
    case 1: if (number == maxItem - 1) {
        return 0;
      } else {
        return ++number;
      }
    case 2: if (number == 0) {
        return maxItem - 1;
      } else {
        return --number;
      }
    default: return number;
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

void saveFreqUpdateWeb() {

  config.freqUpdateWebTemperature = setFreq(freqUpdateWebTemperatureIndex);
  config.freqUpdateWebTDS = setFreq(freqUpdateWebTDSIndex);
  config.freqUpdateWebPH = setFreq(freqUpdateWebPHIndex);
  waitingActionMenu();
  saveConfiguration(filename, config);
  exitFromMenu();
}

int setFreq(int freq) {
  if (freq == 0) {
    return 23;
  } else {
    return freqNumbber[freq];
  }

}

// Return index of element starting
// Return -1 if element is not present
int indexOf(const int elm, const int *ar, int ar_cnt) {
  // decreasing array count till it reaches negative
  // arr_cnt - 1 to 0
  while (ar_cnt--)
  {
    // Return array index if current element equals provided element
    if (ar[ar_cnt] == elm)
      return ar_cnt;
  }

  // Element not present
  return -1; // Should never reaches this point
}
