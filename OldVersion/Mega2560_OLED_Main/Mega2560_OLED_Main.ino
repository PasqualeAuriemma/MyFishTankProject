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
#include "DFRobot_PH.h"
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/***************************************************************/

/* PH */
#define PH_PIN A2
float voltage, phValue;
DFRobot_PH ph;

/*EC - TDS*/
#define TdsSensorPin A1
#define VREF 4.60// analog reference voltage(Volt) of the ADC
#define SCOUNT 30 // sum of sample point

// All of the backpacks like the one shown are at 0x27.
#define I2C_ADDR   0x27 // <--Change to match your display. Use scanner.ino to find address.
#define DS18B20_Pin  42


OneWire ds(DS18B20_Pin);


RTC_DS3231 RTC;
const byte keypadPin = A0;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin) 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   79
#define LOGO_WIDTH    77

#define SD_PIN 53
static const unsigned char PROGMEM salmon[770] = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
  0xff, 0xff, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xff, 0xff, 0xfe,
  0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xfe,
  0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xfe,
  0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xfe,
  0xff, 0xff, 0xfe, 0x00, 0x7e, 0x00, 0x00, 0x3f, 0xff, 0xfe,
  0xff, 0xff, 0xf8, 0x00, 0x1f, 0xfc, 0x00, 0x0f, 0xff, 0xfe,
  0xff, 0xff, 0xe0, 0x00, 0x00, 0xff, 0xc0, 0x03, 0xff, 0xfe,
  0xff, 0xff, 0xc0, 0x00, 0x00, 0x1f, 0xf8, 0x01, 0xff, 0xfe,
  0xff, 0xff, 0x80, 0x00, 0x00, 0x03, 0xfe, 0x00, 0xff, 0xfe,
  0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x7f, 0xfe,
  0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x3f, 0xfe,
  0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x1f, 0xfe,
  0xff, 0xf8, 0x00, 0x60, 0x00, 0x00, 0x07, 0xfc, 0x0f, 0xfe,
  0xff, 0xf0, 0x00, 0xc0, 0x00, 0x00, 0x01, 0xff, 0x07, 0xfe,
  0xff, 0xe0, 0x01, 0x80, 0x00, 0x00, 0x00, 0xff, 0x83, 0xfe,
  0xff, 0xc0, 0x03, 0x80, 0x00, 0x00, 0x00, 0x7f, 0xc1, 0xfe,
  0xff, 0xc0, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x3f, 0xe1, 0xfe,
  0xff, 0x82, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0xfe,
  0xff, 0x02, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0xfe,
  0xff, 0x06, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfc, 0x7e,
  0xfe, 0x06, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x7e,
  0xfe, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x3e,
  0xfc, 0x0f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xff, 0xbe,
  0xfc, 0x1f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x9e,
  0xfc, 0x3f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xde,
  0xf8, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xee,
  0xf8, 0x3f, 0xdf, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xee,
  0xf8, 0x7f, 0xe3, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfe,
  0xf0, 0x7f, 0xfc, 0x00, 0x00, 0x00, 0x02, 0x00, 0x0f, 0xfe,
  0xf0, 0x7f, 0xff, 0x00, 0x00, 0x00, 0x01, 0x80, 0x0f, 0xfe,
  0xf0, 0x7f, 0xff, 0xe0, 0x00, 0x00, 0x01, 0xc0, 0x07, 0xfe,
  0xf0, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0xe0, 0x07, 0xfe,
  0xf0, 0x7f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0xf0, 0x03, 0xfe,
  0xf0, 0x7f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0xf8, 0x03, 0xfe,
  0xf0, 0x3f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0xfc, 0x23, 0xfe,
  0xe0, 0x3f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7e, 0x21, 0xfe,
  0xe0, 0x3f, 0xfe, 0xff, 0xff, 0x80, 0x00, 0x7f, 0x31, 0xfe,
  0xe0, 0x1f, 0xff, 0xfe, 0xff, 0xc0, 0x00, 0x7f, 0xf9, 0xfe,
  0xf0, 0x1f, 0xfd, 0xfe, 0xff, 0x40, 0x00, 0xff, 0xf8, 0xfe,
  0xf0, 0x0f, 0xfd, 0xff, 0xf9, 0x80, 0x00, 0xff, 0xfc, 0xfe,
  0xf0, 0x07, 0xfd, 0xff, 0xdf, 0x80, 0x01, 0xff, 0xfe, 0xfe,
  0xf0, 0x01, 0xfd, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xfe, 0xfe,
  0xf0, 0x00, 0x7e, 0xff, 0xf8, 0x00, 0x1f, 0xf9, 0xfe, 0xfe,
  0xf0, 0x3f, 0xff, 0x7f, 0xe0, 0x00, 0x07, 0xe7, 0xfe, 0x7e,
  0xf0, 0x3f, 0xff, 0xdf, 0x00, 0x00, 0x00, 0x0f, 0xfe, 0x7e,
  0xf0, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x7e,
  0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0x7e,
  0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0x7e,
  0xf8, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xfe, 0x7e,
  0xfc, 0x10, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xfe, 0x7e,
  0xfc, 0x10, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x7e,
  0xfc, 0x08, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xfe, 0x7e,
  0xfe, 0x0c, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xfe, 0x7e,
  0xfe, 0x04, 0x00, 0x00, 0x01, 0xff, 0x3f, 0xbf, 0xfc, 0x7e,
  0xff, 0x06, 0x00, 0x00, 0x00, 0x7f, 0xbf, 0xdf, 0xf8, 0xfe,
  0xff, 0x03, 0x00, 0x00, 0x00, 0xcf, 0xff, 0xdf, 0xf8, 0xfe,
  0xff, 0x83, 0x80, 0x00, 0x00, 0xfb, 0xff, 0xdf, 0xe0, 0xfe,
  0xff, 0x81, 0xc0, 0x00, 0x00, 0x7f, 0xff, 0xbf, 0xc1, 0xfe,
  0xff, 0xc1, 0xe0, 0x00, 0x00, 0x1f, 0xff, 0x3f, 0x01, 0xfe,
  0xff, 0xe0, 0xf0, 0x00, 0x00, 0x03, 0xfe, 0xff, 0xff, 0xfe,
  0xff, 0xf0, 0x78, 0x00, 0x00, 0x00, 0x7d, 0xff, 0xff, 0xfe,
  0xff, 0xf8, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe,
  0xff, 0xf8, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfe,
  0xff, 0xfc, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfe,
  0xff, 0xfe, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe,
  0xff, 0xff, 0x87, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe,
  0xff, 0xff, 0xc3, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe,
  0xff, 0xff, 0xe1, 0xff, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfe,
  0xff, 0xff, 0xf8, 0xff, 0xe0, 0x00, 0x00, 0x1f, 0xff, 0xfe,
  0xff, 0xff, 0xfc, 0x7f, 0xfc, 0x00, 0x00, 0xff, 0xff, 0xfe,
  0xff, 0xff, 0xff, 0x1f, 0xff, 0xc0, 0x07, 0xff, 0xff, 0xfe,
  0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
  0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe
};

/*MAPPATURA PIN ARDUINO MEGA 2560
   ------------------I/O DIGITALI--------------------------------
   00 NON DISPONIBILE
   01 NON DISPONIBILE
   02 RELE' in 1
   03 RELE' in 2
   04 RELE' in 3
   05 RELE' in 4
   42 ingresso DS18S20
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
   A1 TDS-EC Meter v 1.0 KS0429
   A2 PH
   A3 LIBERO
   A4 SDA I2C RTC
   A5 SCL IC2 RTC
*/

boolean onOffLightAuto = true;
boolean onOffHeater = true;
boolean onOffEC = true;
boolean onOffPH = true;
boolean onOffTemperature = true;
boolean onOffTemperatureSending = true;
boolean onOffECSending = true;
boolean onOffPhSending = true;
bool manualSendingTemperature = false;

/****************************************************************/
// Global Variables
byte oraEMinTimeClockTemp[2] = {0, 0};
int dateTemp[3] = {0, 0, 0};
byte oraEMinTemp[4] = {0, 0, 0, 0};
byte temperatureMinAndMaxTemp[2] = {0, 0};
float temperature = 0.0;
int freqNumbber[8] = {24, 12, 8, 6, 4, 3, 2, 1};
int freqUpdateWebTemperatureIndex = 0;
int freqUpdateWebECIndex = 0;
int freqUpdateWebPHIndex = 0;
byte rele[5] = {2, 3, 4, 5};
// ------------------  EC Meter  --------------------------------
float ec = 0.0;
bool monitorEC = false;
bool manualSendingEC = false;

// ------------------  PH GRAVITY  -------------------------------
bool monitorPH = false;
float phFinal = 0.0;
bool manualSendingPH = false;

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
  bool onOffEC;
  bool onOffPH;
  bool onOffTemperature;
  bool onOffFilter;
  bool onOffTemperatureSending;
  bool onOffECSending;
  bool onOffPhSending;
  int freqUpdateWebTemperature;
  int freqUpdateWebEC;
  int freqUpdateWebPH;
  byte hourLoading;
  byte minLoading;
  char hostname[64];
};
const char *filename = "SETTINGS.TXT";  // <- SD library uses 8.3 filenames
Config config;                         // <- global configuration object
// ------------------   WIFI variables   -------------------------
char *keyTemp = "Temp";
char *keyEC = "Ec";
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
byte previousSecSendEC = 55; //random
byte previousSecSendPH = 55; //random
byte previousSecResend = 55;
byte H = 0;
byte M = 0;
byte S = 0;
byte D = 0;
// -------------------   Menu variables   ------------------------
int activeMenu = 0; //1: Main, 2: Manual, 3:Settings
bool menuOnOff = false;
bool changingPage = false;
// ------------------   Rele variables   --------------------------

int releSymbol[5] = {0, 0, 0, 0};
int numRele = 4;
const int maxSize = 16;
//-----------------------   Main   ------------------------------
char menuItem0[maxSize] = "Automation";
char menuItem1[maxSize] = "Manual";
char menuItem2[maxSize] = "Maintenance";
char menuItem3[maxSize] = "Settings";
//---------------------   Manuale   -----------------------------
char menuItem4[maxSize] = "Light";
char menuItem5[maxSize] = "Filter";
char menuItem6[maxSize] = "Heater";
char menuItem7[maxSize] = "Oxygen";
char menuItem22[maxSize] = "Monitoring EC";
char menuItem23[maxSize] = "Monitoring PH";
char menuItem26[maxSize] = "Send Temperat.";
char menuItem24[maxSize] = "Send EC";
char menuItem25[maxSize] = "Send PH";

//-----------------------   Settings   --------------------------
char menuItem10[maxSize] = "Timer Light";
char menuItem11[maxSize] = "Time";
char menuItem21[maxSize] = "Data";
char menuItem12[maxSize] = "Connection";
char menuItem13[maxSize] = "Reconnect";
char menuItem17[maxSize] = "Heater Auto";
char menuItem20[maxSize] = "WebRate sending";

char menuItem9[maxSize] =  "Thermometer";
char menuItem18[maxSize] = "EC Meter";
char menuItem19[maxSize] = "PH Meter";
char menuItem14[maxSize] = "Temp. Sending";
char menuItem15[maxSize] = "EC/TDS Sending";
char menuItem16[maxSize] = "PH Sending";
//----------------   Initialize Menu Array   ---------------------
char *mainMenu[4];
int mainMenuSize = 0;
char *manualMenu[9];
int manualMenuSize = 0;
char *settingMenu[13];
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

  //SD.remove(filename);
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

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  Wire.begin();
  RTC.begin();
  if (! RTC.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (RTC.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
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

  // message welcome
  initScreen();

  //Printing rele Symbols
  showReleSymbol();
  //-----------------------   Main Menu Page   ----------------------
  mainMenu[0] = menuItem1;
  mainMenu[1] = menuItem0;
  mainMenu[2] = menuItem2;
  mainMenu[3] = menuItem3;
  mainMenuSize = sizeof(mainMenu) / sizeof(mainMenu[0]);
  //-----------------------   Manual Menu Page  ---------------------
  manualMenu[0] = menuItem4;
  manualMenu[1] = menuItem5;
  manualMenu[2] = menuItem6;
  manualMenu[3] = menuItem7;
  manualMenu[4] = menuItem22;
  manualMenu[5] = menuItem23;
  manualMenu[6] = menuItem24;
  manualMenu[7] = menuItem25;
  manualMenu[8] = menuItem26;

  manualMenuSize = sizeof(manualMenu) / sizeof(manualMenu[0]);
  //-----------------------   Setting Menu Page   -------------------
  settingMenu[0] = menuItem10;
  settingMenu[1] = menuItem11;
  settingMenu[2] = menuItem21;
  settingMenu[3] = menuItem12;
  settingMenu[4] = menuItem13;
  settingMenu[5] = menuItem17;
  settingMenu[6] = menuItem20;
  settingMenu[7] = menuItem9;
  settingMenu[8] = menuItem18;
  settingMenu[9] = menuItem19;
  settingMenu[10] = menuItem14;
  settingMenu[11] = menuItem15;
  settingMenu[12] = menuItem16;

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
    D = now.day();
    //Serial.println(buffer);
    previousSec = S;
  }
  
  //Check when loading Measure not sended because of some issues
  if (config.onOffTemperatureSending || config.onOffPhSending || config.onOffECSending){
    chackWhenResendMeasure(config.hourLoading, config.minLoading);
  }
  
  //Check when turning on lights
  if (config.onOffLightAuto) {
     manageAquariumLights(H, M);
  }

  //Getting changingPageature if it is enabled
  if (config.onOffTemperature) {
    temperature = getTemp();
  }

  // Get EC monitoring values if it is selected by menu
  if (monitorEC) {
    ec = getEC(temperature);
    showMonitoring(keyEC, ec, temperature);
  }

  // Sending EC values to web page if it is selected by menu
  if (manualSendingEC) {
    sendValueToWeb(ec, keyEC, now);
    manualSendingEC = false;
  }

  // Get PH monitoring values if it is selected by menu
  if (monitorPH) {
    phFinal = getPH(temperature);
    showMonitoring(keyPh, phFinal, temperature);
  }

  // Sending PH values to web page if it is selected by menu
  if (manualSendingPH) {
    sendValueToWeb(phFinal, keyPh, now);
    manualSendingPH = false;
  }

  // Sending Temperature values to web page if it is selected by menu
  if (manualSendingTemperature) {
    sendValueToWeb(temperature, keyTemp, now);
    manualSendingTemperature = false;
  }

  //ec = getEC(temperature);
  //Serial.println(ec);
  //Get EC values automatically only when it is the right time to get it
  if (config.onOffEC) {
    ec = activateECMeasurement(config.freqUpdateWebEC, 10, temperature);
  }

  //Serial.println(getEC(25.6));
  //Get PH values automatically only when it is the right time to get it
  if (config.onOffPH) {
    phFinal = activatePHMeasurement(config.freqUpdateWebPH, 32, temperature);
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
  //  Serial.println("EC = " + String(ec));

  //Check when turning on the screen backlight
  checkScreenBeckLight(key);

  //Sent the temperature to WEB DB
  if (config.onOffTemperatureSending) {
    chackIfSendTempValue(config.freqUpdateWebTemperature, 0, temperature, keyTemp, now);
  }

  //Sending EC value to WEB DB 5 minute later after ec measurement
  if (config.onOffECSending) {
    chackIfSendECValue(config.freqUpdateWebEC, 15, ec, keyEC, now);
  }

  //Sending PH value to WEB DB 5 minute later after PH measurement
  if (config.onOffPhSending) {
    chackIfSendPHValue(config.freqUpdateWebPH, 37, phFinal, keyPh, now);
  }

  //menu on screen
  if (menuOnOff) {
    menu_(key);
  } else { // menu off
    mainScreen();
  }

  delay(frp);
  resetVar();
}
