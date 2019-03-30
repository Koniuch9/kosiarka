/*
  DC Voltmeter
  An Arduino based on voltage divider concept
  Shahadat Hossen Razib
  Youtube Channel: Razib Shahadat
*/

#include <LiquidCrystal.h>
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int analogInput = 1;
float vout = 0.0;
float vin = 0.0;
float R1 = 100000.0; // R1(100K)
float R2 = 10000.0; //  R2 (10K)
int value = 0;
int a[5] = {};
void setup() {
  Serial.begin(9600);
  pinMode(analogInput, INPUT);
//  lcd.begin(16, 2);
//  lcd.setCursor(2, 0);
//  lcd.print("DC VOLTMETER");
//  lcd.setCursor(0, 1);
//  lcd.print("-----YAHOO-----");
//  delay(3000);
//  lcd.clear();

}
unsigned long timeNow = 0; 
void loop() {
  //lcd.print("DC Voltmeter");
  // read the value at analog input
  value = analogRead(analogInput);
  vout = (value * 5) / 1024.0;
  vin = vout / (R2 / (R1 + R2));
  //Serial.println(vin);
  if (vin < 50.)
  {
    vin = 0.0;
  } else {
    long rpm = 60000000/(micros()-timeNow)/2;
    if(rpm < 4000)
    Serial.println(rpm);
    timeNow = micros();    
  }
//  lcd.setCursor(0, 1);
//  lcd.print("Voltage V :");
//  lcd.print(vin);
  //delay(1000);
  //lcd.clear();
}
