/*
  Aquarium Project Pasquale
*/
char monthNames[][3] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
char waitingString[15] = "Waiting...";
int rowItem = 0, colItem = 0, rowItemMainMenu = 0, rowItemManualMenu = 0, rowItemSettingMenu = 0;

void initMessages(void) {
  display.clearDisplay();
  display.setTextSize(2);             // Normal 2x pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,20);
  display.println(F("AQUARIUM"));
  display.setCursor(65,41);
  display.println(F("PIA12"));
  display.display();
// Scroll from left to right, only one string with (0x00, 0x04)
  display.startscrollright(0x00, 0x04);
  delay(1500);
  display.stopscroll();
  display.clearDisplay();
  display.setCursor(20,20);
  display.println(F("AQUARIUM"));
  display.setCursor(65,41);
  display.println(F("PIA12"));
  display.display();
// Scroll from right to left, only second string with (0x05, 0x07)
  display.startscrollleft(0x05, 0x07);
  delay(2000);
  display.stopscroll();
}

void drawroundrect(void) {
  display.clearDisplay();
  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, WHITE);
    display.display();
    delay(100);
  }
}

void drawbitmap(void) {
  display.clearDisplay();
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    salmon, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();  
}

void initScreen() {
 
  drawbitmap();    // Draw logo bitmap image  
  delay(3000);
  
  initMessages();
  delay(2000);
  
  drawroundrect();
  delay(1000);
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10,30);
  display.println(F("Ready!!!"));
  display.display();
  delay(1000); 
// Clear the buffer
  display.clearDisplay(); 
  display.display();
  
// Turning off the dispaly OLED 
  display.ssd1306_command(SSD1306_DISPLAYOFF);
}

// Screen with time, temperature, ec and ph
void mainScreen() {
  //display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE, BLACK);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(buffer);
  display.setCursor(0,20);             // Start at top-left corner
  display.setTextSize(1);             // Draw 2X-scale text
  display.print(F("Temp: "));
  display.print(temperature);
  display.print(F(" "));
  display.cp437(true);
  display.write(167); //174 175 16 17
  display.println(F("C"));

  display.setCursor(0,30);             // Start at top-left corner
  display.print(F("EC: "));
  display.print(ec);
  display.print(F(" "));
  display.write(230);
  display.println(F("S/cm"));

  display.setCursor(0,40);             // Start at top-left corner
  display.print(F("PH: ")); 
  display.println(phFinal);

  display.display();
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
      showListMenu(mainMenuSize, rowItem, mainMenu);
      if (colItem == -1) {
        exitFromMenu();
      } else if (colItem == 1) {
        changingPage = true;
        switch (rowItem) {
          case 0: activeMenu = 2; break;
          case 1: activeMenu = 5; break;
          case 2: activeMenu = 5; break;
          case 3: activeMenu = 3; break;
          default: break;
        }
        rowItemMainMenu = rowItem; rowItem = 0; colItem = 0;
      }
      break;
    case 2: //Serial.println("Menu Manual...");
      manageMenuCursors(keyPad, manualMenuSize, -1, 2);
      showListMenu(manualMenuSize, rowItem, manualMenu);
      if (colItem == -1) {
        setChangingPageVariable(1, rowItemMainMenu);
      } else if (colItem == 1) {
        if (rowItem >= 4 && rowItem <= 5) {
          activeMenu = 15;      
        } else {
          activeMenu = 4;
        }
        changingPage = true; rowItemManualMenu = rowItem; rowItem = 0; colItem = 0;
      }
      break;
    case 3: //Serial.println("Menu Setting...");
      manageMenuCursors(keyPad, settingMenuSize, -1, 2);
      showListMenu(settingMenuSize, rowItem, settingMenu);
      if (colItem == -1) {
        setChangingPageVariable(1, rowItemMainMenu);
      } else if (colItem == 1) {
        switch (rowItem) {
          case 0: activeMenu = 6; break;
          case 1: activeMenu = 6; break;
          case 2: activeMenu = 6; break;
          case 3: showInfoWifi(); waitingActionMenu(); connectionStatusPage(connStatus); exitFromMenu(); break;
          case 4: waitingActionMenu(); reconnectWifi(); exitFromMenu();
          case 5: activeMenu = 11; break;
          case 6: activeMenu = 14; break;
          case 7: activeMenu = 9; break;
          case 8: activeMenu = 9; break;
          case 9: activeMenu = 9; break;
          case 10: activeMenu = 10; break;
          case 11: activeMenu = 10; break;
          case 12: activeMenu = 10; break;
          default: break;
        }
        changingPage = true; rowItemSettingMenu = rowItem; rowItem = 0; colItem = 0;
      }
      break;
    case 4: //Serial.println("Menu Yes o No for Manual Section...");
      manageMenuCursors(keyPad, 1, -1, 2);
      if (colItem == -1) {
        setChangingPageVariable(2, rowItemManualMenu);
        break;
      }
      yesOrNoSelection(colItem, rowItemManualMenu, activeMenu, manualMenu);
      if (keyPad == code[4]) {
        manageManualSelection(rowItemManualMenu, colItem);
        break;
      } else {
        break;
      }
    case 5: //Serial.println("Menu Yes o No for Main Section...");
      manageMenuCursors(keyPad, 1, -1, 2);
      if (colItem == -1) {
        setChangingPageVariable(1, rowItemMainMenu);
        break;
      }
      yesOrNoSelection(colItem, rowItemMainMenu, activeMenu, mainMenu);
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
      } else if (rowItemSettingMenu == 1) {
        showTime();
      } else {
        showDate();
      }
      if (colItem == 1) {
        setChangingPageVariable(7, 0);
      }
      break;
    case 7: //Serial.println("Menu set start time or real time...");
      switch (rowItemSettingMenu) {
        case 0:
          manageMenuCursors(keyPad, 4, 1, 1);
          shiftHourAndMinutesMenu(0, 1, keyPad, "Start", "Time", rowItem, oraEMinTemp);
          if (keyPad == code[4]) {
            if (rowItem == 2) {
              setChangingPageVariable(8, rowItemSettingMenu);
              break;
            } else if (rowItem == 3) {
              setChangingPageVariable(3, rowItemSettingMenu);
              break;
            } 
          }
          break;
        case 1:
          manageMenuCursors(keyPad, 4, 1, 1);
          shiftHourAndMinutesMenu(0, 1, keyPad, "Set", "Time", rowItem, oraEMinTimeClockTemp);
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
        case 2:
          manageMenuCursors(keyPad, 5, 1, 1);
          setDateMenu(0, 1, 2, keyPad, rowItem, dateTemp);
          if (keyPad == code[4]) {
            if (rowItem == 3) {
              saveDate(dateTemp);
            } else if (rowItem == 4) {
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
    case 9: //Serial.println("Menu Settings Sensors activation...");
      manageMenuCursors(keyPad, 1, -1, 2);
      if (colItem == -1) {
        setChangingPageVariable(3, rowItemSettingMenu);
        break;
      }
      yesOrNoSelection(colItem, rowItemSettingMenu, activeMenu, settingMenu);
      if (keyPad == code[4]) {
        waitingActionMenu();
        manageSettingsSelections(rowItemSettingMenu, colItem);
        exitFromMenu();
      }
      break;
    case 10: //Serial.println("Menu Yes or No Web Sending...");
      manageMenuCursors(keyPad, 1, -1, 2);
      if (colItem == -1) {
        setChangingPageVariable(3, rowItemSettingMenu);
        break;
      }
      yesOrNoSelection(colItem, rowItemSettingMenu, activeMenu, settingMenu);
      if (keyPad == code[4]) {
        waitingActionMenu();
        manageSettingsSelections(rowItemSettingMenu, colItem);
        exitFromMenu();
      }
      break;
    case 11: //Serial.println("Menu show min - max temperature...");
      manageMenuCursors(keyPad, 1, -1, 2);
      if (colItem == -1) {
        setChangingPageVariable(3, rowItemSettingMenu);
        break;
      }
      showActualTemperatureRange();
      if (colItem == 1) {
        setChangingPageVariable(12, 0);
      }
      break;
    case 12: //Serial.println("Menu set temperature...");
      manageMenuCursors(keyPad, 4, 1, 1);
      settingNewMinMaxTemperatureRange(keyPad, "setGrad", rowItem, temperatureMinAndMaxTemp);
      if (keyPad == code[4]) {
        if (rowItem == 2) {
          saveMinMaxTemperature(temperatureMinAndMaxTemp);
        } else if (rowItem == 3) {
          setChangingPageVariable(3, rowItemSettingMenu);
        } else {
          break;
        }
      }
      break;
    case 13: //Serial.println("Menu set Freq update web...");
      manageMenuCursors(keyPad, 5, 1, 1);
      setFreqUpdateMenu(keyPad, rowItem);
      if (keyPad == code[4]) {
        if (rowItem == 3) {
          saveFreqUpdateWeb();
        } else if (rowItem == 4) {
          setChangingPageVariable(3, rowItemSettingMenu);
        } else {
          break;
        }
      }
      break;
    case 14: //Serial.println("Menu show WEB rate...");
      manageMenuCursors(keyPad, 1, -1, 2);
      if (colItem == -1) {
        setChangingPageVariable(3, rowItemSettingMenu);
        break;
      }
      showWebRateSending();
      if (colItem == 1) {
        freqUpdateWebTemperatureIndex = indexNumber(config.freqUpdateWebTemperature);
        freqUpdateWebECIndex = indexNumber(config.freqUpdateWebEC);
        freqUpdateWebPHIndex = indexNumber(config.freqUpdateWebPH);
        setChangingPageVariable(13, 0);
      }
      break;
    case 15: //Serial.println("Menu show Monitoring EC / PH ...");
      manageMenuCursors(keyPad, 1, 1, 1);
      if(rowItemManualMenu == 4){
          monitorEC = true;
      }else{
          monitorPH = true;
      }
      if(keyPad == code[4]){
           waitingActionMenu();
           monitorEC = false;
           monitorPH = false;
           setChangingPageVariable(2, rowItemManualMenu); //exitFromMenu();
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
void showListMenu(int listSize, int rowItem, char **listMenu) {
  int before, after;

  if(rowItem == 0){
    before = listSize - 1;
    after = rowItem + 1;
  }else if(rowItem == listSize - 1){
    before = rowItem - 1;
    after = 0;
  }else{
    before = rowItem - 1;
    after = rowItem + 1;
  }
  
  display.clearDisplay();
  display.cp437(true);
  display.setTextSize(2);             
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("MENU'"));
  display.drawFastHLine(0, 16, 128, WHITE);

  display.setCursor(8,20);             
  display.write(24);
  display.setCursor(0,34);            
  display.write(27); 
  display.setCursor(16,34);             
  display.write(26); 
  display.setCursor(8,46);             
  display.write(25);
  
  display.setCursor(26,25);             
  display.setTextSize(1);             // Draw 1X-scale text
  display.println(listMenu[before]);
  
  display.setCursor(34,38);            
  display.print(listMenu[rowItem]);
  
  display.setCursor(26,51);            
  display.println(listMenu[after]);

  display.display();
}

void manageManualSelection(int selectedRow, int colItem) {
  if (selectedRow < 4) {
    waitingActionMenu();
    manageReleSymbolAndAction(selectedRow, colItem);
  }else{
    switch (selectedRow) {
        case 6: if (colItem == 0) { manualSendingEC = true; waitingActionMenu();} break;
        case 7: if (colItem == 0) { manualSendingPH = true; waitingActionMenu();} break;
        case 8: if (colItem == 0) { manualSendingTemperature = true; waitingActionMenu();} break;
        default: break;
      }
  }
  exitFromMenu();
}


void showMonitoring(char *key, float value, float temperature){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE );        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Monitoring"));
  display.drawFastHLine(0, 16, 128, WHITE);

  display.setCursor(0,28);
  display.setTextSize(1);             // Draw 1X-scale text
  display.print(F("Temperature: "));
  display.print(temperature);
  display.print(F(" "));
  display.cp437(true);
  display.write(167);
  display.println(F("C"));
  display.setCursor(0,45);
  display.print(key);
  display.print(F(": "));
  display.println(value);
  display.fillRect(80, 50, 40, 20, WHITE);
  display.setTextColor(INVERSE);        // Draw INVERSE text
  display.setCursor(88,53);
  display.println(F("Exit"));
 
  display.display();
}

void exitFromMenu() {
  rowItem = 0, colItem = 0, rowItemMainMenu = 0, rowItemManualMenu = 0, rowItemSettingMenu = 0;
  menuOnOff = false;
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
void yesOrNoSelection(int selectedItem, int rowNumber, int idMenu, char **listMenu){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("CHOICE"));
  display.drawFastHLine(0, 16, 128, WHITE);
  if(selectedItem == 0){
    showFillButton(13, 43, 36, 21, 14, "YES");
    showVoidButton(60, 43, 36, 21, 67, "NO");
  }else{
    showVoidButton(13, 43, 36, 21, 14, "YES");
    showFillButton(60, 43, 36, 21, 67, "NO");
  }
  if(idMenu == 4 && rowNumber < 4){
    showYesNoDispaly("turn on the ", listMenu, rowNumber);
  }else if (idMenu == 5){
    showYesNoDispaly("activate ", listMenu, rowNumber);
  }else if (idMenu == 9){
    showYesNoDispaly("enable ", listMenu, rowNumber);
  }else if (idMenu == 10){
    display.setCursor(1,20);            
    display.setTextSize(1);             // Normal 1:1 pixel scale
    if (rowNumber == 10) {
     showSendingActivation(config.onOffTemperatureSending);
    } else if (rowNumber == 11) {
      showSendingActivation(config.onOffECSending);
    } else if (rowNumber == 12) {
      showSendingActivation(config.onOffPhSending);
    }
    display.println(F("?"));
  }else{
    showYesNoDispaly(" ", listMenu, rowNumber);
  } 
  display.display();
}

void showYesNoDispaly(String verb, char **listMenu, int rowNumber){
   display.setCursor(1,20);
   display.setTextSize(1);             // Normal 1:1 pixel scale
   display.println(F("Would you like to"));
   display.print(verb);
   display.print(listMenu[rowNumber]);
   display.println(F("?"));
}

void showVoidButton(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, String text){
   display.drawRect(x0, y0, x1, y1, WHITE);
   display.setTextColor(WHITE);        // Draw INVERSE text
   display.setCursor(x2, y0 + 3);            
   display.println(text);
}

void showFillButton(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, String text){
   display.fillRect(x0, y0, x1, y1, WHITE);
   display.setTextColor(INVERSE);
   display.setCursor(x2, y0 + 3);
   display.println(text);
}

//Used to get the right sensor status
void showSendingActivation(bool item) {
  if (item) {
    display.print(F("It is activete "));
    display.print(F("would you like to deactivate"));
  } else {
    display.print(F("It is inactive "));
    display.print(F("would you like to activate"));
  }
}

//Showing connection status on screen
void connectionStatusPage(char* connStatus) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);             
  display.println(F("Connection"));
  display.println(F("status: ")); 
  display.println(connStatus);
  display.display();
  delay(2000);
}

//Waiting animation
void waitingActionMenu() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(20,0);             
  display.println(waitingString);
  delay(1000);
  drawroundrect();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10,30);
  display.println(F("Done!!!"));
  display.display();
  delay(1000); 
//  display.drawRect(10, 28, 94, 20, WHITE);
//  display.cp437(true);
//  for (int i = 0; i < 10; i++) {
//    display.setCursor(10 + (i*9), 30);             
//    display.write(178);
//    display.display();
//    delay(4000 / 10);
//  }
}

//Check if the becklight of the screen should be on or off.
//if keyPad is different from zero, it will turn on the light, set the screenBeckLightOnOff
//variable 'true' and will set _timerLightDisplay with the same timerOfLight's parameter
//value. When screenBeckLightOnOff is true and _timerLightDisplay is different from zero or
//one, this value will count back until _timerLightDisplay will be equal to one in order
//to turn off screen backlight and to set screenBeckLightOnOff to false.
void checkScreenBeckLight(int keyPad) {
  if (keyPad != code[6]) {
    screenBeckLightOnOff = true;
    display.ssd1306_command(SSD1306_DISPLAYON);
    _timerLightDisplay = timerOfLight;
  } else if (screenBeckLightOnOff && _timerLightDisplay == 1) {
    display.ssd1306_command(SSD1306_DISPLAYOFF);
    screenBeckLightOnOff = false;
    _timerLightDisplay--;
  } else if (screenBeckLightOnOff) {
    _timerLightDisplay--;
  } else {
    return ;
  }
}

// Show the rele symbols on screen
void showReleSymbol() {
  display.clearDisplay();
  for (int p = 0; p < numRele; p++) { 
    if (releSymbol[p]==0){
       display.drawCircle(70 + p*12, 55, 5, WHITE); 
    }else{
       display.fillCircle(70 + p*12, 55, 5, INVERSE);
    }
  }
  display.display();
}

void showLightsTimerSetting() {
  oraEMinTemp[0] = config.startHour;
  oraEMinTemp[1] = config.startMinutes;
  oraEMinTemp[2] = config.endHour;
  oraEMinTemp[3] = config.endMinutes;
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("TIMER"));
  display.drawFastHLine(0, 16, 128, WHITE);
  display.setTextSize(1);    
  display.setCursor(0, 21);
  display.print(F("Start Timer: "));
  showStringNumber(75, 21, config.startHour);
  display.setCursor(87, 21);  display.print(F(":"));
  showStringNumber(92, 21, config.startMinutes);
  display.setCursor(0, 35);
  display.print(F("End Timer: "));
  showStringNumber(75, 35, config.endHour);
  display.setCursor (87, 35); display.print(F(":"));
  showStringNumber(92, 35, config.endMinutes);
  foo();
  display.display();
}

void showWebRateSending() {
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("WEB RATES"));
  display.drawFastHLine(0, 16, 128, WHITE);
  display.setTextSize(1);    
  display.setCursor(0, 19);
  display.print(F("Temperature: "));
  if (config.freqUpdateWebTemperature == 23) {
    showStringNumber(75, 20, 1);
  } else {
    showStringNumber(75, 20, 24 / config.freqUpdateWebTemperature);
  }
  display.setCursor(0, 30);
  display.print(F("EC/TDS: "));
  if (config.freqUpdateWebEC == 23) {
    showStringNumber(75, 30, 1);
  } else {
    showStringNumber(75, 30, 24 / config.freqUpdateWebEC);
  }
  display.setCursor(0, 40);
  display.print(F("PH: "));
  if (config.freqUpdateWebPH == 23) {
    showStringNumber(75, 40, 1);
  } else {
    showStringNumber(75, 40, 24 / config.freqUpdateWebPH);
  }
  foo();
  display.display();
}

void foo(){
  display.setCursor(18,55);
  display.print(F("BACK"));
  display.setCursor(88,55);
  display.print(F("SET"));
  display.setTextSize(2); 
  display.setCursor(5,52);             
  display.write(27);
  display.setCursor(108,52);             
  display.write(26);
}

void showActualTemperatureRange() {
  temperatureMinAndMaxTemp[0] = config.tempMin;
  temperatureMinAndMaxTemp[1] = config.tempMax;
  
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.print(F("RANGE"));
  display.print(F(" "));
  display.cp437(true);
  display.write(167);
  display.println(F("C"));
  display.drawFastHLine(0, 16, 128, WHITE);
  display.setTextSize(1); 
  display.setCursor(0, 21);   
  display.print(F("Min degrees: "));
  showStringNumber(75, 21, config.tempMin);
  display.print(F(" "));
  display.write(167);
  display.println(F("C"));
  display.setCursor(0, 35);
  display.print(F("Max degrees: "));
  showStringNumber(75, 35, config.tempMax);
  display.print(F(" "));
  display.write(167);
  display.println(F("C"));
  foo();
  display.display();
}

void showStringNumber(byte i, byte j, int number) {
  display.setCursor(i, j);             
  if (number < 10) {
    display.print(F("0"));
    display.setCursor(i + 6, j); display.print(number);
  } else {
    display.print(number);
  }
}

void showTime() {
  oraEMinTimeClockTemp[0] = RTC.now().hour();
  oraEMinTimeClockTemp[1] = RTC.now().minute();
  //  Serial.println(RTC.now().hour());
  //  Serial.println(RTC.now().minute());
  //  Serial.println(oraEMinTimeClockTemp[0]);
  //  Serial.println(oraEMinTimeClockTemp[1]);
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("TIME"));
  display.drawFastHLine(0, 16, 128, WHITE);
  display.setTextSize(1);    
  display.setCursor(0, 21);
  display.print(F("Current time: "));
  showStringNumber(45, 35, oraEMinTimeClockTemp[0]);
  //display.setCursor (57, 35); 
  display.print(F(":"));
  showStringNumber(62, 35, oraEMinTimeClockTemp[1]);
  foo();
  display.display();
}

void showDate() {
  dateTemp[0] = RTC.now().day();
  dateTemp[1] = RTC.now().month();
  dateTemp[2] = RTC.now().year();
  Serial.println( RTC.now().day());
  Serial.println( RTC.now().month());
  Serial.println(dateTemp[0]);
  Serial.println( dateTemp[1]);
  Serial.println( dateTemp[2]);
  Serial.println(RTC.now().year());
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("DATA"));
  display.drawFastHLine(0, 16, 128, WHITE);
  display.setTextSize(1);    
  display.setCursor(0, 21);
  display.print(F("Current Data: "));
  showStringNumber(35, 35, dateTemp[0]);
  display.print(F("/"));
  showStringNumber(52, 35, dateTemp[1]);
  display.print(F("/"));
  showStringNumber(71, 35, dateTemp[2]);
  foo();
  display.display();
}

//Managing the cursor to manage the updating frequence per item
void showManageFreqSetting(int row, int t, int e, int p) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("SET RATES"));
  display.cp437(true);
  display.drawFastHLine(0, 16, 128, WHITE);
  display.setCursor(2,20);
  display.write(24);
  display.setCursor(2,40);
  display.write(25);
  display.setTextSize(1);
  display.setCursor(16, 19);
  display.print(F("Temperature: "));
  display.setCursor(102, 19);
  display.print(24 / freqNumbber[t]);
  
  display.setCursor(16, 29);
  display.print(F("EC/TDS: "));
  display.setCursor(102, 29);
  display.print(24 / freqNumbber[e]);
  
  display.setCursor(16, 38);
  display.print(F("PH: "));
  display.setCursor(102, 38);
  display.print(24 / freqNumbber[p]);
  
  if(row == 0){
    display.setTextSize(2);
    display.setCursor(90,16);
    display.write(27);
    display.setCursor(114,16);
    display.write(26);
    display.setTextSize(1);
    showVoidButton(30, 49, 24, 14, 35, "OK");
    showVoidButton(73, 49, 31, 14, 78, "EXIT");  
  }else if(row == 1){
    display.setTextSize(2);
    display.setCursor(90,26);
    display.write(27);
    display.setCursor(114,26);
    display.write(26);
    display.setTextSize(1);
    showVoidButton(30, 49, 24, 14, 35, "OK");
    showVoidButton(73, 49, 31, 14, 78, "EXIT");  
  }else if(row == 2){
    display.setTextSize(2);
    display.setCursor(90,36);
    display.write(27);
    display.setCursor(114,36);
    display.write(26);
    display.setTextSize(1);
    showVoidButton(30, 49, 24, 14, 35, "OK");
    showVoidButton(73, 49, 31, 14, 78, "EXIT");
  }else if(row == 3){
    display.setTextSize(1);
    showFillButton(30, 49, 24, 14, 35, "OK");
    showVoidButton(73, 49, 31, 14, 78, "EXIT");  
  }else if (row == 4){
    display.setTextSize(1);
    showVoidButton(30, 49, 24, 14, 35, "OK");
    showFillButton(73, 49, 31, 14, 78, "EXIT");  
  }
  display.display();
}

//Managing the cursor that selects the temperature to manage the termometer
void showDateSetting(int row, int y, int m, int d) {
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.print(F("NEW DATA"));
  display.cp437(true);
  display.drawFastHLine(0, 16, 128, WHITE);
  display.setCursor(2,20);             
  display.write(24);
  display.setCursor(2,40);             
  display.write(25);
  display.setTextSize(1); 
  display.setCursor(16, 18);   
  display.print(F("Year: "));
  showStringNumber(65, 18, y);
  display.setCursor(16, 27);
  display.print(F("Month: "));
  showStringNumber(65, 27, m);
  display.setCursor(16, 36);
  display.print(F("Day: "));
  showStringNumber(65, 36, d);
  if(row == 0){
    display.setTextSize(2);
    display.setCursor(50,15);
    display.write(27); 
    display.setCursor(91,15);
    display.write(26);
    display.setTextSize(1);
    showVoidButton(30, 49, 24, 14, 35, "OK");
    showVoidButton(73, 49, 31, 14, 78, "EXIT");
  }else if(row == 1){
    display.setTextSize(2);
    display.setCursor(52,24);            
    display.write(27); 
    display.setCursor(84,24);             
    display.write(26);   
    display.setTextSize(1); 
    showVoidButton(30, 49, 24, 14, 35, "OK");
    showVoidButton(73, 49, 31, 14, 78, "EXIT");  
  }else if(row == 2){
    display.setTextSize(2);
    display.setCursor(52,32);
    display.write(27);
    display.setCursor(84,32);
    display.write(26);
    display.setTextSize(1);
    showVoidButton(30, 49, 24, 14, 35, "OK");
    showVoidButton(73, 49, 31, 14, 78, "EXIT");
  }else if(row == 3){
    display.setTextSize(1); 
    showFillButton(30, 49, 24, 14, 35, "OK");
    showVoidButton(73, 49, 31, 14, 78, "EXIT");  
  }else if (row == 4){
    display.setTextSize(1);
    showVoidButton(30, 49, 24, 14, 35, "OK");
    showFillButton(73, 49, 31, 14, 78, "EXIT");  
  }
  display.display();
}

//Managing the cursor that selects the temperature to manage the termometer
void showMinMaxRangeSetting(int row, String one, int h, int m) {
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.print(F("NEW RANGE"));
  display.cp437(true);
  display.drawFastHLine(0, 16, 128, WHITE);
  display.setCursor(2,20);             
  display.write(24);
  display.setCursor(2,40);             
  display.write(25);
  display.setTextSize(1); 
  display.setCursor(16, 21);   
  display.print(F("Min: "));
  showStringNumber(58, 21, h);
  display.print(F(" "));
  display.write(167);
  display.println("C");
  display.setCursor(16, 35);
  display.print(F("Max: "));
  showStringNumber(58, 36, m);
  display.print(F(" "));
  display.write(167);
  display.println("C"); 
  if(row == 0){
    display.setTextSize(2);
    display.setCursor(45,18);            
    display.write(27); 
    display.setCursor(90,18);             
    display.write(26);   
    display.setTextSize(1); 
    showVoidButton(30, 49, 24, 14, 35, "OK");
    showVoidButton(73, 49, 31, 14, 78, "EXIT");  
  }else if(row == 1){
    display.setTextSize(2);
    display.setCursor(45,30);
    display.write(27);
    display.setCursor(90,30);
    display.write(26);
    display.setTextSize(1);
    showVoidButton(30, 49, 24, 14, 35, "OK");
    showVoidButton(73, 49, 31, 14, 78, "EXIT");
  } else if(row == 2){
    display.setTextSize(1);
    showFillButton(30, 49, 24, 14, 35, "OK");
    showVoidButton(73, 49, 31, 14, 78, "EXIT");  
  }else if (row == 3){
    display.setTextSize(1);
    showVoidButton(30, 49, 24, 14, 35, "OK");
    showFillButton(73, 49, 31, 14, 78, "EXIT");  
  }
  display.display();
}

// Managing the cursor to move inside the time selection menu
void manageLightsTimerSetting(int row, String one, String two, int h, int m) {
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.print(one);
  display.print(F(" "));
  display.print(two);
  display.cp437(true);
  display.drawFastHLine(0, 16, 128, WHITE);
  display.setCursor(2,20);             
  display.write(24);
  display.setCursor(2,40);             
  display.write(25);
  display.setTextSize(1); 
  display.setCursor(16, 21);   
  display.print(F("Hours: "));
  showStringNumber(82, 21, h);
  display.setCursor(16, 35);
  display.print(F("Minutes: "));
  showStringNumber(82, 35, m);
  if(row == 0){
    display.setTextSize(2);
    display.setCursor(70,18);
    display.write(27);
    display.setCursor(96,18);
    display.write(26);
    display.setTextSize(1);
    showVoidButton(30, 49, 24, 14, 35, "OK");
    showVoidButton(73, 49, 31, 14, 78, "EXIT");
  }else if(row == 1){
    display.setTextSize(2);
    display.setCursor(70,32);
    display.write(27);
    display.setCursor(96,32);
    display.write(26);
    display.setTextSize(1);
    showVoidButton(30, 49, 24, 14, 35, "OK");
    showVoidButton(73, 49, 31, 14, 78, "EXIT");
  }else if(row == 2){
    display.setTextSize(1); 
    showFillButton(30, 49, 24, 14, 35, "OK");
    showVoidButton(73, 49, 31, 14, 78, "EXIT");  
  }else if (row == 3){
    display.setTextSize(1);
    showVoidButton(30, 49, 24, 14, 35, "OK");
    showFillButton(73, 49, 31, 14, 78, "EXIT");  
  }
  display.display();
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

void settingNewMinMaxTemperatureRange(int keyPad, String firstS, int row, byte *minMax) {
  if (row == 0) {
    minMax[0] = manageMenuRangeNumberCursors(keyPad, row , minMax[0], 0, 99);
  } else if (row == 1) {
    minMax[1] = manageMenuRangeNumberCursors(keyPad, row, minMax[1], 0, 99);
  }
  showMinMaxRangeSetting(row, firstS, minMax[0], minMax[1]);
}

void setFreqUpdateMenu(int keyPad, int row) {
  if (row == 0) {
    freqUpdateWebTemperatureIndex = manageMenuRangeNumberCursors(keyPad, row , freqUpdateWebTemperatureIndex, 0, 8);
  } else if (row == 1) {
    freqUpdateWebECIndex = manageMenuRangeNumberCursors(keyPad, row, freqUpdateWebECIndex, 0, 8);
  } else if (row == 2) {
    freqUpdateWebPHIndex = manageMenuRangeNumberCursors(keyPad, row, freqUpdateWebPHIndex, 0, 8);
  }
  showManageFreqSetting(row, freqUpdateWebTemperatureIndex, freqUpdateWebECIndex, freqUpdateWebPHIndex);
}

void shiftHourAndMinutesMenu(int i, int j, int keyPad, String firstS, String secondS, int row, byte *oraEMin) {
  if (row == 0) {
    oraEMin[i] = manageMenuRangeNumberCursors(keyPad, row , oraEMin[i], 0, 24);
  } else if (row == 1) {
    oraEMin[j] = manageMenuRangeNumberCursors(keyPad, row, oraEMin[j], 0, 60);
  }
  manageLightsTimerSetting(row, firstS, secondS, oraEMin[i], oraEMin[j]);
}

void setDateMenu(byte i, byte j, byte z, int keyPad, int row, int *date) {
  if (row == 0) {
    date[z] = manageYearRangeCursor(keyPad, row, date[z], 2060);
  } else if (row == 1) {
    date[j] = manageMenuRangeNumberCursors(keyPad, row, date[j], 1, 13);
  } else {
    date[i] = manageDayRangeCursor(keyPad, row , date[j],  date[z], date[i]);
  }
  if (date[i] > _get_max_day(short(date[j] - 1), date[z])) {
    date[i] = _get_max_day(short(date[j] - 1), date[z]);
  }
  showDateSetting(row, date[z], date[j], date[i]);
}

// Managing the cursor to move inside the menu for year setting
int manageYearRangeCursor(int keyPad, int row, int number, int maxItem) {
  switch (keyPad) {
      if (number == 0) {
        number = 2020;
      }
    case 1: if (number == maxItem - 1) {
        return 2020;
      } else {
        return ++number;
      }
    case 2: if (number == 2020) {
        return maxItem - 1;
      } else {
        return --number;
      }
    default: return number;
  }
}

// Managing the cursor to move inside the menu for year setting
int manageDayRangeCursor(int keyPad, int row, int _month, int _year, int number) {
  switch (keyPad) {
    case 1: if (number == _get_max_day((short(_month - 1)), _year)) {
        return 1;
      } else {
        return ++number;
      }
    case 2:
      if (number == 1) {
        return _get_max_day(short(_month - 1), _year);
      } else {
        return --number;
      }
    default: return number;
  }
}

// Managing the cursor to move inside the menu
int manageMenuRangeNumberCursors(int keyPad, int row, int number, int minItem, int maxItem) {
  switch (keyPad) {
    case 1: if (number == maxItem - 1) {
        return minItem;
      } else {
        return ++number;
      }
    case 2: if (number == minItem) {
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
  char timeBufferSub[8];
  char dateBuffer[11];
  sprintf(timeBuffer, "%02d:%02d:00", oraEMinTimeClockTemp[0], oraEMinTimeClockTemp[1]);
  sprintf(dateBuffer, "%s %02d %04d",  monthName(RTC.now().month()), RTC.now().day(), RTC.now().year());
  substring(timeBuffer, timeBufferSub, 1,8);
  //Serial.print("Time buffer: "+String(dateBuffer)+ " "+String(timeBufferSub));
  RTC.adjust(DateTime(dateBuffer, timeBufferSub));
  exitFromMenu();
}

// C substring function definition
void substring(char s[], char sub[], int p, int l) {
   int c = 0;
   
   while (c < l) {
      sub[c] = s[p+c-1];
      c++;
   }
   sub[c] = '\0';
}

short _get_max_day(short month, int year) {
  if (month == 0 || month == 2 || month == 4 || month == 6 || month == 7 || month == 9 || month == 11)
    return 31;
  else if (month == 3 || month == 5 || month == 8 || month == 10)
    return 30;
  else {
    if (year % 4 == 0) {
      if (year % 100 == 0) {
        if (year % 400 == 0)
          return 29;
        return 28;
      }
      return 29;
    }
    return 28;
  }
}

const char *monthName(int m) {
  switch (m) {
    case 1: return "Jan";
    case 2: return "Feb";
    case 3: return "Mar";
    case 4: return "Apr";
    case 5: return "May";
    case 6: return "Jun";
    case 7: return "Jul";
    case 8: return "Aug";
    case 9: return "Sep";
    case 10: return "Oct";
    case 11: return "Nov";
    case 12: return "Dec";
    default: return "Jan";
  }
}

void saveDate(int *date) {
  waitingActionMenu();
  char dateBuffer[11];
  char timeBuffer[8];
  char timeBufferSub[8];
  sprintf(timeBuffer, "%02d:%02d:00", RTC.now().hour(), RTC.now().minute());
  sprintf(dateBuffer, "%s %02d %04d",  monthName(date[1]), date[0], date[2]);
  substring(timeBuffer, timeBufferSub, 1,8);
  //Serial.println("Date buffer: " + String(dateBuffer) + " " + String(timeBuffer));
  RTC.adjust(DateTime(dateBuffer, timeBufferSub));
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
  config.freqUpdateWebEC = setFreq(freqUpdateWebECIndex);
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
