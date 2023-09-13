#include <DHT.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>

// Define the pin for the DHT sensor and the sensor type
#define DHT_SENSOR_PIN 2
#define DHT_SENSOR_TYPE DHT11

// WiFi credentials
#define WIFI_SSID "Tanmaya's Iphone (2)"
#define WIFI_PASS "tanmayaspw"

// ThingSpeak credentials
#define THINGSPEAK_CHANNEL_ID 2249474
#define THINGSPEAK_API_KEY "X46C19PYVXSZT11K"

// Create DHT object and WiFiClient object
DHT dht(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
WiFiClient client;

void setup() {
  // Initialize serial communication at 9600 baud
  Serial.begin(9600);

  // Initialize the DHT sensor
  dht.begin();

  // Connect to WiFi
  connectToWiFi();

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Read temperature and humidity from the DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if the sensor readings are valid
  if (areReadingsValid(temperature, humidity)) {
    // Send data to ThingSpeak
    sendDataToThingSpeak(temperature, humidity);
  } else {
    Serial.println("Error reading sensor data!");
  }

  // Delay for 30 seconds before the next iteration
  delay(30000);
}

// Function to connect to WiFi
void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  Serial.println("Connected to WiFi");
}

// Function to check if sensor readings are valid
bool areReadingsValid(float temp, float humidity) {
  return !isnan(temp) && !isnan(humidity);
}

// Function to send data to ThingSpeak
void sendDataToThingSpeak(float temp, float humidity) {
  // Set the fields for temperature and humidity
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, humidity);

  // Write the fields to the ThingSpeak channel
  int httpCode = ThingSpeak.writeFields(THINGSPEAK_CHANNEL_ID, THINGSPEAK_API_KEY);

  // Check if the data was sent successfully
  if (httpCode == 200) {
    Serial.println("Data sent to ThingSpeak successfully");
  } else {
    Serial.println("Error sending data to ThingSpeak");
  }
}
