/*
  Aquarium Project Pasquale
*/
#include <string.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <SPI.h>
#include <StreamUtils.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <MemoryFree.h>
#include <EEPROM.h>


/***************************************************************/

#define TdsSensorPin A1
#define VREF 4.5 // analog reference voltage(Volt) of the ADC
#define SCOUNT 30 // sum of sample point
// All of the backpacks like the one shown are at 0x27.
#define I2C_ADDR   0x27 // <--Change to match your display. Use scanner.ino to find address.
#define DS18B20_Pin  42
#define SD_PIN 53
OneWire ds(DS18B20_Pin);
RTC_DS1307 RTC;
LiquidCrystal_I2C lcd(I2C_ADDR, 16, 2);
const byte keypadPin = A0;

/*MAPPATURA PIN ARDUINO MEGA 2560
   ------------------I/O DIGITALI--------------------------------
   00 NON DISPONIBILE
   01 NON DISPONIBILE
   02 RELE' in 1
   03 RELE' in 2
   04 RELE' in 3
   05 RELE' in 4
   06 RELE' in 5
   22 ingresso DS18S20
   50 miso
   51 mosi
   52 sck
   53 cs
   ---------------------------------------------------------------
   5v alimentazione moduli
   gnd alimentazione moduli
   gnd alimentazione board
   VIN alimentazione board
   -----------------I ANALOGICI-----------------------------------
   A0 Tasti keypad
   A1 TDS Meter v 1.0 KS0429
   A2 LIBERO
   A3 LIBERO
   A4 SDA I2C RTC
   A5 SCL IC2 RTC
*/

boolean onOffLightAuto = true;
boolean onOffHeater = true;
boolean onOffTDS = true;
boolean onOffPH = true;
boolean onOffTemperature = true;
boolean onOffTemperatureSending = true;
boolean onOffTDSSending = true;
boolean onOffPhSending = true;

/****************************************************************/
// Global Variables
byte oraEMinTimeClockTemp[2] = {0, 0};
int dateTemp[3] = {0, 0, 0};
byte oraEMinTemp[4] = {0, 0, 0, 0};
byte temperatureMinAndMaxTemp[2] = {0, 0};
int temperature = 0;
int freqNumbber[8] = {24, 12, 8, 6, 4, 3, 2, 1};
int freqUpdateWebTemperatureIndex = 0;
int freqUpdateWebTDSIndex = 0;
int freqUpdateWebPHIndex = 0;
byte rele[5] = {2, 3, 4, 5, 6};
// ------------------  TDS Meter  --------------------------------
bool activeContinuousTDSMeasurement = false;
float tds = 0.0;
float tds_continuous = 0.0;
// ------------------   SD varables   ----------------------------
struct Config {
  int startHour;
  int startMinutes;
  int endHour;
  int endMinutes;
  byte tempMax;
  byte tempMin;
  bool autoEnabled;
  bool manteinEnabled;
  bool onOffLightAuto;
  bool onOffHeater;
  bool onOffTDS;
  bool onOffPH;
  bool onOffTemperature;
  bool onOffFilter;
  bool onOffTemperatureSending;
  bool onOffTDSSending;
  bool onOffPhSending;
  int freqUpdateWebTemperature;
  int freqUpdateWebTDS;
  int freqUpdateWebPH;
  char hostname[64];
};
const char *filename = "SETTINGS.TXT";  // <- SD library uses 8.3 filenames
Config config;                         // <- global configuration object
// ------------------   WIFI variables   -------------------------
char *keyTemp = "Temp";
char *keyTDS = "Ec";
char *keyPh = "Ph";
char *connStatus = "";
// ------------------   Frame rate   -----------------------------
int frp = 1000 / 4;
// ---------------   KeyPad variables   --------------------------
int key = 0;
int code[7] = {1, 2, 3, 4, 5, 6, 0};
int keypad_value = 0;
// ------------   Display timer becklight   ----------------------
int timerOfLight = (1000 * 30) / frp ;
bool screenBeckLightOnOff = false;
int _timerLightDisplay = 0; //temp
// ---------------   Setting Day Time   --------------------------
char buffer[16];
byte previousSec = 0;
byte previousSecSend = 55; //random
byte previousSecSendTDS = 55; //random
byte H = 0;
byte M = 0;
byte S = 0;
// -------------------   Menu variables   ------------------------
int activeMenu = 0; //1: Main, 2: Manual, 3:Settings, 4:On Or Off, 5:Yes Or No, 6:Show Time, 7:Set Start Time Or Real Time, 8:Set End Time
bool menuOnOff = false;
bool changingPage = false;
// ------------------   LCD variables   --------------------------
byte startReleSimbolsOnLCD = 11;
int releSymbol[5] = {0, 0, 0, 0, 0};
int numRele = 5;
//-------------------   LCD Special characters   -----------------
uint8_t SPENTO[8] = {0x0E, 0x11, 0x11, 0x11, 0x0A, 0x0E, 0x0E, 0x04,};
uint8_t ACCESO[8] = {0x0E, 0x1F, 0x1F, 0x1F, 0x0E, 0x0E, 0x0E, 0x04,};
byte gradi[8] = {B01110, B01010, B01110, B00000, B00000, B00000, B00000, B00000};
byte Smile[8] = {B00000, B10001, B00000, B00000, B10001, B01110, B00000,};
byte Freccia_su[8] = {B00100, B00100, B01110, B01110, B01110, B11111, B11111, B11111,};
byte Freccia_lato_sx[8] = {B10000, B11000, B11110, B11111, B11111, B11110, B11000, B10000,};
byte Freccia_lato_dx[8] = {B00001, B00011, B01111, B11111, B11111, B01111, B00011, B00001,};
byte V[8] = {B00001, B00001, B00001, B00001, B00001, B1001, B01100, B00100,};
byte vuoto[8] = {B00000, B00000, B00000, B00000, B00000, B0000, B00000, B00000,};
byte off[8] = {B11111, B10001, B10001, B10001, B10001, B10001, B11111, B00000,};
byte on[8] = {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B00000,};
byte g_off[8] = {B00100, B01110, B01110, B01110, B00100, B00100, B00100, B00100,};
byte g_on[8] = {B00100, B00100, B00100, B00100, B01110, B01110, B01110, B00100,};

const int maxSize = 16;
//-----------------------   Main   ------------------------------
char menuItem0[maxSize] = "Automatico";
char menuItem1[maxSize] = "Manuale";
char menuItem2[maxSize] = "Manutenzione";
char menuItem3[maxSize] = "Impostazioni";
//---------------------   Manuale   -----------------------------
char menuItem4[maxSize] = "Luci";
char menuItem5[maxSize] = "Filtro";
char menuItem6[maxSize] = "Riscaldatore";
char menuItem7[maxSize] = "Ossigeno";
char menuItem8[maxSize] = "Mangiatoia";
char menuItem9[maxSize] = "Termometro";
char menuItem18[maxSize] = "TDS Meter";
char menuItem19[maxSize] = "PH Meter";
char menuItem14[maxSize] = "Enable T Send";
char menuItem15[maxSize] = "Enable EC Send";
char menuItem16[maxSize] = "Enable PH Send";
//-----------------------   Settings   --------------------------
char menuItem10[maxSize] = "Set Timer Luci";
char menuItem11[maxSize] = "Set Orario";
char menuItem21[maxSize] = "Set Data";
char menuItem12[maxSize] = "Conn. Status";
char menuItem13[maxSize] = "Reconnect";
char menuItem17[maxSize] = "Set Heater Auto";
char menuItem20[maxSize] = "Set Freq to web";
//----------------   Initialize Menu Array   ---------------------
char *mainMenu[4];
int mainMenuSize = 0;
char *manualMenu[11];
int manualMenuSize = 0;
char *settingMenu[7];
int settingMenuSize = 0;

//--------------------   Serial3 Variables -----------------------
String inString;
//
/***********************************  SETUP  *********************/
//
void setup() {
  //Initialize in and out port
  Serial.begin(115200);
  while (!Serial) continue;
  Serial3.begin(115200);

  //Set pin for TSD Meter
  pinMode(TdsSensorPin, INPUT);

  //pinMode(PIN_LED, OUTPUT);
  //digitalWrite(PIN_LED, LOW);

  // Initialize SD library
  if (!SD.begin(SD_PIN)) {
    Serial.println(F("Failed to initialize SD library"));
  } else {
    Serial.println(F("Initialize SD library"));
  }
  
  if (SD.exists(filename)) {
    // Should load default config if run for the first time
    Serial.println(F("Loading configuration..."));
    loadConfiguration(filename, config);
  } else {
    // Create configuration file
    Serial.println(F("Setting configuration..."));
    setConfiguration(filename, config);
  }

  // Dump config file
  //Serial.println(F("Print config file..."));
  //printFile(filename);

  Wire.begin();
  RTC.begin();
  //Initialize Display LCD
  lcd.init();
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  //Setting the analogue pin for input and turn on the internal pullup resistor
  pinMode(keypadPin, INPUT_PULLUP);
  for (int i = 0; i < numRele; i++) {
    pinMode(rele[i], OUTPUT);
    digitalWrite(rele[i], HIGH);
  }
  //Setting Auto Process or maintenance
  if (config.autoEnabled) {
    onAutomaticProcess();
  } else if (config.manteinEnabled) {
    offAutomaticProcess();
  }

  lcd.backlight();
  //Setting lcd special characters
  lcd.createChar(0, SPENTO);
  lcd.createChar(1, ACCESO);
  lcd.createChar(2, gradi);
  lcd.createChar(3, Smile);
  lcd.createChar(4, Freccia_su);
  lcd.createChar(5, Freccia_lato_dx);
  lcd.createChar(6, Freccia_lato_sx);

  initScreen();

  //Setting delay
  delay(4000);

  //Turning off screen backlight
  lcd.noBacklight();
  lcd.clear();

  //Printing rele Symbols
  showReleSymbol();
  //-----------------------   Main Menu Page   ----------------------
  mainMenu[0] = menuItem0;
  mainMenu[1] = menuItem1;
  mainMenu[2] = menuItem2;
  mainMenu[3] = menuItem3;
  mainMenuSize = sizeof(mainMenu) / sizeof(mainMenu[0]);
  //-----------------------   Manual Menu Page  ---------------------
  manualMenu[0] = menuItem4;
  manualMenu[1] = menuItem5;
  manualMenu[2] = menuItem6;
  manualMenu[3] = menuItem7;
  manualMenu[4] = menuItem8;
  manualMenu[5] = menuItem9;
  manualMenu[6] = menuItem18;
  manualMenu[7] = menuItem19;
  manualMenu[8] = menuItem14;
  manualMenu[9] = menuItem15;
  manualMenu[10] = menuItem16;
  manualMenuSize = sizeof(manualMenu) / sizeof(manualMenu[0]);
  //-----------------------   Setting Menu Page   -------------------
  settingMenu[0] = menuItem10;
  settingMenu[1] = menuItem11;
  settingMenu[2] = menuItem21;
  settingMenu[3] = menuItem12;
  settingMenu[4] = menuItem13;
  settingMenu[5] = menuItem17;
  settingMenu[6] = menuItem20;
  settingMenuSize = sizeof(settingMenu) / sizeof(settingMenu[0]);
}

//
/***************************   LOOP   *****************************/
//
void loop() {
  //Getting time from RTC Modul
  DateTime now = RTC.now();
  S = now.second();
  if (S != previousSec) {
    H = now.hour();
    M = now.minute();
    sprintf(buffer, "%02d:%02d:%02d", H, M, S);
    //    Serial.println(buffer);
    previousSec = S;
  }
  //Check when turning on lights
  if (config.onOffLightAuto) {
    manageAquariumLights(H, M);
  }

  //Getting changingPageature if it is enabled
  if (config.onOffTemperature) {
    temperature = getTemp();
  }
  
  //Get TDS values automatically only when it is the right time to get it 
  if (config.onOffTDS) {
    tds = activateTDSMeasurement(config.freqUpdateWebTDS, 10, temperature);
  }

  //turning on the heater automatically if it is enabled
  if (config.onOffHeater) {
    manageAcquarioumHeater(temperature);
  }

  //Getting kayboard value
  key = getKeyValue();
  if (!menuOnOff && key == code[4]) {
    menuOnOff = true;
    activeMenu = 1;
    changingPage = true;
  }

  //  Serial.println("TEMPERATURE = " + String(temperature));
  //  Serial.println("KEY PAD VALUE = " + String(key));
  //  Serial.println("ACTIVE MENU = " + String(activeMenu));
  //  Serial.println("MENU = " + String(menuOnOff));
  //  Serial.println("TDS = " + String(tds));

  //Check when turning on the screen backlight
  checkScreenBeckLight(key);

  //Sent the temperature to WEB DB
  if (config.onOffTemperatureSending) {
    chackIfSendTempValue(config.freqUpdateWebTemperature, 0, temperature, keyTemp, now);
  }
  
  //Sent the TDS to WEB DB 5 minute later than tds measurement
  if (config.onOffTDSSending) {
    chackIfSendTDSValue(config.freqUpdateWebTDS, 15, float(tds), keyTDS, now);
  }

  //menu on lcd
  if (menuOnOff) {
    menu_(key);
  } else { // menu off
    mainScreen();
  }
  delay(frp);
  resetVar();
}
