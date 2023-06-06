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
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

int i = 9;
int Animation;
String RandAn;

MD_Parola P = MD_Parola(HARDWARE_TYPE,DATA_PIN,CLK_PIN, CS_PIN, MAX_DEVICES);

char daysOfTheWeek[7][12] = {"Dum", "Luni", "Marti", "Mierc", "Joi", "Vineri", "Sambat"};
char monthOfTheYear[12][12] = {"Ian", "Feb", "Mart", "April", "Mai", "Iun", "Iul", "Aug", "Sept", "Oct", "Noi", "Dec"};

struct sCatalog
{
  textEffect_t  effect;   
  const char *  psz;      
  uint16_t      speed;    
  uint16_t      pause;    
};

sCatalog catalog[] =
{
  { PA_PRINT,"",80,3000 },
  { PA_SCROLL_UP,"",80,3000 },
  { PA_SCROLL_DOWN,"",80,3000 },
  { PA_SCROLL_LEFT,"",80,3000 },
  { PA_SCROLL_RIGHT,"",80,3000 },
  { PA_SPRITE,"",80,3000  },
  { PA_SLICE,"",15,3000  },
  { PA_MESH,"",150,3000  },
  { PA_FADE,"",250,3000  },
  { PA_DISSOLVE,"",500,3000  },
  { PA_BLINDS,"",120,3000  },
  { PA_RANDOM,"",50,3000  },
  { PA_WIPE,"",80,3000  },
  { PA_WIPE_CURSOR,"",80,3000  },
  { PA_SCAN_HORIZ,"",80,3000  },
  { PA_SCAN_HORIZX,"",80,3000  },
  { PA_SCAN_VERT,"",80,3000  },
  { PA_SCAN_VERTX,"",80,3000  },
  { PA_OPENING,"",80,3000  },
  { PA_OPENING_CURSOR,"",80,3000  },
  { PA_CLOSING,"",80,3000  },
  { PA_CLOSING_CURSOR,"",80,3000  },
  { PA_SCROLL_UP_LEFT,"",80,3000  },
  { PA_SCROLL_UP_RIGHT,"",80,3000   },
  { PA_SCROLL_DOWN_LEFT,"",80,3000  },
  { PA_SCROLL_DOWN_RIGHT,"",80,3000  },
  { PA_GROW_UP,"",80,3000  },
  { PA_GROW_DOWN,"",80,3000  },
};


// Sprite Definitions
const uint8_t F_PMAN1 = 6;
const uint8_t W_PMAN1 = 8;
static const uint8_t PROGMEM pacman1[F_PMAN1 * W_PMAN1] =  // gobbling pacman animation
{
  0x00, 0x81, 0xc3, 0xe7, 0xff, 0x7e, 0x7e, 0x3c,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c,
};

const uint8_t F_PMAN2 = 6;
const uint8_t W_PMAN2 = 18;
static const uint8_t PROGMEM pacman2[F_PMAN2 * W_PMAN2] =  // ghost pursued by a pacman
{
  0x00, 0x81, 0xc3, 0xe7, 0xff, 0x7e, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x73, 0xfb, 0x7f, 0xf3, 0x7b, 0xfe,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x73, 0xfb, 0x7f, 0xf3, 0x7b, 0xfe,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x73, 0xfb, 0x7f, 0xf3, 0x7b, 0xfe,
};


void setup()
{
  //Serial.begin(9600);
  P.begin();
  P.setInvert(false); 
  P.setIntensity(10);
  dht.begin();
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning())
      {
        //Serial.println("RTC is NOT running!");
        RTC.adjust(DateTime(__DATE__, __TIME__));
      }
#if ENA_SPRITE
  P.setSpriteData(pacman1, W_PMAN1, F_PMAN1, pacman2, W_PMAN2, F_PMAN2);
#endif
        P.displayText("Digital clock ARDUINO - Andrei & Levente Daradici" , PA_CENTER, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT); 
         while (!P.displayAnimate());
}

const char *ZiuaCurenta = " ";

String Hour;
String TIMP=" ";
String ORA=" ";
String MINUT=" ";
String SECUNDA=" ";
String DATA="";

String Day;
String Month;
String Year;
String HumSTR;
String CelSTR;
String FarSTR;

void loop()
{
  now = RTC.now();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
 
  if (isnan(h) || isnan(t) || isnan(f)) {
    //Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float z = (float)h/1.0;
  HumSTR = String(z,1);
  HumSTR = HumSTR + " %";
  const char *HumC = HumSTR.c_str(); 

  float q = (float)t/1.0;
  CelSTR = String(q,1);
  CelSTR = CelSTR + " C";
  const char *CelC = CelSTR.c_str(); 

  float w = (float)f/1.0;
  FarSTR = String(w,1);
  FarSTR = FarSTR + " F";
  const char *FarC = FarSTR.c_str(); 

  String DayOfWeek = daysOfTheWeek[now.dayOfTheWeek()];
  String MonthOfYear = monthOfTheYear[now.month() - 1];
  const char *DayOfWeekC = DayOfWeek.c_str();
  const char *MonthOfYearC = MonthOfYear.c_str();

  ORA = (now.hour());
  if (ORA.length() < 2)
         {
           ORA = "0"+ ORA;
         }
 
  MINUT = (now.minute());
  if (MINUT.length() < 2)
         {
           MINUT = "0"+ MINUT;
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

  
   if (i == 1)
    {
        Animation = random(1, 29);
        //  P.setIntensity(10);
        P.displayText(DayOfWeekC, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect); 
         while (!P.displayAnimate());
    }
  if (i == 2)
    {
        Animation = random(1, 29);
        P.displayText(Ziua, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect); 
         while (!P.displayAnimate());
    }
    if (i == 3)
    {
        Animation = random(1, 29);
        P.displayText(MonthOfYearC, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect); 
         while (!P.displayAnimate());
    }
    if (i == 4)
    {
        Animation = random(1, 29);
        P.displayText(Anul, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect); 
         while (!P.displayAnimate());
    }
 
    if (i == 5)
    {
        Animation = random(1, 29);
        P.displayText(Timp, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect); 
         while (!P.displayAnimate());
    }
    if (i == 6)
    {
        String Celsius = String(t) + "°C";
        Animation = random(1, 29);
        P.displayText(CelC, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect); 
         while (!P.displayAnimate());
    }
    if (i == 7)
    {
        Animation = random(1, 29);
        P.displayText(FarC, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect); 
         while (!P.displayAnimate());
    }
    if (i ==8)
    {
        Animation = random(1, 29);
        P.displayText(HumC, PA_CENTER, catalog[Animation].speed, catalog[Animation].pause, catalog[Animation].effect, catalog[Animation].effect); 
         while (!P.displayAnimate());
    }
    //Serial.println(Animation);
       i= i+1; 
       if (i > 8) 
       {
         i=1;
       }; 

}
