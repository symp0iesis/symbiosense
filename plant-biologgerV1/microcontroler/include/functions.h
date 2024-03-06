#include "var.h"

void wifiConnect() {
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);
    
    Serial.print("Connecting to WiFi Router.");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }

    Serial.print("Connected to the WiFi network: ");
    Serial.println(WiFi.localIP());
}

void sendAPI(float biodata1, float biodata2, float galvanometer, float myoware, float pulsesensor, float moisture, float temperature, float humidity, float pressure, float gas, int activity) {

    int httpResponseCode;
    HTTPClient http;


    if(WiFi.status() == WL_CONNECTED) {

        // String httpRequestData = "{\"sensor\":\"287\",\"press\":\"657.09\",\"humi\":\"49.54\",\"temp\":\"22.03\",\"gas\":\"4984\"}";
        String httpRequestData = "{\"biodata1\":\"";
        httpRequestData += biodata1;
        httpRequestData += "\",\"biodata2\":\"";
        httpRequestData += biodata2;
        httpRequestData += "\",\"galvanometer\":\"";
        httpRequestData += galvanometer;
        httpRequestData += "\",\"myoware\":\"";
        httpRequestData += myoware;
        httpRequestData += "\",\"pulsesensor\":\"";
        httpRequestData += pulsesensor;
        httpRequestData += "\",\"moisture\":\"";
        httpRequestData += moisture;
        httpRequestData += "\",\"temperature\":\"";
        httpRequestData += temperature;
        httpRequestData += "\",\"humidity\":\"";
        httpRequestData += humidity;
        httpRequestData += "\",\"pressure\":\"";
        httpRequestData += pressure;
        httpRequestData += "\",\"gas\":\"";
        httpRequestData += gas;
        httpRequestData += "\",\"activity\":\"";
        httpRequestData += activity;
        httpRequestData += "\"}";

        Serial.print(httpRequestData);

        http.begin(server_url);       
        http.addHeader("Content-Type", "application/json");
        httpResponseCode = http.POST(httpRequestData);
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        http.end();
    }
}

void sample(){
  if(sampleIndex < samplesize) {
    samples[sampleIndex] = micros() - microseconds;
    microseconds = samples[sampleIndex] + microseconds; //rebuild micros() value w/o recalling
    //micros() is very slow
    //try a higher precision counter
    //samples[index] = ((timer0_overflow_count << 8) + TCNT0) - microseconds;
    sampleIndex += 1;

  } 
}

void analyzeSample(){
  unsigned long averg = 0;
  unsigned long maxim = 0;
  unsigned long minim = 100000;
  float stdevi = 0;
  unsigned long delta = 0;
  byte change = 0;


  if (sampleIndex >= samplesize) { //array is full
    unsigned long sampanalysis[analysize];;
    for (byte i=0; i<analysize; i++){ 
      //skip first element in the array
      sampanalysis[i] = samples[i+1];  //load analysis table (due to volitle)
      //manual calculation
      if(sampanalysis[i] > maxim) { maxim = sampanalysis[i]; }
      if(sampanalysis[i] < minim) { minim = sampanalysis[i]; }
      averg += sampanalysis[i];
      stdevi += sampanalysis[i] * sampanalysis[i];  //prep stdevi
    }
    //manual calculation
    averg = averg/analysize;
    stdevi = sqrt(stdevi / analysize - averg * averg); //calculate stdevu
    if (stdevi < 1) { stdevi = 1.0; } //min stdevi of 1
    delta = maxim - minim; 
    //**********perform change detection 
    if (delta > (stdevi * threshold)){
      change = 1;
    }
    //*********
    if(change){
      value = stdevi;
      //Serial.println(stdevi);
      //Serial.println(map(constrain(threshold*stdevi,0,300),0,300,0,100)); Serial.print(","); //threshold compare against delta
      //Serial.println(map(delta,0,300,0,100)); Serial.print(","); //delta 
      //Serial.println(change*90); //Serial.print(","); //change detected      
     }
     //reset array for next sample
      sampleIndex = 0;
  }
}