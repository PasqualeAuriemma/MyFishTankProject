/*
  Aquarium Project Pasquale
*/
#ifndef header_H
#define header_H

class Screen {

  protected:

    const int maxSize = 16;

    byte freqUpdateWebTemperatureIndex = 0;

    byte freqUpdateWebECIndex = 0;

    byte freqUpdateWebPHIndex = 0;

    bool monitorEC = false;
    
    bool monitorPH = false;

    bool connectionShow = false;

    int okButton;

    byte hourEMinTemp[4] = {0, 0, 0, 0};

    byte hourEMinTimeClockTemp[2] = {0, 0};
	
	byte hourEMinTimeRecovery[2] = {0, 0};

    int dateTemp[3] = {0, 0, 0};

    byte temperatureMinAndMaxTemp[2] = {0, 0};

    //----------------   Initialize Menu Array   ---------------------
    //-----------------------   Main Menu Page   ----------------------
    String mainMenu[4] = {"Manual", "Automation", "Maintenance", "Settings"};
    byte mainMenuSize = sizeof(mainMenu) / sizeof(mainMenu[0]);
    //-----------------------   Manual Menu Page  ---------------------
    String manualMenu[9] = {"Light", "Filter", "Heater", "Oxygen", "Monitoring EC",
                            "Monitoring PH", "Send Temperat.", "Send EC", "Send PH"
                           };
    byte manualMenuSize = sizeof(manualMenu) / sizeof(manualMenu[0]);
    //-----------------------   Setting Menu Page   -------------------
    String settingMenu[13] = {"Timer Light", "Time/Date", "Recovery", "Connection", "Reconnect",
                              "Heater Auto", "WebRate sending", "Thermometer", "EC Meter",
                              "PH Meter", "Temp. Sending", "EC/TDS Sending", "PH Sending"
                             };
    byte settingMenuSize = sizeof(settingMenu) / sizeof(settingMenu[0]);

    int activeMenu = 1;

    bool changingPage = true;

    byte freqNumbber[8] = {24, 12, 8, 6, 4, 3, 2, 1};

    char monthNames[12][3] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    char waitingString[15] = "Waiting...";

    int rowItem = 0, colItem = 0;
    byte rowItemMainMenu = 0, rowItemManualMenu = 0, rowItemSettingMenu = 0;

  public:  
    //Showing Menu
    // It is the main function to show the menu. The most important variables are 'activeMenu' and 'changingPage'
    // that they allow to change menu page. When menuOnOff is true the main menu page is showed ed it is possible
    // to select some macro settings. If a macro setting is selected, you can see its section with all the voice
    // that you can manage. In little speech, you can change the page changing the activeMenu value. With the use
    // of all the variable values, every function will be customable for the specific action. It is very important
    // to know what is the row and the column of the menu in each option menu because they led the choice of the
    // action.
    bool menu_(int keyPad) {
      //se non è stato premuto nessun tasto ma si riceve un comando di cambiare pagina da qualche pagina precedente altrimenti rimane la stessa pagina statica
      if (keyPad == 0 && !changingPage){ return true ;}
      //setta sempre a false il cambio pagina per cambiarla una sola volta nel caso e fermasi al controllo precedente
      if (changingPage){ setChangingPage(false);}
      // switch sulle varie sezioni che vengono scelte
      switch (activeMenu) {
        case 1: //Serial.println("Main Menu...");
          setRow(manageRowMenuCursors(keyPad, mainMenuSize, rowItem));
          setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
          showListMenu(mainMenuSize, rowItem, mainMenu);
          if(colItem == -1){
            exitFromMenu();
            return setMenuAndChangingPage(true, false);}
          else if(colItem == 1){
            setChangingPage(true);
            switch (rowItem){
              case 0: return moveFromMainMenu(2, rowItem);
              case 1: return moveFromMainMenu(5, rowItem);
              case 2: return moveFromMainMenu(5, rowItem);
              case 3: return moveFromMainMenu(3, rowItem);
              default: return setMenuAndChangingPage(true, false);
            }
          }else{ return setMenuAndChangingPage(false, true);}
        case 2: //Serial.println("Menu Manual...");
          setRow(manageRowMenuCursors(keyPad, manualMenuSize, rowItem));
          setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
          showListMenu(manualMenuSize, rowItem, manualMenu);
          if (colItem == -1) {return comingBackFromMenuPage(1, rowItemMainMenu);} 
          else if(colItem == 1){
            if(rowItem >= 4 && rowItem <= 5){return moveFromManualMenu(15, rowItem);} 
            else{return moveFromManualMenu(4, rowItem);}}
          else{return setMenuAndChangingPage(false, true);}
        case 3: //Serial.println("Menu Setting...");
          setRow(manageRowMenuCursors(keyPad, settingMenuSize, rowItem));
          setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
          showListMenu(settingMenuSize, rowItem, settingMenu);
          if (colItem == -1) {
            return comingBackFromMenuPage(1, rowItemMainMenu);
          } else if (colItem == 1) {
            switch (rowItem) {
              case 0: return moveFromSettingMenu(6, rowItem);
              case 1: return moveFromSettingMenu(6, rowItem);
              case 2: return moveFromSettingMenu(6, rowItem);
              case 3: return moveFromSettingMenu(16, rowItem); 
              case 4: return moveFromSettingMenu(16, rowItem);
              case 5: return moveFromSettingMenu(11, rowItem);
              case 6: return moveFromSettingMenu(14, rowItem);
              case 7: return moveFromSettingMenu(9, rowItem);
              case 8: return moveFromSettingMenu(9, rowItem);
              case 9: return moveFromSettingMenu(9, rowItem);
              case 10: return moveFromSettingMenu(10, rowItem);
              case 11: return moveFromSettingMenu(10, rowItem);
              case 12: return moveFromSettingMenu(10, rowItem);
              default: return setMenuAndChangingPage(true, false);
            }
          } else {
            return setMenuAndChangingPage(false, true);
          }
        case 4: //Serial.println("Menu Yes o No for Manual Section...");
          setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
          if (colItem == -1) {
            return comingBackFromMenuPage(2, rowItemManualMenu);
          }
          showYesOrNoSelection1(colItem, rowItemManualMenu, manualMenu);
          if (keyPad == okButton) {
            waitingActionMenu();
            manageReleSymbolAndAction(rowItemManualMenu, colItem);
            exitFromMenu();
            return setMenuAndChangingPage(true, false);
          } else {
            return setMenuAndChangingPage(false, true);
          }
        case 5: //Serial.println("Menu Yes o No for Main Section...");
          setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
          if (colItem == -1){return comingBackFromMenuPage(1, rowItemMainMenu);}
          showYesOrNoSelection2(colItem, rowItemMainMenu, mainMenu);
          if(keyPad == okButton){
            waitingActionMenu(); manageAutomationProcessAndMaintenance(rowItemMainMenu, colItem);
            exitFromMenu(); return setMenuAndChangingPage(true, false);}
          else{return setMenuAndChangingPage(false, true);}
        case 6: //Serial.println("Menu show time...");
          setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
          if(colItem == -1){return comingBackFromMenuPage(3, rowItemSettingMenu);}
			  if(rowItemSettingMenu == 0){lightsTimerSetting(); 
            showLightsTimerSetting(hourEMinTemp[0], hourEMinTemp[1], hourEMinTemp[2], hourEMinTemp[3]);}
          else if (rowItemSettingMenu == 1){timeSetting(); dateSetting();
		  showTimeDate(hourEMinTimeClockTemp[0], hourEMinTimeClockTemp[1],
		           dateTemp[0], dateTemp[1], dateTemp[2], "Current Time/Date");}
          else{recoverySetting();  showTime(hourEMinTimeRecovery[0], hourEMinTimeRecovery[1], "Recovery Time");}
          if(colItem == 1){ return moveFromMenu(7, 0);}
          else{ return setMenuAndChangingPage(false, true);}
        case 7: //Serial.println("Menu set start time or real time...");
          switch(rowItemSettingMenu) {
            case 0:
              setRow(manageRowMenuCursors(keyPad, 4, rowItem));
              setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
              shiftHourAndMinutesMenu(0, 1, keyPad, "Start", "Time", rowItem, hourEMinTemp);
              if(keyPad == okButton){
                if(rowItem == 2){ return moveFromMenu(8, 0);}
                else if(rowItem == 3){ return comingBackFromMenuPage(3, rowItemSettingMenu);}
                else{ return setMenuAndChangingPage(false, true);}}
              else{ return setMenuAndChangingPage(false, true);}
            case 1:
              setRow(manageRowMenuCursors(keyPad, 4, rowItem));
              setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
              shiftHourAndMinutesMenu(0, 1, keyPad, "Set", "Time", rowItem, hourEMinTimeClockTemp);
              if(keyPad == okButton){
                if(rowItem == 2){ return moveFromSettingMenu(7, 3);}
                else if (rowItem == 3){return comingBackFromMenuPage(3, rowItemSettingMenu);}
                else{ return setMenuAndChangingPage(false, true);}}
              else{ return setMenuAndChangingPage(false, true);}
			 case 2:
              setRow(manageRowMenuCursors(keyPad, 4, rowItem));
              setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
              shiftHourAndMinutesMenu(0, 1, keyPad, "Set", "Time", rowItem, hourEMinTimeRecovery);
              if(keyPad == okButton){
                if(rowItem == 2){ waitingActionMenu(); saveRecoveryTime(hourEMinTimeRecovery); exitFromMenu();
                  return setMenuAndChangingPage(true, false);}
                else if(rowItem == 3){ return comingBackFromMenuPage(3, rowItemSettingMenu);}
                else{ return setMenuAndChangingPage(false, true);}}
              else{ return setMenuAndChangingPage(false, true);}  
            case 3:
              setRow(manageRowMenuCursors(keyPad, 5, rowItem));
              setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
              setDateMenu(0, 1, 2, keyPad, rowItem, dateTemp);
              if(keyPad == okButton){
                if(rowItem == 3){ saveDate(dateTemp); waitingActionMenu();
                    			  saveTime(hourEMinTimeClockTemp); exitFromMenu();
                  return setMenuAndChangingPage(true, false);}
                else if(rowItem == 4){ return comingBackFromMenuPage(3, 1);}
                else{ return setMenuAndChangingPage(false, true);}}
              else{ return setMenuAndChangingPage(false, true);}
            default: return setMenuAndChangingPage(true, false);
          }  
        case 8: //Serial.println("Menu set end time...");
          setRow(manageRowMenuCursors(keyPad, 4, rowItem));
          setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
          shiftHourAndMinutesMenu(2, 3, keyPad, "End", "Time", rowItem, hourEMinTemp);
          if(keyPad == okButton){
            if(rowItem == 2){waitingActionMenu(); saveTimerLights(hourEMinTemp);
              exitFromMenu(); return setMenuAndChangingPage(true, false);}
            else if(rowItem == 3){ return comingBackFromMenuPage(3, rowItemSettingMenu);}
            else{ return setMenuAndChangingPage(false, true);}}
          else{ return setMenuAndChangingPage(false, true);}
        case 9: //Serial.println("Menu Settings Sensors activation...");
          setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
          if(colItem == -1){ return comingBackFromMenuPage(3, rowItemSettingMenu);}
          showYesOrNoSelection3(colItem, rowItemSettingMenu, settingMenu);
          if(keyPad == okButton){
            waitingActionMenu(); manageSettingsSelections(rowItemSettingMenu, colItem);
            exitFromMenu(); return setMenuAndChangingPage(true, false);}  
          else{ return setMenuAndChangingPage(false, true);}
        case 10: //Serial.println("Menu Yes or No Web Sending...");
          setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
          if (colItem == -1) { comingBackFromMenuPage(3, rowItemSettingMenu);}
          yesOrNoSelection4(colItem, rowItemSettingMenu, settingMenu);
          if(keyPad == okButton){ 
            waitingActionMenu(); manageSettingsSelections(rowItemSettingMenu, colItem);
            exitFromMenu(); return setMenuAndChangingPage(true, false);}
            else{ return setMenuAndChangingPage(false, true);}
        case 11: //Serial.println("Menu show min - max temperature...");
          setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
          actualTemperatureRange();
          showActualTemperatureRange(temperatureMinAndMaxTemp[1], temperatureMinAndMaxTemp[0]);
          if(colItem == 1){ moveFromMenu(12, 0);}
          else if(colItem == -1){ return comingBackFromMenuPage(3, rowItemSettingMenu);}
          else{ return setMenuAndChangingPage(false, true);}
        case 12: //Serial.println("Menu set temperature...");
          setRow(manageRowMenuCursors(keyPad, 4, rowItem));
          setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
          settingNewMinMaxTemperatureRange(keyPad, "setGrad", rowItem, temperatureMinAndMaxTemp);
          if(keyPad == okButton){
            if(rowItem == 2){ waitingActionMenu(); 
              saveMinMaxTemperature(temperatureMinAndMaxTemp);
              exitFromMenu(); return setMenuAndChangingPage(true, false);}  
            else if(rowItem == 3){ return comingBackFromMenuPage(3, rowItemSettingMenu);}
            else{ return setMenuAndChangingPage(false, true);}
          }else{ return setMenuAndChangingPage(false, true);}
        case 13: //Serial.println("Menu set Freq update web...");
          setRow(manageRowMenuCursors(keyPad, 5, rowItem));
          setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
          setFreqUpdateMenu(keyPad, rowItem);
          if(keyPad == okButton){
            if(rowItem == 3){ waitingActionMenu(); 
              saveFreqUpdateWeb(); exitFromMenu(); return setMenuAndChangingPage(true, false);}
            else if (rowItem == 4) {comingBackFromMenuPage(3, rowItemSettingMenu);}
            else { return setMenuAndChangingPage(false, true);}}
          else{ return setMenuAndChangingPage(false, true);}
        case 14: //Serial.println("Menu show WEB rate...");
          setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
          freqNumberIndex();
          showWebRateSending(freqUpdateWebTemperatureIndex, freqUpdateWebECIndex, freqUpdateWebPHIndex);
          if (colItem == -1){return comingBackFromMenuPage(3, rowItemSettingMenu);}
          else if(colItem == 1){return moveFromMenu(13, 0);}
          else{ return setMenuAndChangingPage(false, true);}
        case 15: //Serial.println("Menu show Monitoring EC / PH ...");
          setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
          if(rowItemManualMenu == 4){setMonitorEC(true);}
          else{setMonitorPH(true);}
          if(keyPad == okButton){
            waitingActionMenu(); setMonitorEC(false); setMonitorPH(false);
            return comingBackFromMenuPage(2, rowItemManualMenu);}
          else{return setMenuAndChangingPage(false, true);}  
        case 16: //Serial.println("Menu WIFI ...");
          setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
          if(getRowSettingMenuMem() == 4){
            waitingActionMenu(); reconnectMenu(); exitFromMenu(); return setMenuAndChangingPage(true, false);}
          else{ sentConnectionRequest(); setConnectionShow(true);}
          if(keyPad == okButton){
            waitingActionMenu(); setConnectionShow(false);
            return comingBackFromMenuPage(3, rowItemSettingMenu);}
          else{return setMenuAndChangingPage(false, true);}   
        default: return setMenuAndChangingPage(true, false);
      }
    }

    void sentConnectionRequest();
    void checkScreenBeckLight(int keyPad, byte noTouch);
    void reconnectMenu();
    void freqNumberIndex();
    void saveFreqUpdateWeb();
    void saveMinMaxTemperature(byte *minMax);
    void yesOrNoSelection4(byte selectedItem, int rowNumber, String *listMenu);
    void actualTemperatureRange();
    void manageSettingsSelections(int item, int colItem);
    void saveTimerLights(byte *hourEMin);
    void saveTime(byte *hourEMin);
	void saveRecoveryTime(byte *hourEMin);
    void saveDate(int *date);
    void dateSetting();
    void timeSetting();
	void recoverySetting();
    void lightsTimerSetting();
    void manageAutomationProcessAndMaintenance(int itemSelected, int colItem);
    void manageReleSymbolAndAction(int index, int onOff);
    void releSymbolMenu();
    virtual void initScreen() = 0;
    virtual void showActualTemperatureRange(byte _max, byte _min) = 0; 
    virtual void mainScreen(char *_time, float temperature, float ec, float phFinal) = 0;
    virtual void showReleSymbol(int *rele, byte num) = 0;
    virtual void waitingActionMenu() = 0;
    virtual void showTimeDate(byte _hour, byte _minutes, int _day, int _month, int _year, String title) = 0;
    virtual void showYesOrNoSelection1(byte selectedItem, int rowNumber, String *listMenu) = 0;
    virtual void showYesOrNoSelection2(byte selectedItem, int rowNumber, String *listMenu) = 0;
    virtual void showYesOrNoSelection3(byte selectedItem, int rowNumber, String *listMenu) = 0;
    virtual void showYesOrNoSelection4(byte selectedItem, int rowNumber, bool onOffTemp, bool onOffEc, bool onOffPh) = 0;
    virtual void showLightsTimerSetting(int startHour, int startMinutes, int endHour, int endMinutes) = 0;
    virtual void manageLightsTimerSetting(int row, String one, String two, int h, int m) = 0;
    virtual void showStringNumber(byte i, byte j, int number) = 0;
    virtual void showDateSetting(int row, int y, int m, int d) = 0;
    virtual void showTime(byte _hour, byte _minutes, String title) = 0;
    // Showing row item with selecting symbol in circle way
    virtual void showListMenu(int listSize, int rowItem, String *listMenu) = 0;
    virtual void showMinMaxRangeSetting(int row, String one, int h, int m) = 0;
    virtual void showManageFreqSetting(int row, int t, int e, int p) = 0;
    virtual void showWebRateSending(byte freqT, byte freqE, byte freqP) = 0;
    virtual void showMonitoring(char *key, float value, float temperature) = 0;
    virtual void turnOffLight() = 0;
    virtual void turnOnLight() = 0;
    virtual void connectionStatusPage(char* connStatus) = 0;

    void shiftHourAndMinutesMenu(const int i, const int j, const int keyPad, const String firstS, const String secondS, int row, byte *hourEMin){
      if(row == 0){
        hourEMin[i] = manageMenuRangeNumberCursors(keyPad, row , hourEMin[i], 0, 24);
      }else if(row == 1){
        hourEMin[j] = manageMenuRangeNumberCursors(keyPad, row, hourEMin[j], 0, 60);
      }
      manageLightsTimerSetting(row, firstS, secondS, hourEMin[i], hourEMin[j]);
    }

    void settingNewMinMaxTemperatureRange(const int keyPad, const String firstS, int row, byte *minMax) {
      if(row == 0){
        minMax[0] = manageMenuRangeNumberCursors(keyPad, row , minMax[0], 0, 99);
      }else if(row == 1){
        minMax[1] = manageMenuRangeNumberCursors(keyPad, row, minMax[1], 0, 99);
      }
      showMinMaxRangeSetting(row, firstS, minMax[0], minMax[1]);
    }

    void setFreqUpdateMenu(int keyPad, int row) {
      if(row == 0){
        freqUpdateWebTemperatureIndex = manageMenuRangeNumberCursors(keyPad, row , freqUpdateWebTemperatureIndex, 0, 8);
      }else if(row == 1){
        freqUpdateWebECIndex = manageMenuRangeNumberCursors(keyPad, row, freqUpdateWebECIndex, 0, 8);
      }else if(row == 2){
        freqUpdateWebPHIndex = manageMenuRangeNumberCursors(keyPad, row, freqUpdateWebPHIndex, 0, 8);
      }
      showManageFreqSetting(row, freqUpdateWebTemperatureIndex, freqUpdateWebECIndex, freqUpdateWebPHIndex);
    }

    void setDateMenu(byte i, byte j, byte z, int keyPad, int row, int *date) {
      if (row == 0) { date[z] = manageYearRangeCursor(keyPad, row, date[z], 2060);}
      else if(row == 1){date[j] = manageMenuRangeNumberCursors(keyPad, row, date[j], 1, 13);}
      else{date[i] = manageDayRangeCursor(keyPad, row , date[j],  date[z], date[i]);}
      if (date[i] > _get_max_day(short(date[j] - 1), date[z])){
        date[i] = _get_max_day(short(date[j] - 1), date[z]);}
      showDateSetting(row, date[z], date[j], date[i]);
    }

    bool setMenuAndChangingPage(bool cp, bool ret) {
      setChangingPage(cp);
      return ret;
    }

    bool moveFromMenu(byte menuPage, int row) {
      setActiveMenu(menuPage);
      setRow(row); setColumn(0);
      return setMenuAndChangingPage(true, true);
    }

    bool moveFromSettingMenu(byte menuPage, int row) {
      setActiveMenu(menuPage);
      setRowSettingMenuMem(row);
      setRow(0); setColumn(0);
      return setMenuAndChangingPage(true, true);
    }

    bool moveFromManualMenu(byte menuPage, int row) {
      setActiveMenu(menuPage);
      setRowManualMenuMem(row);
      setRow(0); setColumn(0);
      return setMenuAndChangingPage(true, true);
    }
    
    bool moveFromMainMenu(byte menuPage, int row) {
      setActiveMenu(menuPage);
      setRowMainMenuMem(row);
      setRow(0); setColumn(0);
      return setMenuAndChangingPage(true, true);
    }

    bool comingBackFromMenuPage(byte menuPage, byte row) {
      setActiveMenu(menuPage);
      setRow(row);
      setColumn(0);
      return setMenuAndChangingPage(true, true);
    }

    // Managing the row cursor to move inside the menu
    int manageRowMenuCursors(int keyPad, int menuSize, int exRow) {
      switch (keyPad) {
        case 3:
          if (exRow == 0) {
            return menuSize - 1;
          } else {
            return exRow - 1;
          }
        case 4:
          if (exRow == menuSize - 1) {
            return 0;
          } else {
            return exRow + 1;
          }
        default: return exRow;
      }
    }

    // Managing the col cursor to move inside the menu
    int manageColMenuCursors(int keyPad, int minCol, int maxCol, int exCol) {
      switch (keyPad) {
        case 1:
          if (exCol != maxCol - 1) {
            return exCol + 1;
          } else {
            return exCol;
          }
        case 2:
          if (exCol != minCol) {
            return exCol - 1;
          } else {
            return exCol;
          }
        default: return exCol;
      }
    }

    void exitFromMenu() {
      setRow(0); setColumn(0); setRowMainMenuMem(0); setRowManualMenuMem(0); 
      setRowSettingMenuMem(0); setActiveMenu(1); releSymbolMenu();
    }

    void setChangingPage(bool value){ this->changingPage = value;}

    void setHourAndMinutes(const byte _hour, const byte _minutes){
      hourEMinTimeClockTemp[0] = _hour;
      hourEMinTimeClockTemp[1] = _minutes;  
    }
	
	void setRecoveryHourAndMinutes(const byte _hour, const byte _minutes){
      hourEMinTimeRecovery[0] = _hour;
      hourEMinTimeRecovery[1] = _minutes;  
    }
     
    void setDayMonthYear(const int _day, const int _month, const int _year){
      dateTemp[0] = _day;
      dateTemp[1] = _month;
      dateTemp[2] = _year;
    }
    
    void setTimerLights(int startH, int startM, int endH, int endM){
      hourEMinTemp[0] = startH;
      hourEMinTemp[1] = startM;
      hourEMinTemp[2] = endH;
      hourEMinTemp[3] = endM;
    }

    void setRow(int value){ this->rowItem = value;}

    int getRow(){ return rowItem;}

    void setMonitorEC(bool value){ this->monitorEC = value;}

    bool getMonitorEC(){ return monitorEC;}

    void setConnectionShow(bool value){ this->connectionShow = value;}

    bool getConnectionShow(){ return connectionShow;}

    void setMonitorPH(bool value){ this->monitorPH = value;}

    bool getMonitorPH(){ return monitorPH;}

    void setActiveMenu(byte value){ this->activeMenu = value;}

    byte getActiveMenu(){ return activeMenu;}

    void setColumn(int value){ this->colItem = value;}

    int getColumn(){ return colItem;}

    void setRowMainMenuMem(byte value){ this->rowItemMainMenu = value;}

    void setTemperatureRange(byte _min, byte _max){
      temperatureMinAndMaxTemp[0] = _min;
      temperatureMinAndMaxTemp[1] = _max;  
    }

    void setRowManualMenuMem(byte value){ this->rowItemManualMenu = value;}

    void setRowSettingMenuMem(byte value){ this->rowItemSettingMenu = value;}

    byte getRowMainMenuMem(){ return rowItemMainMenu;}

    byte getRowManualMenuMem(){ return rowItemManualMenu;}

    byte getRowSettingMenuMem(){ return rowItemSettingMenu;}

    byte indexNumber(byte freq){
      if (freq == 23) {
        return indexOf(24, freqNumbber, 8);
      } else {
        return indexOf(freq, freqNumbber, 8);
      }
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

    // C substring function definition
    void substring(char s[], char sub[], int p, int l) {
      int c = 0;
      while (c < l) {
        sub[c] = s[p + c - 1];
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

    int setFreq(int freq) {
      if (freq == 0) {
        return 23;
      } else {
        return freqNumbber[freq];
      }
    }

    // Return index of element starting
    // Return -1 if element is not present
    int indexOf(const int elm, const byte *ar, int ar_cnt) {
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
};

#endif
