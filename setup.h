#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include "BLEDevice.h"
using namespace websockets;
WebsocketsClient client;
const char* ssid = "CHIWifi2.4"; //Enter SSID
const char* password = "L3tme1n@CHI"; //Enter Password
static BLEUUID connectserviceUUID("0000cd80-0000-1000-8000-00805f9b34fb");
static BLEAdvertisedDevice* myDevice;
static boolean doConnect = false;

static boolean doScan = false;
void setup() {
  
                                                                      //INITIALIZE_WIFI//

    Serial.begin(115200);
    // Connect to wifi
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Check if connected to wifi
    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("No Wifi!");
        return;
    }

    Serial.println("Connected to Wifi, Connecting to server.");
    // try to connect to Websockets server
    bool connected = client.connect("ws://10.8.0.57:8000");
    if(connected) {
      
        Serial.println("Connected!");
    } else {
        Serial.println("Not Connected!");
    }



                                                                //INITIALIZE_BLE//


    Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("ESP32-BLE-Client");

  /* Retrieve a Scanner and set the callback we want to use to be informed when we
     have detected a new device.  Specify that we want active scanning and start the
     scan to run for 5 seconds. */
  BLEScan* pBLEScan = BLEDevice::getScan();


  
  class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
{
 /* Called for each advertising BLE server. */
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    Serial.print("BLE Device found");
    Serial.println(advertisedDevice.toString().c_str());

    /* We have found a device, let us now see if it contains the service we are looking for. */
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(connectserviceUUID))
    {
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
       doConnect = true;
       doScan = true;

    }
  }
};



  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(10, false);

                                                      //CALL_BACK_WHEN_MESSAGES_ARE_RECIEVED//

    client.onMessage([&](WebsocketsMessage message){
        Serial.print("Got Message: ");
        Serial.println(message.data());
    });
}
