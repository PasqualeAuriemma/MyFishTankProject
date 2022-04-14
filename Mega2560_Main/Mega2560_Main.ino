/*
  Aquarium Project Pasquale
*/
#include <string.h>
#include <StreamUtils.h>
#include <SoftwareSerial.h>
#include <RTClib.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <SPI.h>
#include <MemoryFree.h>
#include <EEPROM.h>
#include <OneWire.h>
#include "DFRobot_PH.h"
#include "MenuScreen/LCD.h"
#include "MenuScreen/OLED.h"

/***************************************************************/
#define CHOOSE_SCREEN 2 // 1 LCD - 2 OLED 
#define DS18B20_Pin  42
#define PH_PIN A2
#define SD_PIN 53
#define TdsSensorPin A7
#define VREF 5.00// analog reference voltage(Volt) of the ADC
#define SCOUNT 30 // sum of sample point
const byte keypadPin = A0;

OneWire ds(DS18B20_Pin);
RTC_DS3231 RTC;
DFRobot_PH ph;
Screen *screen;

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
   5v alimentazione board
   gnd alimentazione board
   -----------------I ANALOGICI-----------------------------------
   A0 Tasti keypad
   A7 TDS-EC Meter v 1.0 KS0429
   A2 PH
   A3 LIBERO
   SDA I2C RTC
   SCL IC2 RTC
*/

//-----------------------------------------------------------------------------------------------------------

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

float temperature = 0.0;
byte rele[5] = {2, 3, 4, 5};
// ------------------  EC Meter  --------------------------------
float ec = 0.0;
bool manualSendingEC = false;
// ------------------  PH GRAVITY  -------------------------------
float phFinal = 0.0;
bool manualSendingPH = false;
float voltage, phValue;
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
  byte freqUpdateWebTemperature;
  byte freqUpdateWebEC;
  byte freqUpdateWebPH;
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
byte code[7] = {1, 2, 3, 4, 5, 6, 0};
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
byte previousSecResend = 55; //random
byte H = 0;
byte M = 0;
byte S = 0;
byte D = 0;
// -------------------   Menu variables   ------------------------
bool menuOnOff = false;
// ------------------   Rele variables   --------------------------
int releSymbol[4] = {0, 0, 0, 0};
int const numRele = sizeof(releSymbol) / sizeof(releSymbol[0]);

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
  
  // Calling Oled constructor with okButton in input to know 
  if (CHOOSE_SCREEN == 1){
    screen = new Lcd(code[4]); 
  }else{
    screen = new Oled(code[4]); 
  }
  // message welcome
  screen->initScreen();

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
  
  //Printing rele Symbols
  screen->releSymbolMenu();
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
  if(screen->getMonitorEC()){
    ec = getEC(int(temperature));
    screen -> showMonitoring(keyEC, ec, temperature); 
  }  
  
  // Sending EC values to web page if it is selected by menu
  if(screen->getManualSendingEC()){
    sendValueToWeb(ec, keyEC, now); 
    screen->getManualSendingEC();
  }  
  
//  // Get PH monitoring values if it is selected by menu
  if(screen->getMonitorPH()){
    phFinal = getPH(int(temperature));
    screen -> showMonitoring(keyPh, phFinal, temperature);
  }

  if(screen->getConnectionShow()){
    screen->connectionStatusPage(connStatus);
  }

  // Sending PH values to web page if it is selected by menu
  if(screen->getManualSendingPH()){
    sendValueToWeb(phFinal, keyPh, now); 
    screen->setManualSendingPH(false);
  }

  // Sending Temperature values to web page if it is selected by menu
  if(screen->getManualSendingTemperature()){
    sendValueToWeb(temperature, keyTemp, now); 
    screen->setManualSendingTemperature(false);
  }  

  //Get EC values automatically only when it is the right time to get it
  if (config.onOffEC) {
    ec = activateECMeasurement(config.freqUpdateWebEC, 10, temperature);
  }

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
  }
   
  //  Serial.println("KEY PAD VALUE = " + String(key));
  //  Serial.println("ACTIVE MENU = " + String(activeMenu));
  //  Serial.println("MENU = " + String(menuOnOff));
  //  Serial.println("EC = " + String(ec));

  //Check when turning on the screen backlight
  screen->checkScreenBeckLight(key, code[6]);

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
    menuOnOff = screen->menu_(key);
  } else { // menu off
    screen->mainScreen(buffer, temperature, ec, phFinal);
  }
  
  delay(frp);
  resetVar();
}
