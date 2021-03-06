#include <Wire.h>
#include "LCD.h"
#include "LiquidCrystal_I2C.h"
#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
int n = 1;
LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

int sensorPin = A0;
int sensorValue = 0;  // variable to store the value coming from the sensor
int sensorPercentage = 0;

int solonoidPin = 11;
void setup()

{
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  Serial.begin(9600);
  lcd.begin(16, 2);// set up the LCD's number of columns and rows:


}

void loop()
{
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  sensorPercentage = ((1023 - sensorValue) / 10.23);

  Serial.print("sensor = " );
  Serial.println(sensorPercentage);
  lcd.setCursor(0, 1);
  lcd.print("  Moisture: ");
  lcd.print(sensorPercentage);
  lcd.print("%        ");
  delay(350);


  if (sensorPercentage < 30) {
    lcd.setCursor(4, 0);
    Serial.println("Water On");
    digitalWrite(9, HIGH);//if water level is below desired percentage, send power to solenoid
    lcd.print("Water On    ");
    digitalWrite(11, LOW);
  }
  else
  {
    lcd.clear();
    lcd.setCursor(4, 0);
    Serial.println("Water Off");
    digitalWrite(9, LOW);//if water level is at desired percentage, shut off power to the solenoid
    lcd.print("Water Off");
     digitalWrite(11, HIGH);
  }
}

