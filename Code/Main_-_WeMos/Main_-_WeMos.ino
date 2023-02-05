#include <LiquidCrystal.h>
#include <ESP8266WiFi.h>
#include <RCSwitch.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <FS.h>
#include <ESP8266FtpServer.h>
#include <ESP8266Ping.h>
#include <WiFiClient.h> 

#define UP 1
#define OK 2
#define DOWN 4

String lcdBuffer[32]; //buffer can contain 32 lines of text, first line is menu title, menu items only up to 15 charactres
String keyBuffer; //holds the web keyboard's buffer
String webKeyboardTitle = "***";
String ssid, pass;
byte loadAnim[6][8]; //BMP char array
byte loadPixel = 0b00000; //BMP representation

boolean usingRemote = true;
const byte contrastPin = D7;
unsigned int contrastVal = 300; //contrast level
boolean mdnsF = 0; //mdns state

LiquidCrystal lcd(D0, D1, D2, D3, D4, D5); //RS, EN, D4, D5, D6, D7
RCSwitch remote = RCSwitch(); //Radio data is on pin 6
ESP8266WebServer server(80);
FtpServer ftpSrv;

void buddyStartup(); //logo and startup animation
char buddyRemote(); //decode remote
String buddyDisplayMenu(byte _bufferSize, String _lcdBuffer[32]); //display specified menu
boolean buddyCheckConnection(); //check to see if already connected
byte buddyFindWifi(); //look for available networks
String buddyUserInput(String _title, String _webKeyboardTitle); //opens text box for user input, "***" - default web title
boolean buddyConnect(String _ssid, String _password); //returns 1 if user wants to stay connected
boolean buddySignalTest();
boolean buddyWebServer(); 
boolean buddySaveLogin();
boolean buddySettings();
boolean buddyPing();
String buddySaveSSID(String _ssid); //returns password if exists or entered, '1' if unsuccessful, '0' if unencrypted

//Boolean functions return '1' if unsuccessful

void setup()
{
  pinMode(contrastPin, OUTPUT);
  Serial.begin(115200);
  SPIFFS.begin();
  lcd.begin(16, 2);
  lcd.clear();
  analogWrite(contrastPin, contrastVal);
  buddyStartup();
  if (buddyCheckConnection())
  {
reconnectDevice: ssid = buddyDisplayMenu(buddyFindWifi(), lcdBuffer);
    if (ssid == "Reload list...")
      goto reconnectDevice;
    pass = buddySaveSSID(ssid); //"buddySaveSSID" returns password
    if (buddyConnect(ssid, pass))
    {
      lcd.clear();
      lcd.print("Unsuccessful!");
      lcd.setCursor(0, 1);
      lcd.print("Try again...");
      delay(2000);
      goto reconnectDevice;
    }
  }
  startServer(); //function  defined under "buddyWebServer"
}

void loop()
{
  lcdBuffer[0] = "Main Menu: ";
  lcdBuffer[1] = "1.Signal test";
  lcdBuffer[2] = "2.Web keyboard";
  lcdBuffer[3] = "3.NTP Clock";
  lcdBuffer[4] = "4.Ping";
  lcdBuffer[5] = "5.Settings";
  lcdBuffer[6] = "that's it...";
  String menuSelection = buddyDisplayMenu(7, lcdBuffer);
  if (menuSelection == "1.Signal test")
    buddySignalTest();
  else if (menuSelection == "2.Web keyboard")
    buddyUserInput("WiFiBuddy.local/", "***");
  else if (menuSelection == "3.NTP Clock")
    buddyNTPClient();
  else if (menuSelection == "4.Ping")
    buddyPing();
  else if (menuSelection == "5.Settings")
    buddySettings();
}
