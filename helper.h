#include<ArduinoWebsockets.h>
#include <BLEDevice.h>
#include<WiFi.h>
using namespace websockets;
WebsocketsClient* client;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;
static BLEUUID connectserviceUUID("0000cd80-0000-1000-8000-00805f9b34fb");
static BLEUUID appserviceUUID("cdeacd80-5235-4c07-8846-93a37ee6b86d");
static BLEUUID    charUUID("cdeacd81-5235-4c07-8846-93a37ee6b86d");

 boolean doScan = false;
 boolean doConnect=false;
const char* ssid = "CHIWifi2.4"; //Enter SSID
const char* password = "L3tme1n@CHI"; //Enter Password
 boolean pconnected=false;
