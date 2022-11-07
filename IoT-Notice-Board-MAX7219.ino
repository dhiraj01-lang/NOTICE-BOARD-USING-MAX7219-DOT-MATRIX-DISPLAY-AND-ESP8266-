/* IoT based Notice Board using ESP8266/NodeMCU & MAX7219 8x8 disp LED Display
  Author: Ashish Labade (Tech Vegan) https://www.ashishvegan.com
  Subscribe My YouTube Channel: https://www.youtube.com/channel/UCs_dbtq_OF-0HxkAQnBGapA/
  ####Adafruit-GFX Library https://bit.ly/2APukK7
  ####Max 72XX Panel Library https://bit.ly/2Xc1YBF

  ESP8266 Boards Integration Video https://youtu.be/cw8IGJX-Bio

  ******** Register at www.techvegan.in/iot-notice-board-api and Generate API ********
  **** NOT FOR COMMERCIAL/SELLING USE

*/
#include <ESP8266HTTPClient.h>
#include<ESP8266WiFi.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
int CSP = D4;
int horizontaDisplays = 4;
int verticalDisplays   = 1;
int wifi = D0; // WiFi Onboard LED
const char* ssid = "WiFi Name";
const char* password = "WiFi Password";
String api = "paste_api_here"; // Call 9890345539 for Custom Website for IoT Notice Board
String value;
Max72xxPanel disp = Max72xxPanel(CSP, horizontaDisplays, verticalDisplays);
int duration = 70;
int spacer = 1;
int width  = 5 + spacer;
void setup() {
  Serial.begin(9600);
  pinMode(wifi, OUTPUT);
  digitalWrite(wifi, HIGH); // OFF
  disp.setIntensity(0);
  disp.setRotation(0, 1);
  disp.setRotation(1, 1);
  disp.setRotation(2, 1);
  disp.setRotation(3, 1);
  disp.fillScreen(LOW);
  disp.write();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting to WiFi");
    digitalWrite(wifi, LOW); // ON
    delay(500);
    digitalWrite(wifi, HIGH); // OFF
    delay(500);
  }
  digitalWrite(wifi, LOW); // Permanent WiFi LED ON
  delay(1000);
}
void loop() {
  disp.setIntensity(10);
  disp.fillScreen(LOW);
  HTTPClient http;
  http.begin("http://www.techvegan.in/iot-notice-board-api/script.php?api="+api); // Read Status from Cloud
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    value = http.getString();
    Serial.println(value);
    scroll(value);
    delay(1000);
  }
  else
  {
    scroll("SORRY! NO INTERNET");
    delay(1000);
  }
}
void scroll(String message) {
  for ( int i = 0 ; i < width * message.length() + disp.width() - spacer; i++ ) {
    int charac = i / width;
    int x = (disp.width() - 1) - i % width;
    int y = (disp.height() - 8) / 2;
    while ( x + width - spacer >= 0 && charac >= 0 ) {
      if ( charac < message.length() ) {
        disp.drawChar(x, y, message[charac], HIGH, LOW, 1);
      }
      charac--;
      x -= width;
    }
    disp.write();
    delay(duration / 2);
  }
}
