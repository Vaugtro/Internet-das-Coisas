#include <WiFi.h>
#include <WebServer.h>

int on = 0;

int LED_BUILTIN = 2;
// SSID & Password
const char* ssid = "Victor Augusto_2G_EXT"; // Enter your SSID here
const char* password = "g262ok1608"; //Enter your Password here

WebServer server(8080); // Object of WebServer(HTTP port, 80 is defult)

String HTML = "<!DOCTYPE html>\
<html>\
<body>\
<h1>My First Web Server with ESP32 - Station Mode &#128522;</h1>\
<form action='/' method='get'>\
<input type='submit' value='Ativar/Desativar led'></form>\
</body>\
</html>";

void ADButton() {
  if(on == 0){
    digitalWrite(LED_BUILTIN, HIGH);
    on = 1;
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
    on = 0;
  }
}

void handle_root() {
  ADButton();
  server.send(200, "text/html", HTML);
}


void setup() {
  pinMode (LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  delay(1000); // Delay to let system boot
  Serial.println("Scanning Networks...");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  bool found = false;

  // Print the name and RSSI (signal strength) of each network found
  if (n == 0)
    Serial.println("No networks found");
  else
  {
    Serial.print(n);
    Serial.println("Networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");

      if(strcmp(WiFi.SSID(i).c_str(), ssid) == 0){
        found = true;
      }
      delay(10);
    }
  }
  if(!found){
    Serial.println((String) "\nThe " + ssid + " network was not found, setting up as an Access Point");
    
    // Setting up as an Access Point
    WiFi.mode(WIFI_AP);

    // Setting up SSID and Password for AP Mode
    WiFi.softAP(ssid, password);

    while(WiFi.status() != WL_NO_SHIELD) {
      delay(1000);
      Serial.print(".");
    }

    Serial.println((String) "\nSetup AP Successfully. IP: " + WiFi.softAPIP().toString());
  } else {
    Serial.println((String) "The " + ssid + " network was found, connecting");

    // Connect to your wi-fi modem
    WiFi.begin(ssid, password);

    // Check wi-fi is connected to wi-fi network
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }

    Serial.println((String) "\nWiFi connected successfully. IP: " + WiFi.localIP().toString());
  }

  server.on("/", handle_root);
  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}

void loop() {
  server.handleClient();
}