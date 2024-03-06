#include <Arduino.h>
#include <HardwareSerial.h>
#include <HTTPClient.h>
#include "WiFi.h"

#define SSID "Quintal"
#define PASSWORD "beijaflora"
// #define SSID "HughesNet_6032B1"
// #define PASSWORD "HN57382C"
// #define SSID "informatica"
// #define PASSWORD "informatica453"

// String server_url = "https://plants.ifprinteligente.com.br/data/create";
String server_url = "http://apibiodata.sympoiesis.xyz/data/create";

float biodata1, biodata2, galvanometer, myoware, pulsesensor, moisture, temperature, humidity, pressure, gas;
int activity;

//Sampling

//****** sample size sets the 'grain' of the detector
// a larger size will smooth over small variations
// a smaller size will excentuate small changes
const byte samplesize = 10; //set sample array size
const byte analysize = samplesize - 1;  //trim for analysis array,
volatile unsigned long microseconds; //sampling timer
volatile byte sampleIndex = 0;
volatile unsigned long samples[samplesize];
float threshold = 1.71; //threshold multiplier
word value = 0;
void sample();
void analyzeSample();