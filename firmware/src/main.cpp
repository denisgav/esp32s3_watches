#include <Arduino.h>
#include <Wire.h>

#include "board_defines.h"

//--------------------------------
// -        SD CARD
//--------------------------------
#include "FS.h"
#include "SD.h"
#include "SPI.h"

void SDCARD_Init();
//--------------------------------

//--------------------------------
// -        BME 280
//--------------------------------
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C
void BME280_Init();
void BME280_print_values();
//--------------------------------

//--------------------------------
// -        RTC 3231
//--------------------------------
// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"
RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
void RTC3231_Init();
void RTC3231_print_values();
//--------------------------------

//--------------------------------
// -        SSD 1306
//--------------------------------
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(SSD1306_SCREEN_WIDTH, SSD1306_SCREEN_HEIGHT, &Wire, SSD1306_OLED_RESET);
void SSD1306_Init();
//--------------------------------

//--------------------------------
// -        FastLED
//--------------------------------
#include <FastLED.h>

// Define the array of leds
CRGB clock_leds[WS2812_CLOCK_NUM_LEDS];
//--------------------------------

//--------------------------------
// -        Radar
//--------------------------------
#include <ld2410.h>

ld2410 radar;
void LD2410_Init();
//--------------------------------


void setup()
{
  Serial.begin(115200);
  
  SDCARD_Init();

  FastLED.addLeds<WS2812, WS2812_CLOCK_LED_PIN, RGB>(clock_leds, WS2812_CLOCK_NUM_LEDS);

  BME280_Init();
  RTC3231_Init();
  SSD1306_Init();
  LD2410_Init();
  
}

uint32_t lastReading = 0;
void loop()
{
  radar.read();
  if(millis() - lastReading > 1000)  //Report every 1000ms
  {
    lastReading = millis();
    BME280_print_values();
    RTC3231_print_values();

    int LDR_val = analogRead(LDR_IN);
    Serial.print(F("LDR: "));
    Serial.print(LDR_val);
    Serial.println();

    if(radar.isConnected()){
      
      if(radar.presenceDetected())
      {
        if(radar.stationaryTargetDetected())
        {
          Serial.print(F("Stationary target: "));
          Serial.print(radar.stationaryTargetDistance());
          Serial.print(F(" cm energy:"));
          Serial.print(radar.stationaryTargetEnergy());
          Serial.println();
        }
        if(radar.movingTargetDetected())
        {
          Serial.print(F("Moving target: "));
          Serial.print(radar.movingTargetDistance());
          Serial.print(F(" cm energy:"));
          Serial.print(radar.movingTargetEnergy());
          Serial.println();
        }
      }
      else
      {
        Serial.println(F("No target"));
      }
    }
    else
    {
      Serial.println(F("Radar not connected"));
    }
  }

  delay(1000);


  // onboard_leds[0] = CRGB::Red;
  // FastLED.show();

  // delay(1000);

  // onboard_leds[0] = CRGB::Black;
  // FastLED.show();
}

void BME280_Init()
{
  unsigned status;

  // default settings
  status = bme.begin(I2C_BME280_ADDRESS, &Wire);
  // You can also pass in a Wire library object like &Wire2
  // status = bme.begin(0x76, &Wire2)
  if (!status)
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x");
    Serial.println(bme.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1)
      delay(10);
  }

  Serial.println("-- BME 280 is ready --");
}

void BME280_print_values()
{
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" °C");

  Serial.print("Pressure = ");

  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  // Serial.print("Approx. Altitude = ");
  // Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  // Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}

void RTC3231_Init()
{
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1)
      delay(10);
  }

  if (rtc.lostPower())
  {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

void RTC3231_print_values()
{
  DateTime now = rtc.now();

  // Serial.print(now.year(), DEC);
  // Serial.print('/');
  // Serial.print(now.month(), DEC);
  // Serial.print('/');
  // Serial.print(now.day(), DEC);
  // Serial.print(" (");
  // Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  // Serial.print(") ");
  // Serial.print(now.hour(), DEC);
  // Serial.print(':');
  // Serial.print(now.minute(), DEC);
  // Serial.print(':');
  // Serial.print(now.second(), DEC);
  // Serial.println();

  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner

  display.print(now.year(), DEC);
  display.print('/');
  display.print(now.month(), DEC);
  display.print('/');
  display.println(now.day(), DEC);
  display.println(daysOfTheWeek[now.dayOfTheWeek()]);
  display.print(now.hour(), DEC);
  display.print(':');
  display.print(now.minute(), DEC);
  display.print(':');
  display.println(now.second(), DEC);

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
}

void SSD1306_Init(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.print("Hello world!");

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
}

void LD2410_Init()
{
  RADAR_SERIAL.begin(256000, SERIAL_8N1, RADAR_RX_PIN, RADAR_TX_PIN); //UART for monitoring the radar
  delay(500);

  Serial.print(F("\nConnect LD2410 radar TX to GPIO:"));
  Serial.println(RADAR_RX_PIN);
  Serial.print(F("Connect LD2410 radar RX to GPIO:"));
  Serial.println(RADAR_TX_PIN);
  Serial.print(F("LD2410 radar sensor initialising: "));
  
  if(radar.begin(RADAR_SERIAL))
  {
    Serial.println(F("OK"));
    Serial.print(F("LD2410 firmware version: "));
    Serial.print(radar.firmware_major_version);
    Serial.print('.');
    Serial.print(radar.firmware_minor_version);
    Serial.print('.');
    Serial.println(radar.firmware_bugfix_version, HEX);
  }
  else
  {
    Serial.println(F("not connected"));
  }
 
}


void SDCARD_Init(){
  if(!SD.begin(SD_CARD_CS)){
    while(true){
      Serial.println("Card Mount Failed");
      delay (1000);
    }
    return;
  }
}