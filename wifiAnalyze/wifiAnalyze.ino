#include <Arduino_GFX_Library.h>
#include <WiFi.h>


/*
 *
 * Sprig Wifi Analyzer.
 * By V205, kind of works

 * Revised from ESP8266 WiFi Analyzer to work with a pico w on a Hack Club Sprig.
 * Original project by github user moononournation.
 */




/*
// Uncomment only the TFT model you are using
#define ILI9341
//#define ST7735_18GREENTAB
//#define ST7735_18REDTAB
//#define ST7735_18GBLACKTAB
//#define ST7735_144GREENTAB

//POWER SAVING SETTING
#define SCAN_COUNT_SLEEP 5
// Uncomment this option if using PNP transistor control LCD power
#define PNP_PWR_TRANSISTOR

#if defined(PNP_PWR_TRANSISTOR)
#define LCD_PWR_PIN 4 // D2
#else
#define LCD_PWR_PIN 4 // D2
#define LED_PWR_PIN 2 // D4
#endif

#include "ESP8266WiFi.h"

#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
// Hardware-specific library
#if defined(ST7735_18GREENTAB) || defined(ST7735_18REDTAB) || defined(ST7735_18GBLACKTAB) || defined(ST7735_144GREENTAB)
#include <Adafruit_ST7735.h>
#elif defined(ILI9341)
#include <Adafruit_ILI9341.h>
#endif

#define DC     5 // D1
#define CS     15 // D8
*/


//#if defined(ST7735_18GREENTAB) || defined(ST7735_18REDTAB) || defined(ST7735_18GBLACKTAB) || defined(ST7735_144GREENTAB)
//Adafruit_ST7735 tft = Adafruit_ST7735(CS,  DC, 0 /* RST */);

/*
#elif defined(ILI9341)
Adafruit_ILI9341 tft = Adafruit_ILI9341(CS, DC);
#endif

// Graph constant
#if defined(ST7735_18GREENTAB) || defined(ST7735_18REDTAB) || defined(ST7735_18GBLACKTAB)
#define WIDTH 160
#define HEIGHT 128
#define BANNER_HEIGHT 8
#elif defined(ST7735_144GREENTAB)
#define WIDTH 128
#define HEIGHT 128
#define BANNER_HEIGHT 8
#elif defined(ILI9341)
#define WIDTH 320
#define HEIGHT 240
#define BANNER_HEIGHT 16
#endif
#define GRAPH_BASELINE (HEIGHT - 18)
#define GRAPH_HEIGHT (HEIGHT - 52)
#define CHANNEL_WIDTH (WIDTH / 16)

// RSSI RANGE
#define RSSI_CEILING -40
#define RSSI_FLOOR -100
#define NEAR_CHANNEL_RSSI_ALLOW -70
*/


uint8_t scan_count = 0;


Arduino_DataBus *bus = new Arduino_HWSPI(22, 21);
//Arduino_GFX *gfx = new Arduino_ILI9341(bus, 17 /* RST */);
Arduino_GFX *gfx = new Arduino_ST7735(bus, 26, 3);


uint16_t channel_color[] = {
  RED, ORANGE, YELLOW, GREEN, CYAN, MAGENTA,
  RED, ORANGE, YELLOW, GREEN, CYAN, MAGENTA,
  RED, ORANGE
};
// RSSI RANGE
#define RSSI_CEILING -40
#define RSSI_FLOOR -100
#define NEAR_CHANNEL_RSSI_ALLOW -70

#define WIDTH 160
#define HEIGHT 128
#define BANNER_HEIGHT 8

#define GRAPH_BASELINE (HEIGHT - 18)
#define GRAPH_HEIGHT (HEIGHT - 52)
#define CHANNEL_WIDTH (WIDTH / 16)

void setup() {

  pinMode(17, OUTPUT);
  digitalWrite(17, HIGH);
  gfx->begin();
  gfx->fillScreen(BLACK);
  gfx->setCursor(0, 0);
  gfx->setTextColor(GREEN);
  gfx->setTextSize(1);
  Serial.begin(115200);
  // init banner
  /*
#ifdef ILI9341
  gfx->setTextSize(2);
#endif
  gfx->fillScreen(BLUE);
  gfx->setTextColor(WHITE, RED);
  gfx->setCursor(0, 0);
#if defined(ST7735_144GREENTAB)
  gfx->print(" ESP");
  gfx->setTextColor(WHITE, ORANGE);
  gfx->print("8266");
  gfx->setTextColor(WHITE, GREEN);
  gfx->print("WiFi");
  gfx->setTextColor(WHITE, BLUE);
  gfx->print("Analyzer");
#else
  gfx->print(" ESP ");
  gfx->setTextColor(WHITE, ORANGE);
  gfx->print(" 8266 ");
  gfx->setTextColor(WHITE, GREEN);
  gfx->print(" WiFi ");
  gfx->setTextColor(WHITE, BLUE);
  gfx->print(" Analyzer");
#endif
*/
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // rest for WiFi routine?
  delay(100);
}

void loop() {
  uint8_t ap_count[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  int32_t max_rssi[] = { -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100 };

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();

  // clear old graph
  gfx->fillRect(0, BANNER_HEIGHT, 320, 224, BLACK);
  //gfx->fillScreen(BLACK);
  gfx->setTextSize(1);

  if (n == 0) {
    //gfx->setTextColor(BLACK);
    //gfx->setCursor(0, BANNER_HEIGHT);
    //gfx->println("no networks found");
    gfx->setTextColor(BLACK);
    gfx->setCursor(0, 0);
    gfx->println("no networks found");

  } else {
    // plot found WiFi info
    for (int i = 0; i < n; i++) {
      int32_t channel = WiFi.channel(i);
      int32_t rssi = WiFi.RSSI(i);
      uint16_t color = channel_color[channel - 1];
      int height = constrain(map(rssi, RSSI_FLOOR, RSSI_CEILING, 1, GRAPH_HEIGHT), 1, GRAPH_HEIGHT);

      // channel stat
      ap_count[channel - 1]++;
      if (rssi > max_rssi[channel - 1]) {
        max_rssi[channel - 1] = rssi;
      }

      gfx->drawLine(channel * CHANNEL_WIDTH, GRAPH_BASELINE - height, (channel - 1) * CHANNEL_WIDTH, GRAPH_BASELINE + 1, color);
      gfx->drawLine(channel * CHANNEL_WIDTH, GRAPH_BASELINE - height, (channel + 1) * CHANNEL_WIDTH, GRAPH_BASELINE + 1, color);

      // Print SSID, signal strengh and if not encrypted
      gfx->setTextColor(color);
      gfx->setCursor((channel - 1) * CHANNEL_WIDTH, GRAPH_BASELINE - 10 - height);
      gfx->print(WiFi.SSID(i));
      gfx->print('(');
      gfx->print(rssi);
      gfx->print(')');
      if (WiFi.encryptionType(i) == ENC_TYPE_NONE) {
        gfx->print('*');
      }




      Serial.print(WiFi.SSID(i));
      Serial.print('(');
      Serial.print(rssi);
      Serial.print(')');
      if (WiFi.encryptionType(i) == ENC_TYPE_NONE) {
        Serial.print('*');
      }
      Serial.println(' ');

      // rest for WiFi routine?
      delay(10);
    }
  }

  // print WiFi stat
  gfx->setTextColor(WHITE);
  gfx->setCursor(0, BANNER_HEIGHT);
  gfx->print(n);
  gfx->print(" networks found, suggested channels: ");
  bool listed_first_channel = false;
  for (int i = 1; i <= 11; i++) {                                                     // channels 12-14 may not available
    if ((i == 1) || (max_rssi[i - 2] < NEAR_CHANNEL_RSSI_ALLOW)) {                    // check previous channel signal strengh
      if ((i == sizeof(channel_color)) || (max_rssi[i] < NEAR_CHANNEL_RSSI_ALLOW)) {  // check next channel signal strengh
        if (ap_count[i - 1] == 0) {                                                   // check no AP exists in same channel
          if (!listed_first_channel) {
            listed_first_channel = true;
          } else {
            gfx->print(", ");
          }
          gfx->print(i);
        }
      }
    }
  }

  // draw graph base axle
  gfx->drawFastHLine(0, GRAPH_BASELINE, 320, WHITE);
  for (int i = 1; i <= 14; i++) {
    gfx->setTextColor(channel_color[i - 1]);
    gfx->setCursor((i * CHANNEL_WIDTH) - ((i < 10) ? 3 : 6), GRAPH_BASELINE + 2);
    gfx->print(i);
    if (ap_count[i - 1] > 0) {
      gfx->setCursor((i * CHANNEL_WIDTH) - ((ap_count[i - 1] < 10) ? 9 : 12), GRAPH_BASELINE + 11);
      gfx->print('(');
      gfx->print(ap_count[i - 1]);
      gfx->print(')');
    }
  }

  // Wait a bit before scanning again
  delay(5000);

  //POWER SAVING

  /*
  if (++scan_count >= SCAN_COUNT_SLEEP) {
#if defined(PNP_PWR_TRANSISTOR)
    pinMode(LCD_PWR_PIN, INPUT);  // disable pin
#else
    pinMode(LCD_PWR_PIN, INPUT);  // disable pin
    pinMode(LED_PWR_PIN, INPUT);  // disable pin
#endif
    ESP.deepSleep(0);
  }
  */
}