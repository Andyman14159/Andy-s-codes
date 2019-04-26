/*    This code currently serves as a rudamentary burglar alarm for my front door. I have a NodeMcu 0.9 ESP-F 
 * with a reed switch wired ground to ground and the signal on the switch is wired to D1(GPIO 5) on the board.
 * When the reed switch is Open, the door is closed and when the door is open, the switch is closed and it pulls
 * the signal low on D1. Any time the state of the door changes, a webhook trigger is fired through a web request 
 * using the IFTTT web service. Debugging in the serial port is really nice on this one but the libraries were a 
 * pain in the butt. I had to delete multiple entries of the "WiFi.h" file on my computer until the compiler 
 * found the right one that it wanted to use. I hope my version helped make things a little easier for you, 
 * my DIY friends.     -ANDY ROLLER 
 *                     25 April, 2019                                                                   
 */

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#define REED_SWITCH 5  // D1

int status = WL_IDLE_STATUS; // not required.

const char* ssid = "XXXX";        // Change this to your SSID
const char* password = "XXXX";    // Change this to your password
int doorClosed = 1;

void setup() {
  pinMode(REED_SWITCH, INPUT_PULLUP);

  Serial.begin(115200);

  setupWifi();
   
   //get_httpOpen();

}

void setupWifi()
{
     // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
   WiFi.mode(WIFI_STA);
   status = WiFi.begin(ssid, password);    

   Serial.print("Attempting to connect to SSID: ");
   Serial.println(ssid);  

   // Wait for connection
   while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
   }
   Serial.println("Connected to wifi");
}

void loop() {
   if (WiFi.status() != WL_CONNECTED)
   {
     setupWifi();
   }
  // put your main code here, to run repeatedly:
    if ((digitalRead(REED_SWITCH) == HIGH) && (doorClosed == 1))
    {
      Serial.println("DOOR OPEN!!");
      while (get_httpOpen(String("DOOR_OPEN_")) != 0);      
      doorClosed = 0;
    } 
    else if ((digitalRead(REED_SWITCH) == LOW) && (doorClosed == 0))
    {
      Serial.println("DOOR CLOSED!!");
      while (get_httpClosed(String("DOOR_CLOSED_")) != 0);      
      doorClosed = 1;
    }
    delay(10);


}


int get_httpOpen(String state)
{
   HTTPClient http;
   int ret = 0;
   Serial.print("[HTTP] begin...\n");
   // configure ifttt server and url  should be HTTP only..not https!!!  (http://)   
   http.begin("http://maker.ifttt.com/trigger/Front_Door_Open/with/key/nrj6JrimOsEKXQE-iHSk7aAUABHWDAvj3a-qGkRkxEi"); //HTTP
   //If you have enabled value1 from iftt settings then uncomment below line to send value and comment out above line  
   //http.begin("http://maker.ifttt.com/trigger/Door/with/key/nrj6JrimOsEKXQE-iHSk7aAUABHWDAvj3a-qGkRkxEi"+state); //HTTP

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    // httpCode will be negative on error
    if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET code: %d\n", httpCode);

      if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
        ret = -1;
        Serial.printf("[HTTP] GET failed, error: %s\n", http.errorToString(httpCode).c_str());
        delay(500); // wait for half sec before retry again
    }

    http.end();
    return ret;
}  

int get_httpClosed(String state)
{
   HTTPClient http;
   int ret = 0;
   Serial.print("[HTTP] begin...\n");
   // configure ifttt server and url  should be HTTP only..not https!!!  (http://)   
   http.begin("http://maker.ifttt.com/trigger/event/with/your/own personal key"); // Change this to your own IFTTT Webhook KEY
   //If you have enabled value1 from iftt settings then uncomment below line to send value and comment out above line  
   //http.begin("http://maker.ifttt.com/trigger/event/with/your/own personal key"+state); // Be sure to remove the "s" from "https://" it will only accept "http://"

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    // httpCode will be negative on error
    if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET code: %d\n", httpCode);

      if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
        ret = -1;
        Serial.printf("[HTTP] GET failed, error: %s\n", http.errorToString(httpCode).c_str());
        delay(500); // wait for half sec before retry again
    }

    http.end();
    return ret;
}  
