#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

int scanTime = 3; // In seconds
String msg;
float distance1;
float rssi1;
int distance2;
float distance3;
int rssi2;
bool device_found;
BLEScan* pBLEScan;
const int rssiAtOneMeter = -72; // RSSI value at one meter (calibrated based on your setup)
const int signalAttenuationFactor = 2; // Empirical attenuation factor

// Function prototype
int mapRssiToDistance(int rssi);

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (strcmp(advertisedDevice.getAddress().toString().c_str(), "74:da:ea:92:d5:7d") == 0) {
      msg = advertisedDevice.getAddress().toString().c_str();
      Serial.println("Msg=");
      Serial.println(msg);
      rssi1 = advertisedDevice.getRSSI();
      Serial.println(rssi1);
      distance1 = mapRssiToDistance(rssi1);
      Serial.print("Distance: ");
      Serial.println(distance1);
    }
     else if (strcmp(advertisedDevice.getAddress().toString().c_str(), "d0:b5:c2:9b:a3:fe") == 0) {
      msg = advertisedDevice.getAddress().toString().c_str();
      Serial.println("Msg=");
      Serial.println(msg);
      rssi2 = advertisedDevice.getRSSI();
      Serial.println(rssi2);
      distance2 = mapRssiToDistance(rssi2);
      Serial.print("Distance: ");
      Serial.println(distance2);
    }
    // ... (similar blocks for other devices)
    else {
      device_found = false;
      msg = "NO";
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
          display.clearDisplay();
          display.setTextSize(2);
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(0, 10);
          // Display static text
          display.println("Indoor");
       
          display.println("GPS");
          display.display(); 
          delay(3000);
          display.clearDisplay();
          display.setCursor(0, 10);
          display.println("Scaning");
          display.display(); 
          delay(2000);

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); // create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); // active scan uses more power, but gets results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results from BLEScan buffer to release memory
  
  
  // Print on OLED
  if (strcmp(msg.c_str(), "74:da:ea:92:d5:7d") == 0) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Your are near Lounge");
  // Display static text
 // display.println();
 // display.println("BLE ID:");
  //display.println(msg);
  display.println();
  display.print("Distance: ");
  distance3 = pow(10, ((rssiAtOneMeter - rssi1) / (10 * signalAttenuationFactor)));
  display.print(distance3);
  display.println("m");
//  }
  
  display.println();
  display.print("RSSI value:");
  display.print(rssi1);
  display.display();
  delay(100);
  }
  

}


int mapRssiToDistance(int rssi) {
  // Calculate distance using a simple linear mapping
  // This is a basic example, and you might need to adjust the values based on your setup
  int distance = pow(10, ((float)(rssi - rssiAtOneMeter) / (10 * signalAttenuationFactor)));
  return distance;
}
