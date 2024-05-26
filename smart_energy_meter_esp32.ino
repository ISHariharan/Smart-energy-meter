#include <LiquidCrystal.h>

#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include<WiFi.h>
#include<HTTPClient.h>

#define ZMPT_PIN 34
#define SCT_PIN 35
LiquidCrystal_I2C lcd(0x27, 16, 2);

const float VOLTAGE_CALIBRATION_FACTOR = 1.0; 
const float CURRENT_CALIBRATION_FACTOR = 1.0; 

String URL = "http://10.16.50.32/IOT/data.php";
const char* ssid = "hariharan"; 
const char* password = "hariharan"; 


void setup() {
  Serial.begin(9600); 
  connectWiFi();
  lcd.init(); 
  lcd.backlight(); 
  lcd.setCursor(0, 0);
  lcd.print("Voltage: ");
  lcd.setCursor(0, 1);
  lcd.print("Current: ");
  lcd.setCursor(0, 2);
  lcd.print("Power: ");
  // lcd.setCursor(0, 3);
  // lcd.print("Energy: ");

}

void loop() {
  if(WiFi.status() != WL_CONNECTED) { 
    connectWiFi();
  }
  
  float voltage = readVoltage();
  float current = readCurrent();

  float power = voltage * current;
  float energy = power * (millis() / 1000.0 / 3600.0); 
  
  String postData = "voltage=" + String(voltage) + "&current=" + String(current); 

  HTTPClient http; 
  http.begin(URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  int httpCode = http.POST(postData); 
  String payload = http.getString(); 
  
  Serial.print("URL : "); Serial.println(URL); 
  Serial.print("Data: "); Serial.println(postData); 
  Serial.print("httpCode: "); Serial.println(httpCode); 
  Serial.print("payload : "); Serial.println(payload); 
  Serial.println("--------------------------------------------------");
  
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print("V\t Current: ");
  Serial.print(current);
  Serial.print("A\t Power: ");
  Serial.print(power);
  Serial.print("W\t Energy: ");
  Serial.print(energy);
  Serial.println("Wh");

  lcd.setCursor(9, 0);
  lcd.print(voltage);
  lcd.setCursor(9, 1);
  lcd.print(current);
  lcd.setCursor(9, 2);
  lcd.print(power);
  // lcd.setCursor(9, 3);
  // lcd.print(energy);
  delay(1000);
}

float readVoltage() {

  int sensorValue = analogRead(ZMPT_PIN);
  float voltage = sensorValue * (5.0 / 1023.0); 
  return voltage * VOLTAGE_CALIBRATION_FACTOR; 
}

float readCurrent() {
  
  int sensorValue = analogRead(SCT_PIN);
  float current = sensorValue * (5.0 / 1023.0); 
  
  current = current * 30.0 / 5.0; 
  return current * CURRENT_CALIBRATION_FACTOR; 
}

void connectWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  //This line hides the viewing of ESP as wifi hotspot
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    
  Serial.print("connected to : "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
}
