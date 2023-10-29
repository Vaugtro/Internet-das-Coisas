#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>

const int LED_BUILTIN = 2;
const int IR_DETECTOR_PIN = 23; // obstacle avoidance sensor interface
const int ENABLE_PIN = 22;  // sensor enable interface (EN)

const char* SSID = "victor";
const char* PASSWORD = "victor";

const char* MQTT_SERVER = "broker.iot.com"; // Replace with your MQTT broker address
const int MQTT_PORT = 1883; // MQTT default port is 1883

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
	Serial.begin(115200);

	// Setup IR Sensor
	pinMode(LED_BUILTIN, OUTPUT); // Define LED as output interface
	pinMode(IR_DETECTOR_PIN, INPUT);  // Define obstacle avoidance sensor as input interface

	
	// Enable IR Sensor if needed
	//pinMode(22, INPUT);
	//pinMode(enablePin, OUTPUT);
	//digitalWrite(enablePin, LOW);  

	// Setting up as an Access Point
    	WiFi.mode(WIFI_AP);

	// Setting up SSID and Password for AP Mode
	WiFi.softAP(SSID, PASSWORD);

	while(WiFi.status() != WL_NO_SHIELD) {
	delay(1000);
	Serial.print(".");
	}

	Serial.println((String) "\nSetup AP Successfully. IP: " + WiFi.softAPIP().toString());

	// Setup MQTT
	client.setServer(MQTT_SERVER, MQTT_PORT);
}
void loop()
{
	if (!client.connected()) {
		if (client.connect("ESP32Client")) {
			Serial.println("Connected to MQTT broker");
		}
	}
	client.loop();

	int ir = digitalRead(IR_DETECTOR_PIN);

	// Publish the values to different MQTT topics
	client.publish("pin/status/ir", String(ir).c_str());

	// Add a delay to control the update rate
  	delay(1000); // Adjust the delay as needed
}