/*
  TITLE
  By V205
*/

#include <WiFi.h>
#include <Arduino_GFX_Library.h>




/*
pin stuff, can ignore
GPIO  16 Pin 21 MISO
GPIO 17 pin 22 LITE
GPIO18 pin 24 SCK
GPIO19 pin 25 MOSI
GPIO22 pin 29 D/C
chip select pin, it's gp21/pin27
RESET = gP26
USE THE GP ONE

Arduino_HWSPI(int8_t dc, int8_t cs = GFX_NOT_DEFINED, SPIClass *spi = &SPI, bool is_shared_interface = true); // Constructor thingy
TODO: get
*/

/*

Arduino_ST7735::Arduino_ST7735(
    Arduino_DataBus *bus, int8_t rst, uint8_t r,
    bool ips, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2,
*/


//Arduino_DataBus *bus = new Arduino_HWSPI(16 /* DC */, 5 /* CS */);

Arduino_DataBus *bus = new Arduino_HWSPI(22, 21);
//Arduino_GFX *gfx = new Arduino_ILI9341(bus, 17 /* RST */);
Arduino_GFX *gfx = new Arduino_ST7735(bus, 26, 3);

// Define pin numbers for the buttons
const int wButtonPin = 5;
const int aButtonPin = 6;
const int sButtonPin = 7;
const int dButtonPin = 8;
const int iButtonPin = 12;
const int jButtonPin = 13;
const int kButtonPin = 14;
const int lButtonPin = 15;

const char *macToString(uint8_t mac[6]) {
  static char s[20];
  sprintf(s, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return s;
}


const char *encToString(uint8_t enc) {
  switch (enc) {
    case ENC_TYPE_NONE: return "NONE";
    case ENC_TYPE_TKIP: return "WPA";
    case ENC_TYPE_CCMP: return "WPA2";
    case ENC_TYPE_AUTO: return "AUTO";
  }
  return "UNKN";
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // begin serial comunication.
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
  Serial.println(F("STARTING " __FILE__ " from " __DATE__ __TIME__));
  pinMode(17, OUTPUT);
  digitalWrite(17, HIGH);
  gfx->begin();
  gfx->fillScreen(BLACK);
  gfx->setCursor(0, 0);
  gfx->setTextColor(GREEN);
  gfx->setTextSize(1);
  pinMode(wButtonPin, INPUT_PULLUP);
  pinMode(aButtonPin, INPUT_PULLUP);
  pinMode(sButtonPin, INPUT_PULLUP);
  pinMode(dButtonPin, INPUT_PULLUP);
  pinMode(iButtonPin, INPUT_PULLUP);
  pinMode(jButtonPin, INPUT_PULLUP);
  pinMode(kButtonPin, INPUT_PULLUP);
  pinMode(lButtonPin, INPUT_PULLUP);
}






void loop() {

  //delay(5000);
  gfx->fillScreen(BLACK);
  gfx->setCursor(0, 0);
  Serial.printf("Beginning scan at %lu\n", millis());

  auto cnt = WiFi.scanNetworks();
  if (!cnt) {
    Serial.printf("No networks found\n");

  } else {
    Serial.printf("Found %d networks\n\n", cnt);
    Serial.printf("%32s %5s %17s %2s %4s\n", "SSID", "ENC", "BSSID        ", "CH", "RSSI");
    for (auto i = 0; i < cnt; i++) {
      uint8_t bssid[6];
      WiFi.BSSID(i, bssid);
      Serial.printf("%32s %5s %17s %2d %4ld\n", WiFi.SSID(i), encToString(WiFi.encryptionType(i)), macToString(bssid), WiFi.channel(i), WiFi.RSSI(i));
      char buffer[100];  // Allocate enough space for the formatted string


      /*
      //This one has the AP MAC
      sprintf(buffer, "%32s %5s %17s %2d %4ld\n",
              WiFi.SSID(i),
              encToString(WiFi.encryptionType(i)),
              macToString(bssid),
              WiFi.channel(i),
              WiFi.RSSI(i));
      */
      

      /*
      sprintf(buffer, "%32s %5s %2d %4ld\n",
              WiFi.SSID(i),
              encToString(WiFi.encryptionType(i)),
              //macToString(bssid),
              WiFi.channel(i),
              WiFi.RSSI(i));

      gfx->print(buffer);
      */
      gfx->print(WiFi.SSID(i));
      gfx->print(" ");
      gfx->print(encToString(WiFi.encryptionType(i)));
       gfx->print(" ");
      gfx->print(WiFi.channel(i));
       gfx->print(" ");
      gfx->println(WiFi.RSSI(i));

      Serial.println(buffer);
    }
    Serial.printf("\n--- Sleeping, press S to rescan ---\n\n\n");
    //delay(5000);

  }
  while (digitalRead(sButtonPin) == HIGH) {

  }
}
