#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <TinyGPS++.h>


#define FIREBASE_HOST "makethon-tracker.firebaseio.com"
#define FIREBASE_AUTH "prTzVLJW2owDX5JSgBOU0nAJJEShtyVoVbNZi6WQ"
#define WIFI_SSID "Redmi"
#define WIFI_PASSWORD "ajay12345"

String la1;
String lo1;
String myString;
int sigdata = 0; //data stored
double lo;
double la;
int vr = A0; //  connected 
int sdata = 0; // Pulse value will be stored in sdata.

static const int RXPin = 5, TXPin = 4;  // pins D1 and D2
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);


void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
  pinMode(vr ,INPUT);
  pinMode(D0,OUTPUT);
 WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
      {
    Serial.print(".");
    delay(500);
      }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Firebase.setString("gps/LAT","fahad");
  Firebase.setString("gps/LONG","fahad");
  Firebase.setString("Pulse/Value","fahad");

}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
  sdata = analogRead(vr);
  int BPM = 60000/sdata;
  myString = String(BPM);
  Serial.println(myString); 
  Firebase.setString("Pulse/Value",myString);
  delay(1000);
}

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
         la = gps.location.lat(), 6;
        Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
         lo = gps.location.lng(), 6;
   
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  la1 = String(la);
  Serial.println(la1); 
  Firebase.setString("gps/LAT",la1);
  lo1 = String(lo);
  Serial.println(lo1); 
  Firebase.setString("gps/LONG",lo1);
  delay(1000);
 
}
  
