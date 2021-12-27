
void setup() {
                                                                      //INITIALIZE_WIFI//
    Serial.begin(115200);
     WiFi.begin(ssid, password);
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }
    // Check if connected to wifi
    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("No Wifi!")//return
        ;
    }
    Serial.println("Connected to Wifi, Connecting to server.");
    WebsocketsClient client;
     bool connected = client.connect("ws://10.8.0.59:8000");
    if(connected) {
      
        Serial.println("Connected!");
    } else {
        Serial.println("Not Connected!");
    }
                                                                //INITIALIZE_BLE//


  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("ESP32-BLE-Client");
  BLEScan* pBLEScan = BLEDevice::getScan();


  
  class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
{
 /* Called for each advertising BLE server. */
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    Serial.print("BLE Device found");
    Serial.println(advertisedDevice.toString().c_str());
    Serial.println("CODE IS GOOD 1");
    /* We have found a device, let us now see if it contains the service we are looking for. */
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(connectserviceUUID))
    {Serial.println("CODE IS GOOD 2");
      BLEDevice::getScan()->stop();
      Serial.println("CODE IS GOOD 3");
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
       doConnect = true;
       doScan=true;
Serial.println("CODE IS GOOD 4");
    }
  }
};



  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(10, false);

                                                      //CALL_BACK_WHEN_MESSAGES_ARE_RECIEVED//
Serial.print("CODE IS GOOD 5");
    client.onMessage([&](WebsocketsMessage message){
        Serial.print("Got Message: ");
        Serial.println(message.data());
    });


 

}
