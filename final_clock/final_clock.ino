#include <MD_Parola.h>
#include "RTClib.h"
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

RTC_DS1307 RTC;
DateTime now;

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN 13
#define DATA_PIN 11
#define CS_PIN 10
#define BUTTON_PIN 3
#define buzzer 4
#define BUTTON_PIN2 5
bool flag = true;
bool humi = false;
int i = 9;
int Animation;
String RandAn;

MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

char daysOfTheWeek[7][12] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
char monthOfTheYear[12][12] = { "Jan", "Feb", "March", "April", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "NoV", "Dec" };

struct sCatalog {
  textEffect_t effect;
  const char *psz;
  uint16_t speed;
  uint16_t pause;
};

sCatalog catalog[] = {
  { PA_PRINT, "", 80, 3000 },
  { PA_SCROLL_UP, "", 80, 3000 },
  { PA_SCROLL_DOWN, "", 80, 3000 },
  { PA_SCROLL_LEFT, "", 80, 3000 },
  { PA_SCROLL_RIGHT, "", 80, 3000 },
  { PA_SPRITE, "", 80, 3000 },
  { PA_SLICE, "", 15, 3000 },
  { PA_MESH, "", 150, 3000 },
  { PA_BLINDS, "", 120, 3000 },

};



byte com = 0;
void setup() {

  Serial.begin(9600);
  P.begin();
  Serial.write(0xAA);
  Serial.write(0x37);
  delay(1000);
  Serial.write(0xAA);
  Serial.write(0x21);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  P.setInvert(false);
  P.setIntensity(10);
  dht.begin();
  Wire.begin();
  RTC.begin();
  if (!RTC.isrunning()) {
    //Serial.println("RTC is NOT running!");
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  P.displayText("Shahwar-043 And Rizwan-054", PA_CENTER, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  while (!P.displayAnimate())
    ;
}

const char *ZiuaCurenta = " ";

String Hour;
String TIMP = " ";
String ORA = " ";
String MINUT = " ";
String SECUNDA = " ";
String DATA = "";

String Day;
String Month;
String Year;
String HumSTR;
String CelSTR;
String FarSTR;

void loop() {
  String timeR = ORA + ":" + MINUT;
  // Serial.println(timeR);



  now = RTC.now();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (timeR == "20:56" && flag == true) {
    Serial.println("Alarm");
    digitalWrite(buzzer, HIGH);
    if (digitalRead(BUTTON_PIN2) == LOW) {
      digitalWrite(buzzer, LOW);
      flag = false;
    }
  }
  ORA = (now.hour());
  if (ORA.length() < 2) {
    ORA = "0" + ORA;
  }

  MINUT = (now.minute());
  if (MINUT.length() < 2) {
    MINUT = "0" + MINUT;
  }

  TIMP = ORA + ":" + MINUT;

  const char *Timp = TIMP.c_str();
  float z = (float)h / 1.0;
  HumSTR = String(z, 1);
  HumSTR = HumSTR + " %";
  const char *HumC = HumSTR.c_str();
  float q = (float)t / 1.0;
  CelSTR = String(q, 1);
  CelSTR = CelSTR + " C";
  const char *CelC = CelSTR.c_str();
  Day = now.day();
  Month = now.month();
  Year = now.year();

  const char *Ziua = Day.c_str();
  const char *Luna = Month.c_str();
  const char *Anul = Year.c_str();
  String DayOfWeek = daysOfTheWeek[now.dayOfTheWeek()];
  String MonthOfYear = monthOfTheYear[now.month() - 1];
  const char *DayOfWeekC = DayOfWeek.c_str();
  const char *MonthOfYearC = MonthOfYear.c_str();
  String Date = Day + "/" + Month + "/" + Year;
  const char *Data = Date.c_str();
  P.displayText(Timp, PA_CENTER, catalog[9].speed, catalog[9].pause, catalog[9].effect);
  P.displayAnimate();
    if (digitalRead(BUTTON_PIN2) == LOW) {
      digitalWrite(buzzer, LOW);
      flag = false;
    }
  if (Serial.available()) {
    com = Serial.read();

    if (com == 0x11) {  // alarm
      // digitalWrite(green, HIGH);
        digitalWrite(buzzer, HIGH);
      Serial.println("Alarm");
    } else if (com == 0x12) {  // Temperature
                               // digitalWrite(yellow, HIGH);
      String Celsius = String(t) + "°C";
      Animation = 6;
      P.displayText(CelC, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect);
      while (!P.displayAnimate())
        ;

      Serial.println("Temperature");
    } else if (com == 0x13) {  // Humidity
       Animation = 6; 
      // digitalWrite(red, HIGH);
      P.displayText(HumC, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect);
      while (!P.displayAnimate())
        ;
      Serial.println("Humidity");
    } else if (com == 0x14) {  // command 4 is for Servo motor
        Animation = 6; 
      // digitalWrite(red, HIGH);
      P.displayText(HumC, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect);
      while (!P.displayAnimate())
        ;
    } else if (com == 0x15) {  // month
       String Celsius = String(t) + "°C";
      Animation = 6;
      P.displayText(CelC, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect);
      while (!P.displayAnimate())
        ;
      Serial.println("Month");
    }
  }



  if (digitalRead(BUTTON_PIN) == LOW) {
    if (isnan(h) || isnan(t) || isnan(f)) {
      //Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    float z = (float)h / 1.0;
    HumSTR = String(z, 1);
    HumSTR = HumSTR + " %";
    const char *HumC = HumSTR.c_str();

    float q = (float)t / 1.0;
    CelSTR = String(q, 1);
    CelSTR = CelSTR + " C";
    const char *CelC = CelSTR.c_str();

    float w = (float)f / 1.0;
    FarSTR = String(w, 1);
    FarSTR = FarSTR + " F";
    const char *FarC = FarSTR.c_str();

    String DayOfWeek = daysOfTheWeek[now.dayOfTheWeek()];
    String MonthOfYear = monthOfTheYear[now.month() - 1];
    const char *DayOfWeekC = DayOfWeek.c_str();
    const char *MonthOfYearC = MonthOfYear.c_str();
    ORA = (now.hour());
    if (ORA.length() < 2) {
      ORA = "0" + ORA;
    }

    MINUT = (now.minute());
    if (MINUT.length() < 2) {
      MINUT = "0" + MINUT;
    }

    TIMP = ORA + ":" + MINUT;

    const char *Timp = TIMP.c_str();

    Day = now.day();
    Month = now.month();
    Year = now.year();

    const char *Ziua = Day.c_str();
    const char *Luna = Month.c_str();
    const char *Anul = Year.c_str();

    String Date = Day + "/" + Month + "/" + Year;
    const char *Data = Date.c_str();


    if (i == 1 || humi == true) {
      Animation = 1;
      Serial.print(humi);
      P.displayText(DayOfWeekC, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect);
      while (!P.displayAnimate())
        ;
      //  P.displayText(Timp, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect);
      //  while (!P.displayAnimate());
    }
    if (i == 2) {
      Animation = 2;
      P.displayText(Ziua, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect);
      while (!P.displayAnimate())
        ;
      //   P.displayText(Timp, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect);
      //  while (!P.displayAnimate());
    }
    if (i == 3) {
      Animation = 9;
      P.displayText(MonthOfYearC, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect);
      while (!P.displayAnimate())
        ;
      //   P.displayText(Timp, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect);
      //  while (!P.displayAnimate());
    }
    if (i == 4) {
      Animation = 4;
      P.displayText(Anul, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect);
      while (!P.displayAnimate())
        ;
      //   P.displayText(Timp, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect);
      //  while (!P.displayAnimate());
    }

    // if (i == 5)
    // {
    //     Animation = 5;
    //     P.displayText(Timp, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect);
    //      while (!P.displayAnimate());
    // }
    if (i == 5) {
      String Celsius = String(t) + "°C";
      Animation = 6;
      P.displayText(CelC, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect);
      while (!P.displayAnimate())
        ;
      // P.displayText(Timp, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect);
      //  while (!P.displayAnimate());
    }
    if (i == 6) {
      Animation = 7;
      P.displayText(FarC, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect);
      while (!P.displayAnimate())
        ;
      //   P.displayText(Timp, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect);
      //  while (!P.displayAnimate());
    }
    if (i == 7) {
      Animation = 8;
      P.displayText(HumC, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect);
      while (!P.displayAnimate())
        ;
      //   P.displayText(Timp, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect);
      //  while (!P.displayAnimate());
    }

    Serial.println(Animation);
    i = i + 1;
    if (i > 7) {
      i = 1;
    };
  }
}
