#include <config.h>

DynamicJsonDocument doc(1024);
ESP8266WiFiMulti WiFiMulti;
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
uint8_t timeouts = 0;

//Querry URL
String prefix = "http://";
String postfix = "/printer/objects/query?virtual_sdcard&print_stats&extruder&heater_bed";
String url = prefix + PRINTER_IP + postfix;
String URL = url.c_str();

int i = 0;
char *message = "Network not found";
char buffer[50];
char divider1[] = "C / ";
char divider2[] = "C / ";
char divider3[] = "%";

void setup()
{
  USE_SERIAL.begin(115200);
  USE_SERIAL.println("");
  USE_SERIAL.println("Setup start");
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SSID, WPWD);
  myDisplay.begin();
  myDisplay.setIntensity(0);
  myDisplay.setTextAlignment(PA_CENTER);
  // myDisplay.setPause(1000);
  myDisplay.setPause(0);
  myDisplay.setSpeed(100);
  myDisplay.displayClear();
  USE_SERIAL.println("Setup finished");
}
void loop()
{
  if (myDisplay.displayAnimate())
  {
    if (WiFiMulti.run() == WL_CONNECTED)
    {
      WiFiClient c;
      HTTPClient http;
      http.begin(c, URL);
      int httpCode = http.GET();

      if (httpCode > 0)
      {
        timeouts = 0;
        String payload = http.getString();
        deserializeJson(doc, payload);
        JsonObject payloadObject = doc.as<JsonObject>();
        if (httpCode == HTTP_CODE_OK)
        {
          int e_temp = payloadObject["result"]["status"]["extruder"]["temperature"];
          int b_temp = payloadObject["result"]["status"]["heater_bed"]["temperature"];
          float_t progressFloat = payloadObject["result"]["status"]["virtual_sdcard"]["progress"];
          int progress = (progressFloat * 100);
          int n;
          n = sprintf(buffer, "E:%dC/B:%dC - %d%%", e_temp, b_temp, progress);
          message = buffer;
        }
      }
      else
      {
        timeouts++;
        if (timeouts > 10)
        {
          message = "Printer not found";
        }
      }
    }
    else
    {
      message = "Network not found";
    }
    if (myDisplay.displayAnimate())
    {
      myDisplay.displayText(message, myDisplay.getTextAlignment(), myDisplay.getSpeed(), myDisplay.getPause(), PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      myDisplay.displayReset();
    }
  }
}