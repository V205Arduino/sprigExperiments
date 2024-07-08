/*
  TITLE
  By V205
*/

#include <WiFi.h>

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200); // begin serial comunication.
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
  Serial.println(F("STARTING " __FILE__ " from " __DATE__ __TIME__));
  WiFi.scanNetworks();
  delay(10000);

  int networkNumber = WiFi.scanComplete();

  for (int i = 0; i < networkNumber; i++) {
    Serial.print("Network number: ");
    Serial.println(networkNumber);
    Serial.println(WiFi.SSID(i));
    //Serial.println(WiFi.BSSID(i));
    Serial.println(WiFi.RSSI(i));
    Serial.print(WiFi.encryptionType(i));


  }
}






void loop() {
  Serial.println("scan start");


  int networkNumber = WiFi.scanNetworks();
  Serial.println("scan done");
  if (networkNumber == 0) {
      Serial.println("no networks found");
  } else {
    Serial.print(networkNumber);
    Serial.println(" networks found");
    for (int i = 0; i < networkNumber; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println(printEncryptionType(i));

      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);
}


void printEncryptionType(int encryptionType) {
  switch (encryptionType) {
    case WIFI_AUTH_OPEN:
      Serial.println("Open");
      break;
    case WIFI_AUTH_WEP:
      Serial.println("WEP");
      break;
    case WIFI_AUTH_WPA_PSK:
      Serial.println("WPA PSK");
      break;
    case WIFI_AUTH_WPA2_PSK:
      Serial.println("WPA2 PSK");
      break;
    case WIFI_AUTH_WPA_WPA2_PSK:
      Serial.println("WPA/WPA2 PSK");
      break;
    case WIFI_AUTH_WPA2_ENTERPRISE:
      Serial.println("WPA2 Enterprise");
      break;
    default:
      Serial.println("Unknown");
  }
}

