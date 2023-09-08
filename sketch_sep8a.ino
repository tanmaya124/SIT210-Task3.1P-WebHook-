#include "arduino_secrets.h"   // Include a file that stores your secret credentials (e.g., Wi-Fi password).
#include "thingProperties.h"    // Include a file for managing properties used in the Arduino IoT Cloud.
#include <Wire.h>              // Include the Wire library for I2C communication.
#include <BH1750.h>            // Include the BH1750 library for working with a light sensor.

BH1750 lightMeter;             // Create an instance of the BH1750 light sensor.

const long interval = 1000;    // Define a time interval (in milliseconds) for taking light measurements.
unsigned long previousMillis = 0;  // Store the previous time when a measurement was taken.

float light_level;             // Store the current light level reading.
const float LIGHT_THRESHOLD = 1000;  // Define a threshold value for detecting sunlight.

void setup() {
  Serial.begin(9600);         // Start serial communication at a baud rate of 9600 (for debugging).
  Wire.begin();               // Initialize the I2C communication.
  lightMeter.begin();         // Initialize the BH1750 light sensor.
  initProperties();            // Initialize properties used for IoT communication.
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);  // Initialize the Arduino IoT Cloud.
  setDebugMessageLevel(2);    // Set the debug message level to 2 (for debugging purposes).
}

void loop() {
  ArduinoCloud.update();      // Update the Arduino IoT Cloud connection.
  unsigned long currentMillis = millis();  // Get the current time in milliseconds.
  
  // Check if it's time to take a new light measurement.
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Update the previous measurement time.
    light_level = lightMeter.readLightLevel();  // Read the current light level.
    
    // Check if the light level is above the threshold and if sunlight_update is not "started."
    if (light_level >= LIGHT_THRESHOLD && sunlight_update != "started") {
      sunlight_update = "started";  // Start updating sunlight status.
    }
    // Check if the light level is below the threshold and if sunlight_update is not "stopped."
    else if (light_level < LIGHT_THRESHOLD && sunlight_update != "stopped") {
      sunlight_update = "stopped";  // Stop updating sunlight status.
    }
  }
}
