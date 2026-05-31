#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Airtel_Sabita";
const char* password = "Kishan@2005";

// void setup() {
//   Serial.begin(115200);

//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);

//   Serial.println("Connecting...");

//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.print("Status: ");
//     Serial.println(WiFi.status());
//     delay(1000);
//   }

//   Serial.println("Connected!");
//   Serial.print("IP: ");
//   Serial.println(WiFi.localIP());
// }

// void loop() {}


WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "Hello from ESP32!");
}

void handleChai(){
  server
}
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  

  Serial.println();
  Serial.println("Connected!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.begin();

  Serial.println("HTTP Server Started");
}

void loop() {
  server.handleClient();
}