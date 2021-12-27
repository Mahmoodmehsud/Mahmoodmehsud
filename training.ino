//----------------------------------------------------------//MERGING BOTH BLE AND WEBSOCKET-CLIENT//--------------------------------------------------------------------//
//_______________________________________________________________________________________________________________________________________________________________________//
#include "helper.h"
#include"loop.h"
#include"setup.h"


//------------------------------------------------------------------//NOTIFYCALLBACK//-----------------------------------------------------------------------------------//
//_______________________________________________________________________________________________________________________________________________________________________//
static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,uint8_t* pData, size_t length, bool isNotify)
{    
    if(pData[0]==129 && pData[1]>=1){
      Serial.print("SPO2 percentage :");
      Serial.println(pData[1]);
      Serial.print("Your Heartbeat BPM :");
      Serial.println(pData[2]);  
            client->send("hello+++++++++");
           
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
//    connected = false;
   Serial.println("onDisconnect");
  }
};

//-------------------------------------------------------------//STARTING_BLE_CONNECTION//-------------------------------------------------------------------------------//
//_______________________________________________________________________________________________________________________________________________________________________//
//connect to server wala
   bool connectToServer()                                          //GETTING_ADDRESS//
{
  //Creating_Client//
  Serial.print("Forming a connection and creating client ");
  Serial.println(myDevice->getAddress().toString().c_str());
    Serial.println("CODE IS GOOD 9");
  BLEClient*  pClient  = BLEDevice::createClient();
  Serial.println(" - Created client");
Serial.println("CODE IS GOOD 10");
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

 
    return true;
}
