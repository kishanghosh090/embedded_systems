#include <WiFi.h>


const char* WIFI_SSID = "Airtel_Sabita";
const char* WIFI_PASSWORD = "Kishan@2005";

WiFiServer server(5000);

void setup() {

  Serial.begin(115200);

  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD);

  Serial.print(
    "Connecting");

  while (
    WiFi.status() != WL_CONNECTED) {

    Serial.print(".");
    delay(500);
  }


  Serial.println();

  Serial.println(
    "Connected");

  Serial.print(
    "IP: ");

  Serial.println(
    WiFi.localIP());

  server.begin();

  Serial.println(
    "TCP Server Started");
}

void loop() {

  WiFiClient client =
    server.available();

  if (!client) {
    return;
  }

  Serial.println(
    "Client Connected");

  client.println(
    "Welcome to Venthen Device");

  while (
    client.connected()) {

    if (
      client.available()) {

      String command =
        client.readStringUntil('\n');

      command.trim();

      Serial.print(
        "Command: ");

      Serial.println(
        command);
      if (command == "status") {
        client.println(
          "WiFi Connected");

        client.println(
          WiFi.localIP());
      }
      if (command == "exit") {
        client.println(
          "connection close");

          client.stop();
          
      }
      client.print("> ");

      client.println(
        command);
    }
  }

  Serial.println(
    "Client Disconnected");
}