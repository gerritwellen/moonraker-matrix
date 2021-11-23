#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define USE_SERIAL Serial

// User config
// more info in the README.md in the root of the repository

#ifndef CONFIG // not #ifnotdef
#define CONFIG

const char *SSID = "Wave";                    // wlan SSID
const char *WPWD = "Cherub-Remorse-Overhang"; // wlan password
String PRINTER_IP = "10.0.0.20";              // the ip address of octoprint
uint16_t pollInterval = 500;                  // ms, lower value will increase poll rate, but the animation time will limit the poll rate too
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN D8
#define DATA_PIN D6
#define CLK_PIN D5

#endif
