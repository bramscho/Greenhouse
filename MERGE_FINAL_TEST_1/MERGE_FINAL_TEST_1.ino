/*
  How to connect temop.humidity sensor:
  Connect pin 1 (on the left) of the sensor to +5V
  NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
  to 3.3V instead of 5V!
  Connect pin 2 of the sensor to whatever your DHTPIN is
  Connect pin 4 (on the right) of the sensor to GROUND
  Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
  Initialize DHT sensor.
  Note that older versions of this library took an optional third parameter to
  tweak the timings for faster processors.  This parameter is no longer needed
  as the current DHT reading algorithm adjusts itself to work on faster procs.
*/

//TEMP/HUMIDITY Libraries
#include "DHT.h"

//TEMP/HUMIDITY Defines
#define DHTPIN 1 //TEMP/HUMIDTY DIGITAL PIN
#define DHTTYPE DHT11

//TEMP/HUMIDITY Other Stuff
DHT dht(DHTPIN, DHTTYPE);

//LCD/SOIL Libraries
#include <Wire.h>
#include "LCD.h"
#include "LiquidCrystal_I2C.h"

//LCD/SOIL Defines
#define I2C_ADDR 0x27
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

//LCD/SOIL Other Stuff
int n = 1;
LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

//LCD/SOIL Int's
int soilSensorPin = A0; //SOIL SENSOR PIN
int soilSensorValue = 0;  // variable to store the value coming from the sensor
int soilSensorPercentage = 0;
int solonoidPin = 2; //SOLONOID DIGITAL PIN

void setup() {

  Serial.begin(9600); //start serial monitor at 9600 baud(bits per second)

  dht.begin(); //start the temp/humidity sensor

  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE); //set the LCD's backlight as positive
  lcd.setBacklight(HIGH); // turn on the LCD's backlight

  lcd.begin(16, 2);//start the LCD and set up the LCD's number of columns and rows

}

void loop() {

  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Read temperature as Celsius (the default)
  float f = dht.readTemperature(true); // Read temperature as Fahrenheit (isFahrenheit = true)

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  float hif = dht.computeHeatIndex(f, h); // Compute heat index in Fahrenheit (the default)
  float hic = dht.computeHeatIndex(t, h, false); // Compute heat index in Celsius (isFahreheit = false)

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");

  soilSensorValue = analogRead(soilSensorPin); // read the value from the soil sensor:
  soilSensorPercentage = ((1023 - soilSensorValue) / 10.23); //convert the incoming analog reading to a percentage

  Serial.print("soil sensor = " );
  Serial.println(soilSensorPercentage);
  lcd.setCursor(0, 1);
  lcd.print("  Moisture: ");
  lcd.print(soilSensorPercentage);
  lcd.print("%        ");

  if (soilSensorPercentage < 30) {
    lcd.setCursor(4, 0);
    Serial.println("Water On");
    digitalWrite(2, HIGH);//if water level is below desired percentage, send power to solenoid
    lcd.print("Water On    ");
    digitalWrite(2, LOW);
  }
  else
  {
    lcd.clear();
    lcd.setCursor(4, 0);
    Serial.println("Water Off");
    digitalWrite(2, LOW);//if water level is at desired percentage, shut off power to the solenoid
    lcd.print("Water Off");
    digitalWrite(2, HIGH);
  }
  delay(500);
}
