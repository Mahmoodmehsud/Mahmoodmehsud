#include <WiFi.h>
#include <HTTPClient.h>
  
const char* ssid = "CHIWifi2.4";
const char* password =  "L3tme1n@CHI";
  
void setup() {
  
  Serial.begin(115200);
  delay(4000);   //Delay needed before calling the WiFi.begin
  
  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println("Connected to the WiFi network");
  
}
  
void loop() {
  
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
  
   HTTPClient http;   
  
   http.begin("https://charms-qa.cognitivehealthintl.com/auth/Login2");  //Specify destination for HTTP request
   http.addHeader("Content-Type", "application/json");             //Specify content-type header
  
   int httpResponseCode = http.POST("{\"app_type\": \"Doctor\",\"app_version\": \"3.1.9-19000\",\"device_id\": \"ba0944e1a1ab6b46\",\"device_model\": \"a01qxx\",\"device_name\": \"SM-A015F\",\"device_type\": \"Android\",\"lang\": \"en\",\"os_name\": \"Android\",\"os_version\": \"10\",\"password\": \"Kashif123@\",\"pn_token\": \"dSbBeY0GLlg:APA91bHIfYat_LvRQMdPFI1m3gKCuHjKoyrUoDOShbYeTNIaNbffz_FGd7VIbffB2DUF4FI6f3z0OHEJEiUszMP9kZ7WRyaY88QWYmiUJhR0Avh1HBv4dwljDGdSeCu2dlfKGnzaaDaZ\",\"pn_type\": \"Firebase\",\"prefix\": \"charms-qa\",\"username\": \"kashifd\"}");   //Send the actual POST request

  
   if(httpResponseCode>0){
  
    String response = http.getString();                       //Get the response to the request
  
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
  
   }else{
  
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  
   }
  
   http.end();  //Free resources
  
 }else{
  
    Serial.println("Error in WiFi connection");   
  
 }
  
  delay(10000);  //Send a request every 10 seconds
  
}
