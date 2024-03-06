#include "functions.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

// I/O Pin declarations
int buttonPin = 13;
// int potPin = A2;
int moisturePin = A0;                 //OK moisture sensor
int PulseSensorPurplePin = A1;        //OK pulse Sensor - PPG sensor
int ICU2 = A2;                        //biodata2 - intercomunit sensor externo
int ICU1 = A3;                        //OK biodata1 - intercomunit sensor embutido
int myoPin = A4;                      //OK myoware - EMG sensor
const byte interruptPin = 12;         //OK galvanometer input

//atmosphere sensor
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME680 bme; // I2C

void setup() {
  Serial.begin(9600);
  wifiConnect();
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ICU1, INPUT);
  pinMode(ICU2, INPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(moisturePin, INPUT);
  pinMode(myoPin, INPUT);
   //setup pulse input pin
  attachInterrupt(digitalPinToInterrupt(interruptPin), sample, CHANGE);  //begin sampling from interrupt

  //atmosphere sensor
  //if (!bme.begin()) {
  //  Serial.println("Could not find a valid BME680 sensor, check wiring!");
  //  while (1);
  //}
  bme.begin();
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  delay(2000);
}

void loop() {

  //Plant Sensors readings
  biodata1 = analogRead(ICU1);
  biodata2 = analogRead(ICU2);
  if( sampleIndex >= samplesize )  { analyzeSample(); }
  galvanometer = value;
  myoware = analogRead(myoPin);
  pulsesensor = analogRead(PulseSensorPurplePin);
  moisture = analogRead(moisturePin);
  //atmosphere readings
  bme.performReading();
  temperature = bme.temperature;
  humidity = bme.humidity;
  pressure = bme.pressure/100.0;
  gas = bme.gas_resistance;
  if (digitalRead(buttonPin) == LOW) {
    activity = 1;
  } else{
    activity = 0;
  }

  // Send data API
  sendAPI(biodata1, biodata2, galvanometer, myoware, pulsesensor, moisture, temperature, humidity, pressure, gas, activity);

  // Send data BLE

  // Print test
  /*
  Serial.print("biodata1: ");
  Serial.print(biodata1);
  Serial.print(", biodata2: ");
  Serial.print(biodata2);
  Serial.print(", galvanometer: ");
  Serial.print(galvanometer);
  Serial.print(", myoware: ");
  Serial.print(myoware);
  Serial.print(", pulsesensor: ");
  Serial.print(pulsesensor);
  Serial.print(", moisture: ");
  Serial.print(moisture);
  Serial.print(", temperature: ");
  Serial.print(temperature);
  Serial.print(", humidity: ");
  Serial.print(humidity);
  Serial.print(", pressure: ");
  Serial.print(pressure);
  Serial.print(", gas: ");
  Serial.print(gas);
  Serial.print(", activity: ");
  Serial.println(activity);
  */
  
  delay(500);
}