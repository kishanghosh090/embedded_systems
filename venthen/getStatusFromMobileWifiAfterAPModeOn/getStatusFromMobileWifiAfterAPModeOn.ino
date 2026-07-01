#include <WiFi.h>
#include <Preferences.h>
#include <WebServer.h>
#include <ArduinoJson.h>

Preferences preferences;
WebServer server(80);

// Temporary device ID
String deviceId = "VTH001";

// -------------------------
// STATUS API
// -------------------------
void handleStatus() {
  String response = "{";
  response += "\"deviceId\":\"" + deviceId + "\",";
  response += "\"firmware\":\"1.0.0\",";
  response += "\"configured\":false";
  response += "}";

  server.send(
      200,
      "application/json",
      response
  );
}
void handleWifi() {

  if (!server.hasArg("plain")) {
    server.send(400, "application/json",
                "{\"success\":false}");
    return;
  }

  String body = server.arg("plain");

  DynamicJsonDocument doc(512);

  deserializeJson(doc, body);

  String ssid = doc["ssid"];
  String password = doc["password"];
  String jwt = doc["jwt"];

  preferences.putString("ssid", ssid);
  preferences.putString("password", password);
  preferences.putString("jwt", jwt);

  preferences.putBool("wifi_done", true);

  server.send(
      200,
      "application/json",
      "{\"success\":true}"
  );

  delay(2000);

  ESP.restart();
}
void handleInfo() {

  DynamicJsonDocument doc(512);

  doc["deviceId"] = deviceId;
  doc["firmware"] = "1.0.0";
  doc["wifiConfigured"] =
      preferences.getBool(
          "wifi_done",
          false
      );

  doc["connected"] =
      WiFi.status() == WL_CONNECTED;

  doc["ip"] =
      WiFi.localIP().toString();

  String response;

  serializeJson(
      doc,
      response
  );

  server.send(
      200,
      "application/json",
      response
  );
}
bool connectWiFi() {

  String ssid =
      preferences.getString(
          "ssid",
          ""
      );

  String password =
      preferences.getString(
          "password",
          ""
      );

  Serial.println();
  Serial.println("Connecting to WiFi...");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);

  WiFi.begin(
      ssid.c_str(),
      password.c_str()
  );

  int retries = 0;

  while (
      WiFi.status() != WL_CONNECTED &&
      retries < 20
  ) {

    delay(500);

    Serial.print(".");

    retries++;
  }

  Serial.println();

  if (
      WiFi.status() == WL_CONNECTED
  ) {

    Serial.println("WiFi Connected");

    Serial.print("IP: ");
    Serial.println(
        WiFi.localIP()
    );

    return true;
  }

  Serial.println(
      "Connection Failed"
  );

  return false;
}
// -------------------------
// START AP
// -------------------------
void startAP() {
  String ssid = "Venthen-" + deviceId;
  String password = "12345678";

  WiFi.mode(WIFI_AP);

  bool success = WiFi.softAP(
      ssid.c_str(),
      password.c_str()
  );

  if (success) {

    Serial.println();
    Serial.println("===== AP STARTED =====");

    Serial.print("SSID: ");
    Serial.println(ssid);

    Serial.print("Password: ");
    Serial.println(password);

    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    // API Route
    server.on(
        "/status",
        HTTP_GET,
        handleStatus
    );
    server.on(
        "/wifi",
        HTTP_POST,
        handleWifi
    );
  
    server.begin();

    Serial.println("HTTP Server Started");
    Serial.println("====================");
  }
  else {
    Serial.println("AP Failed");
  }
}

void startServer() {

    server.on(
        "/status",
        HTTP_GET,
        handleStatus
    );

    server.on(
        "/info",
        HTTP_GET,
        handleInfo
    );

    server.begin();

    Serial.println(
        "HTTP Server Started"
    );
}
// -------------------------
// SETUP
// -------------------------
void setup() {

  Serial.begin(115200);

  preferences.begin(
      "venthen",
      false
  );

  bool wifiConfigured =
      preferences.getBool(
          "wifi_done",
          false
      );

  if (!wifiConfigured) {

    Serial.println(
        "No WiFi configured."
    );

    startAP();
  }
  else {

     bool connected =
        connectWiFi();

    if (!connected) {

        Serial.println(
            "Starting AP Mode."
        );

        startAP();
    }
 
  }
  startServer();
}

// -------------------------
// LOOP
// -------------------------
void loop() {
  server.handleClient();
}