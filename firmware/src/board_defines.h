#pragma once

// SD card connection
#define SDCARD_D0 47
#define SDCARD_D1 21
#define SDCARD_D2 42
#define SDCARD_D3 41
#define SDCARD_CLK 39
#define SDCARD_CMD 40

// // Buttons
#define BUTTON_1 6
#define BUTTON_2 7
#define BUTTON_3 15
#define BUTTON_4 16

// LEDS
#define LED_1 1
#define LED_2 2

// LDR
#define LDR_IN 4

//I2C
#define I2C_SDA 8
#define I2C_SCL 9

#define I2C_BME280_ADDRESS 0x76

// LED strip
// How many leds in your strip?
// #define WS2812_ONBOARD_NUM_LEDS 1
// #define WS2812_ONBOARD_LED_PIN 48
#define WS2812_CLOCK_NUM_LEDS 60
#define WS2812_CLOCK_LED_PIN 38

// SSD 1306
#define SSD1306_SCREEN_WIDTH   128 // OLED display width, in pixels
#define SSD1306_SCREEN_HEIGHT  64 // OLED display height, in pixels
#define SSD1306_OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SSD1306_SCREEN_ADDRESS 0x3c ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// Radar
#define MONITOR_SERIAL Serial
#define RADAR_SERIAL Serial1
#define RADAR_RX_PIN 18
#define RADAR_TX_PIN 17