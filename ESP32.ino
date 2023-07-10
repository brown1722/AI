#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <SimpleDHT.h>


/* 1. Define the WiFi credentials */
// #define WIFI_SSID "Mang Day KTX"
// #define WIFI_PASSWORD "hainam1011"
// #define WIFI_SSID "Xom Tro 123"
// #define WIFI_PASSWORD "thanhvsang"
// #define WIFI_SSID "Iphone 7"
// #define WIFI_PASSWORD "00000000"
// #define WIFI_SSID "TTHocLieu t1"
// #define WIFI_PASSWORD "hoclieut1"
#define WIFI_SSID "1234456"
#define WIFI_PASSWORD "00000000"
// #define WIFI_SSID "Zone Six-19PNX-5G"
// #define WIFI_PASSWORD "19phamnhuxuong"
#define LIGHT  34     // pin cam bien as
#define pinDHT11 4     // pin cam bien nhiet do, do am.

#define D1 12
#define D2 13
#define D3 14       //pin den
#define D4 27


SimpleDHT11 dht11(pinDHT11);

// Dong co DC
int ENA = 25;
int IN1 = 32;
int IN2 = 33;


/* Define the API Key */
#define FIREBASE_AUTH "PyYrZaiy4eg2C7S97OdmCoWOo8io08s0CKOp9I8y" // Nam
#define FIREBASE_HOST "https://database-d07ce-default-rtdb.firebaseio.com/" //Nam

/* Define the RTDB URL */
// #define FIREBASE_AUTH "8lPh1qwhHsBc53VdmLRn6EtockYoRkqDubN145Bw"
// #define FIREBASE_HOST "pbl5-868db-default-rtdb.asia-southeast1.firebasedatabase.app/" 

FirebaseData fbdb;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LIGHT, INPUT);
//led
pinMode(D1, OUTPUT);
pinMode(D2, OUTPUT);
pinMode(D3, OUTPUT);
pinMode(D4, OUTPUT);
//DC
pinMode(ENA, OUTPUT);
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);

// connect wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  server.begin();
  Serial.println("Server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


 // connect database

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(fbdb, 1000*60);
  Firebase.setwriteSizeLimit(fbdb, "tiny");

}

void loop() {


  
  // cam bien anh sang

  int valueLight = analogRead(LIGHT);
  Serial.print("Cam bien anh sang: "); Serial.println(valueLight);
  Firebase.setInt(fbdb, "/LIGHT", (valueLight < 700) ? 0 : 1);
  delay(300);
 


  // cam bien nhiet do, do am 
    byte temperature = 0;
    byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err=");
     Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); 
    Serial.println(SimpleDHTErrDuration(err)); 
    delay(300);
    return;
  }
  else{
  Serial.print("Cam bien nhiet do: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" H");
  Firebase.setInt(fbdb, "/Temp", temperature);
  Firebase.setInt(fbdb, "/Hum", humidity);
  }


//led 
if (Firebase.getInt(fbdb, "DEN1") == true) {
  digitalWrite(D1, fbdb.to<int>() == 1 ? HIGH : LOW);
}

if (Firebase.getInt(fbdb, "DEN2") == true) {
  digitalWrite(D2, fbdb.to<int>() == 1 ? HIGH : LOW);
}

if (Firebase.getInt(fbdb, "DEN3") == true) {
  digitalWrite(D3, fbdb.to<int>() == 1 ? HIGH : LOW);
}

if (Firebase.getInt(fbdb, "DEN4") == true) {
  digitalWrite(D4, fbdb.to<int>() == 1 ? HIGH : LOW);
}

//động cơ DC

 if(Firebase.getInt(fbdb, "Fan") == true)
 {
   int stDC = fbdb.to<int>();
  if(stDC == 1){
    analogWrite(ENA, 200);  // Điều khiển tốc độ động cơ
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW );
    delay(500);
  }
  else{
    analogWrite(ENA, 0);
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    delay(500);
    }
  }

  delay(300);
}



