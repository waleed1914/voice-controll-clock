#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <DHT.h>
#include <SPI.h>
#include <Wire.h>
#include "Font7Seg.h"
 
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4  // Define the number of displays connected
#define CLK_PIN 13     // CLK or SCK
#define DATA_PIN 11    // DATA or MOSI
#define CS_PIN 10      // CS or SS
#define SPEED_TIME 75  // Speed of the transition
#define PAUSE_TIME 0
#define MAX_MESG 20
 
 
// These are for the temperature
#define DHTPIN 2
#define DHTTYPE DHT11
#define TIMEDHT 1000
 
 
 
char szTime[9];  // mm:ss\0
char szMesg[MAX_MESG + 1] = "";
 
float humidity, celsius, fahrenheit;
 
uint8_t degC[] = { 6, 3, 3, 56, 68, 68, 68 };  // Deg C
uint8_t degF[] = { 6, 3, 3, 124, 20, 20, 4 };  // Deg F
 
uint8_t clear = 0x00;
 
uint32_t timerDHT = TIMEDHT;
 
DHT dht(DHTPIN, DHTTYPE);
 
// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE,DATA_PIN,CLK_PIN, CS_PIN, MAX_DEVICES);
 
 
 
uint8_t decToBcd(uint8_t value) {
  return ((value / 10 * 16) + (value % 10));
}
 
uint8_t bcdToDec(uint8_t value) {
  return ((value / 16 * 10) + (value % 16));
}
 
void getTemperature() {
  // Wait for a time between measurements
  if ((millis() - timerDHT) > TIMEDHT) {
    // Update the timer
    timerDHT = millis();
 
 
    humidity = dht.readHumidity();
    celsius = dht.readTemperature();
    fahrenheit = dht.readTemperature(true);
    if (isnan(humidity) || isnan(celsius) || isnan(fahrenheit)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  }
}
 
void setup(void) {
  Wire.begin();
 
  P.begin(2);
  P.setInvert(false);
 
  P.setZone(0, MAX_DEVICES - 4, MAX_DEVICES - 1);
  P.setZone(1, MAX_DEVICES - 4, MAX_DEVICES - 1);
 
  
  P.displayZoneText(0, szMesg, PA_CENTER, SPEED_TIME, 0, PA_PRINT, PA_NO_EFFECT);
 
  P.addChar('$', degC);
  P.addChar('&', degF);
 
  dht.begin();
}
 
void loop(void) {
  static uint32_t lastTime = 0;  // Memory (ms)
  static uint8_t display = 0;    // Current display mode
  static bool flasher = false;   // Seconds passing flasher
 
 
  getTemperature();
 
  P.displayAnimate();
 
  if (P.getZoneStatus(0)) {
    switch (display) {
      case 0:  // Temperature deg Celsius
        P.setPause(0, 1000);
        P.setTextEffect(0, PA_SCROLL_LEFT, PA_SCROLL_UP);
        display++;
        dtostrf(celsius, 3, 1, szMesg);
        strcat(szMesg, "$");
 
        break;
      case 1:  // Temperature deg Fahrenheit
        P.setTextEffect(0, PA_SCROLL_UP, PA_SCROLL_DOWN);
        display++;
        dtostrf(fahrenheit, 3, 1, szMesg);
        strcat(szMesg, "&");
 
        break;
      case 2:  // Humidity
        P.setTextEffect(0, PA_SCROLL_DOWN, PA_SCROLL_LEFT);
        display++;
        dtostrf(humidity, 3, 0, szMesg);
        strcat(szMesg, "%UR");
 
 
        break;
    }
 
    P.displayReset(0);  // Rest zone zero
  }
}