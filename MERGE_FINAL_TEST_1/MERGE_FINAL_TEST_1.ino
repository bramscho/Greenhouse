//START TEMP/HUMIDITY

#include "DHT.h"

#define DHTPIN 2 //TEMP/HUMIDTY DIGITAL PIN

#define DHTTYPE DHT11  

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

//END TEMP/HUMIDITY

//START LCD/SOIL MOISTURE

#include <Wire.h>
#include "LCD.h"
#include "LiquidCrystal_I2C.h"
#define I2C_ADDR    
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

int soilSensorPin = A0; //SOIL SENSOR PIN
int soilSensorValue = 0;  // variable to store the value coming from the sensor
int soilSensorPercentage = 0;

int solonoidPin = 11; //SOLONOID DIGITAL PIN

//END LCD/SOIL MOISTURE

void setup() {
  
  //START TEMP/HUMIDITY
  
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  dht.begin();

  //END TEMP/HUMIDITY

  //START LCD/SOIL MOISTURE
  
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  Serial.begin(9600);
  lcd.begin(16, 2);// set up the LCD's number of columns and rows:


  //END LCD/SOIL MOISTURE
  
}

void loop() {

  //START TEMP/HUMIDITY
  
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

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

  //END TEMP/HUMIDITY

  //START LCD/SOIL MOISTURE

   // read the value from the sensor:
  soilSensorValue = analogRead(soilSensorPin);
  soilSensorPercentage = ((1023 - soilSensorValue) / 10.23);

  Serial.print("soil sensor = " );
  Serial.println(soilSensorPercentage);
  lcd.setCursor(0, 1);
  lcd.print("  Moisture: ");
  lcd.print(soilSensorPercentage);
  lcd.print("%        ");
  delay(350);


  if (soilSensorPercentage < 30) {
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

  //END LCD/SOIL MOISTURE
}
