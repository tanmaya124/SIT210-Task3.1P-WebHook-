#include "arduino_secrets.h"
#include "thingProperties.h"
#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

const long interval = 1000;
unsigned long previousMillis = 0;

float light_level;
const float LIGHT_THRESHOLD = 1000;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lightMeter.begin();
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
}

void loop() {
  ArduinoCloud.update();
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    light_level = lightMeter.readLightLevel();
    
    if (light_level >= LIGHT_THRESHOLD && sunlight_update != "started") {
      sunlight_update = "started";
    }
    else if (light_level < LIGHT_THRESHOLD && sunlight_update != "stopped") {
      sunlight_update = "stopped";
    }
  }
}
