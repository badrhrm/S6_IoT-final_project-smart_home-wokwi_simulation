// Channel ID: 2535771
// channel api key : G1SLZUJLOQ0QOBX0

#include <DHT.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "ThingSpeak.h"
#include <UrlEncode.h>

#define DHTPIN 13      
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

Servo servo;
#define SERVO_PIN 26
#define CO2_NORMAL 1000
#define CO2_HIGH 3000
#define CO2_DANGEROUS 4000 


#define POTENTIOMETER_PIN 33
#define BUZZER_PIN 32

#define PIN_R 27
#define PIN_G 14
#define PIN_B 12

#define seuil_climatisation 35
#define seuil_chauffage 10


// These constants should match the photoresistor's "gamma" and "rl10" attributes
const float GAMMA = 0.7;
const float RL10 = 50;

const int ledCount = 10; // Nombre de LED dans la barre graphique
const int ldrPin = 34; // Pin du capteur de luminosité (LDR)

// array of pins for LEDs in the bar graph
int ledPins[] = {
  0,4,16,17,5,18,19,21, 22, 23,        
};
// Seuils de luminosité
const int seuil_dark = 50;   // Seuil de luminosité pour "dark"
const int seuil_light = 500;  // Seuil de luminosité pour "light"
const int seuil_bright = 1000; // Seuil de luminosité pour "bright"

const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ThingSpeak configuration
const char* thingspeakServer = "api.thingspeak.com";
const char* apiKey = "G1SLZUJLOQ0QOBX0";
const int myChannelNumber =2535771 ;
WiFiClient client;

//whatsapp configuration 
String phone = "212624373495";
String apikey = "8704542";
String text;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  dht.begin();

  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);

  pinMode(POTENTIOMETER_PIN, INPUT);
  servo.attach(SERVO_PIN);
  servo.write(0);

  // Loop over the LEDs pins and set all to output
  for (int i = 0; i < ledCount; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Wifi not connected");
  }
  Serial.println("Wifi connected !");
  Serial.println("Local IP: " + String(WiFi.localIP()));
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop() {
  delay(1000);

  // Read the CO2 level from the potentiometer
  int co2Level = analogRead(POTENTIOMETER_PIN);
  Serial.print("CO2 level: ");
  Serial.println(co2Level);

  // Read humidity and temperature
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // 1. Scénario :
  int servoPosition = 0;
  if (co2Level > CO2_NORMAL) {
    // mapping the CO2 level to the servo position (0-180 degrees)
    int x = map(co2Level, CO2_NORMAL, CO2_HIGH, 0, 180);
    if(x > servoPosition){
      servoPosition = x;
    }
    Serial.print("co2 Servo Position: ");
    Serial.println(x);
  }
  
  if (co2Level>=CO2_DANGEROUS){
    tone(BUZZER_PIN, 262, 250); // 262Hz tone for 0.250 seconds
    sendAlertToWhatsapp("ALERT DANGER! {niveau dyal Co2 rah kbir bzaf}");
  } else {
    noTone(BUZZER_PIN);
  }
  
  if (humidity >= 40) {
    int x = map(humidity, 40, 75, 0, 180);
    if(x > servoPosition){
      servoPosition = x;
    }
    Serial.print("humidity Servo position: ");
    Serial.println(x);
  }

  if (temperature >= 30){
    int x = map(temperature, 30, 40, 0, 180);
    if(x > servoPosition){
      servoPosition = x;
    }
    Serial.print("temperature Servo position: ");
    Serial.println(x);
  }
  
  if( (temperature<30) && (humidity<40) && (co2Level<= CO2_NORMAL) ){ 
    servo.write(0);
  }else {
    servo.write(servoPosition);
  }


  // 2. Scénario :
  if(temperature > seuil_climatisation){
    // Activer le climatiseur (LED devient bleue)
    digitalWrite(PIN_R, LOW);
    digitalWrite(PIN_G, LOW);
    digitalWrite(PIN_B, HIGH);
  }
  else if(temperature < seuil_chauffage){
    // Activer le chauffage (LED devient rouge)
    digitalWrite(PIN_R, HIGH);
    digitalWrite(PIN_G, LOW);
    digitalWrite(PIN_B, LOW);
  }else {
    // Température dans la plage acceptable 
    digitalWrite(PIN_R, LOW); 
    digitalWrite(PIN_G, LOW);  
    digitalWrite(PIN_B, LOW);   
  }

  // 3. Scénario :
  // Convert the analog value into lux value:
  int analogValue = analogRead(ldrPin);
  float voltage = analogValue / 4096. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
  Serial.print("The brightness is ");
  Serial.print(lux);
  Serial.println(" lx");
  

  int lightLevel = lux;
  if (lightLevel < seuil_dark) {
    // lightlevel < 50 lx
    // activate 1 led 
    activateLEDs(1);
  } else if (lightLevel >= seuil_dark && lightLevel < seuil_light) {
    //  50 lx <= lightlevel < 500 lx
    // activate 1 + 2 led 
    activateLEDs(3);
  } else if (lightLevel >= seuil_light && lightLevel < seuil_bright) {
    //  500 lx <= lightlevel < 1000 lx
    // activate 1 + 2 + 3 led 
    activateLEDs(6);
  } else if (lightLevel > seuil_bright){
    // activate 1 + 2 + 3 + 4 led 
    activateLEDs(10);
  }

  // configuration de ThingSpeak 
  ThingSpeak.setField(1,temperature);
  ThingSpeak.setField(2,humidity);
  ThingSpeak.setField(3,co2Level);
  ThingSpeak.setField(4,lightLevel);

  int xx = ThingSpeak.writeFields(myChannelNumber,apiKey);
  
  if(xx == 200){
    Serial.println("Data pushed successfull");
  }else{
    Serial.println("Push error" + String(xx));
  }
}



void activateLEDs(int numLEDs) {
  // shut down all leds
  for (int i = 0; i < ledCount; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  
  // Allumer les LED spécifiées
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
}


void sendAlertToWhatsapp(String message){
  // message to send with HTTP post
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phone + "&apikey=" + apikey + "&text=" + urlEncode(message);    
  HTTPClient http;
  http.begin(url);

  // specify content type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.println("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}
