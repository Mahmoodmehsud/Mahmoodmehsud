
void loop() {
 
    // let the websockets client check for incoming messages
   
    if(client->available()) {
        client->poll();
    }
     Serial.println("CODE IS GOOD 6");
    /* If the flag "doConnect" is true, then we have scanned for and found the desired
     BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
     connected we set the connected flag to be true. */
  bool connectToServer();
  
  
        Serial.println("CODE IS GOOD 7");
  if (doConnect == true)
  {
        Serial.println("CODE IS GOOD 8");
    if (connectToServer())
    {
      Serial.println("Connected to the BLE Server.");
      pconnected=true;
    } 
    else
    {
      Serial.println("Failed to connect");
    }
    doConnect = false;
    
  }

  /* If we are connected to a peer BLE Server, update the characteristic each time we are reached
     with the current time since boot */
  if (pconnected)
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
