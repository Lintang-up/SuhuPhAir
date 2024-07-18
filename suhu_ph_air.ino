
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6sv9x4MfQ"
#define BLYNK_TEMPLATE_NAME "sensor ph dan suhu air"
#define BLYNK_AUTH_TOKEN "t2TVcRGnS5_OatHOMimUpyZe4U33kWNW"
 
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp8266.h>
 
char auth[] = "t2TVcRGnS5_OatHOMimUpyZe4U33kWNW";
char ssid[] = "hem";  //Access Point Wi-Fi
char pass[] = "00000000";  //Password
 
/* Deklarasi LCD I2C */
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
 
/* Deklarasi Sensor Suhu DS18B20 */
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
 
/* Deklarasi Sensor PH */
const int ph_pin = A0;
float Po = 0;
float PH_step;
int nilai_analog_PH;
double TeganganPh;
 
/* Nilai Kalibrasi PH */
float ph4 = 3.01;
float ph7 = 2.66;
 
/*========================================================================================*/
void setup(){
 
  lcd.init();
  lcd.backlight();
  
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  sensors.begin();
}
 
/*========================================================================================*/
void loop(){
  Blynk.run();
 
  //Sensor Suhu DS18B20
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  Serial.print("Temperature     : ");
  Serial.println(temperature, 2);
  
  //Nilai ADC Sensor PH
  int nilai_analog_PH = analogRead(ph_pin);
  Serial.print("Nilai ADC PH    : ");
  Serial.println(nilai_analog_PH);
 
 //Nilai Tegangan Sensor PH
  TeganganPh = 3.00 / 1023.0 * nilai_analog_PH;
  Serial.print("Tegangan PH     : ");
  Serial.println(TeganganPh, 3);
 
 //Nilai Cairan PH
  PH_step = (ph4 - ph7) / 3;
  Po = 7.00 + ((ph7 - TeganganPh) / PH_step);
  Serial.print("Nilai Cairan PH : ");
  Serial.println(Po, 2);
  Serial.println("=====================");
  Serial.println();
  delay(1000);
 
  //Print Nilai PH
  lcd.setCursor(0,0); lcd.print("Nilai PH : ");
  lcd.setCursor(11,0); lcd.print(Po, 1); lcd.print(" ");
 
  //Print Nilai Suhu
  lcd.setCursor(0,1);
  lcd.print("Suhu Air : ");
  lcd.print(temperature,0);
  lcd.print((char)223);
  lcd.print("C");
 
  Blynk.virtualWrite(V0, Po);
  Blynk.virtualWrite(V1, temperature);
}
