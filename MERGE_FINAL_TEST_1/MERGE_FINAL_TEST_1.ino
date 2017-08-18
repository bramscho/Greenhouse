/*
  How to connect temp.humidity sensor:
  Connect pin 1 (on the left) of the sensor to +5V
  NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
  to 3.3V instead of 5V!
  Connect pin 2 of the sensor to whatever your DHTPIN is
  Connect pin 4 (on the right) of the sensor to GROUND
  Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
  Initialize DHT sensor.

light - timer
soil heater- timer

soil moisture/watering - waterbot

PC fan - 24/7

soil temp/soil moisture/ph level - electronic test probe thingie

air temp/humidity - sensor and little weather station

ALL READING WILL BE READ BY SERIAL MONITOR
*/

#include "DHT.h" // temp/humidity sensor library

#define DHTPIN 1 //what digit pin the temp/humidity sensor will be plugged into
#define DHTTYPE DHT11 // define what type of sensor it is

DHT dht(DHTPIN, DHTTYPE); // other stuff for the temp/humidity sensor

int n = 1; //soil moisture other stuff

int soilSensorPin = A0; //SOIL SENSOR PIN
int soilSensorValue = 0;  ////variable to hold the incoming soil moisture value at
int soilSensorPercentage = 0; //variable to hold the incoming soil moisture value at
int solonoidPin = 2; //SOLONOID DIGITAL PIN

void setup() {

  Serial.begin(9600); //start serial monitor at 9600 baud(bits per second)

  dht.begin(); //start the temp/humidity sensor
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

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
 
  soilSensorValue = analogRead(soilSensorPin); // read the value from the soil sensor:
  soilSensorPercentage = ((1023 - soilSensorValue) / 10.23); //convert the incoming analog reading to a percentage

  Serial.print("soil sensor = " );
  Serial.print(soilSensorPercentage);
  Serial.print("  Moisture: ");
  Serial.print(soilSensorPercentage);
  Serial.print("%        ");

  if (soilSensorPercentage < 30) {
    Serial.print("Water On");
    digitalWrite(2, HIGH);//if water level is below desired percentage, send power to solenoid
    Serial.print("Water On    ");
    digitalWrite(2, LOW);
  }
  else
  {
    Serial.println("Water Off");
    digitalWrite(2, LOW);//if water level is at desired percentage, shut off power to the solenoid
    Serial.print("Water Off");
    digitalWrite(2, HIGH);
  }
  delay(500);
}
