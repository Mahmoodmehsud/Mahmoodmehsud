//----------------------------------------------------------//MERGING BOTH BLE AND WEBSOCKET-CLIENT//--------------------------------------------------------------------//
//_______________________________________________________________________________________________________________________________________________________________________//



#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include "BLEDevice.h"


/* Specify the Service and Characterisitc UUID of Server */
static BLEUUID connectserviceUUID("0000cd80-0000-1000-8000-00805f9b34fb");
static BLEUUID appserviceUUID("cdeacd80-5235-4c07-8846-93a37ee6b86d");
static BLEUUID    charUUID("cdeacd81-5235-4c07-8846-93a37ee6b86d");
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;
/* Specify wifi AND ip addRESS */
const char* ssid = "CHIWifi2.4"; //Enter SSID
const char* password = "L3tme1n@CHI"; //Enter Password
using namespace websockets;
WebsocketsClient client;
//------------------------------------------------------------------//NOTIFYCALLBACK//-----------------------------------------------------------------------------------//
//_______________________________________________________________________________________________________________________________________________________________________//
static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,uint8_t* pData, size_t length, bool isNotify)
{
    if(pData[0]==129 && pData[1]>=1){
      Serial.print("SPO2 percentage :");
      Serial.println(pData[1]);
      Serial.print("Your Heartbeat BPM :");
      Serial.println(pData[2]);
      String sdata = "{\"SPO2 percentage " + String(pData[1]) + "\",\"Heartbeat \":\"" + String(pData[2]) + "\"}" ;
      
      client.send(sdata);
      
      }
}

//------------------------------------------------------------------//MY_CLIENT_CALLBACK//-------------------------------------------------------------------------------//
//_______________________________________________________________________________________________________________________________________________________________________//


class MyClientCallback : public BLEClientCallbacks
{
  void onConnect(BLEClient* pclient)
  {    
  }

  void onDisconnect(BLEClient* pclient)
  {
    connected = false;
   Serial.println("onDisconnect");
  }
};

//-------------------------------------------------------------//STARTING_BLE_CONNECTION//-------------------------------------------------------------------------------//
//_______________________________________________________________________________________________________________________________________________________________________//

/* Start connection to the BLE Server */
bool connectToServer()                                          //GETTING_ADDRESS//
{                                                               //Creating_Client//
  Serial.print("Forming a connection and creating client ");
  Serial.println(myDevice->getAddress().toString().c_str());
    
  BLEClient*  pClient  = BLEDevice::createClient();
  Serial.println(" - Created client");

  pClient->setClientCallbacks(new MyClientCallback());

/* Connect to the remote BLE Server */
  pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
  Serial.println(" - Connected to server");
                                                                  //GET_SEVICE_ID//
/* Obtain a reference to the service we are after in the remote BLE server */
  BLERemoteService* pRemoteService = pClient->getService(appserviceUUID);
  if (pRemoteService == nullptr)
  {
    Serial.print("Failed to find our service UUID ");
    Serial.println(appserviceUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our service");
                                                                  //GET_CHARACTERISTIC_ID//
/* Obtain a reference to the characteristic in the service of the remote BLE server */
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr)
  {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our characteristic");

  /* Read the value of the characteristic */
  /* Initial value is 'Hello, World!' */
  if(pRemoteCharacteristic->canRead())
  {
    std::string value = pRemoteCharacteristic->readValue();
    Serial.print("The characteristic value was: ");
    Serial.println(value.c_str());
  }
                                                                          //CALLING_NOTIYFYcallback//
  if(pRemoteCharacteristic->canNotify())
  {
    pRemoteCharacteristic->registerForNotify(notifyCallback);

  }

    connected = true;
    return true;
}





//-------------------------------------------------------------//BLE_ADVERTISE_DEVICE_CALLBACKS//------------------------------------------------------------------------//
//_______________________________________________________________________________________________________________________________________________________________________//

/* Scan for BLE servers and find the first one that advertises the service we are looking for. */
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
//-------------------------------------------------------------------------//MAIN//--------------------------------------------------------------------------------------//
//_______________________________________________________________________________________________________________________________________________________________________//

void setup() {
  
                                                                      //INITIALIZE_WIFI//

    Serial.begin(115200);
    // Connect to wifi
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(2000);
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
//------------------------------------------------------------------//VOID_LOOP//-------------------------------------------------------------------------------//
//_______________________________________________________________________________________________________________________________________________________________________//

void loop() {
    // let the websockets client check for incoming messages
    if(client.available()) {
        client.poll();
    }
    /* If the flag "doConnect" is true, then we have scanned for and found the desired
     BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
     connected we set the connected flag to be true. */
  if (doConnect == true)
  {
    if (connectToServer())
    {
      Serial.println("Connected to the BLE Server.");
    } 
    else
    {
      Serial.println("Failed to connect");
    }
    doConnect = false;
  }

  /* If we are connected to a peer BLE Server, update the characteristic each time we are reached
     with the current time since boot */
  if (connected)
  {
    String newValue = "Time since boot: " + String(millis()/2000);
    Serial.println("Setting new characteristic value to \"" + newValue + "\"");
    
    /* Set the characteristic's value to be the array of bytes that is actually a string */
    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
    /* You can see this value updated in the Server's Characteristic */
  }
  else if(doScan)
  {
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }
  
  delay(2000); /* Delay a second between loops */
}
