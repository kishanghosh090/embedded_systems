#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

Preferences preferences;
WebServer server(80);

String deviceId = "VTH001";
String firmware = "1.0.0";

// Change this to your laptop IP
String backendUrl =
    "http://192.168.1.7:3000/devices/provision";


// ======================
// STATUS API
// ======================
void handleStatus() {

  DynamicJsonDocument doc(256);

  doc["deviceId"] = deviceId;
  doc["firmware"] = firmware;
  doc["configured"] =
      preferences.getBool(
          "wifi_done",
          false
      );

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


// ======================
// INFO API
// ======================
void handleInfo() {

  DynamicJsonDocument doc(512);

  doc["deviceId"] = deviceId;
  doc["firmware"] = firmware;

  doc["wifiConfigured"] =
      preferences.getBool(
          "wifi_done",
          false
      );

  doc["connected"] =
      WiFi.status() ==
      WL_CONNECTED;

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


// ======================
// WIFI CONFIG API
// ======================
void handleWifi() {

  if (!server.hasArg("plain")) {

    server.send(
        400,
        "application/json",
        "{\"success\":false}"
    );

    return;
  }

  String body =
      server.arg("plain");

  DynamicJsonDocument doc(512);

  deserializeJson(
      doc,
      body
  );

  String ssid =
      doc["ssid"];

  String password =
      doc["password"];

  String jwt =
      doc["jwt"];

  preferences.putString(
      "ssid",
      ssid
  );

  preferences.putString(
      "password",
      password
  );

  preferences.putString(
      "jwt",
      jwt
  );

  preferences.putBool(
      "wifi_done",
      true
  );

  server.send(
      200,
      "application/json",
      "{\"success\":true}"
  );

  delay(2000);

  ESP.restart();
}


// ======================
// HTTP SERVER
// ======================
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

  server.on(
      "/wifi",
      HTTP_POST,
      handleWifi
  );

  server.begin();

  Serial.println(
      "HTTP Server Started"
  );
}


// ======================
// AP MODE
// ======================
void startAP() {

  String ssid =
      "Venthen-" +
      deviceId;

  String password =
      "12345678";

  WiFi.mode(WIFI_AP);

  WiFi.softAP(
      ssid.c_str(),
      password.c_str()
  );

  Serial.println();
  Serial.println(
      "AP MODE"
  );

  Serial.print(
      "SSID: "
  );

  Serial.println(
      ssid
  );

  Serial.print(
      "IP: "
  );

  Serial.println(
      WiFi.softAPIP()
  );
}


// ======================
// WIFI CONNECTION
// ======================
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
  Serial.println(
      "Connecting to WiFi..."
  );

  Serial.println(
      ssid
  );

  WiFi.mode(
      WIFI_STA
  );

  WiFi.setAutoReconnect(
      true
  );

  WiFi.begin(
      ssid.c_str(),
      password.c_str()
  );

  int retries = 0;

  while (
      WiFi.status() !=
          WL_CONNECTED &&
      retries < 20
  ) {

    delay(500);

    Serial.print(".");

    retries++;
  }

  Serial.println();

  if (
      WiFi.status() ==
      WL_CONNECTED
  ) {

    Serial.println(
        "WiFi Connected"
    );

    Serial.print(
        "IP: "
    );

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


// ======================
// DEVICE PROVISIONING
// ======================
bool provisionDevice() {

  HTTPClient http;

  http.begin(
      backendUrl
  );

  http.addHeader(
      "Content-Type",
      "application/json"
  );

  DynamicJsonDocument doc(512);

  doc["deviceId"] =
      deviceId;

  doc["firmware"] =
      firmware;

  doc["jwt"] =
      preferences.getString(
          "jwt",
          ""
      );

  String requestBody;

  serializeJson(
      doc,
      requestBody
  );

  int responseCode =
      http.POST(
          requestBody
      );

  Serial.print(
      "Response Code: "
  );

  Serial.println(
      responseCode
  );

  if (responseCode == 200) {

    String response =
        http.getString();

    Serial.println("Provision Response:");
    Serial.println(response);

    DynamicJsonDocument res(1024);

    DeserializationError error =
        deserializeJson(
            res,
            response
        );

    if (error) {

        Serial.println(
            "JSON Parse Failed"
        );

        http.end();

        return false;
    }

    String registeredBy =
        res["registeredBy"]
            .as<String>();

    String collegeId =
        res["collegeId"]
            .as<String>();

    String departmentId =
        res["departmentId"]
            .as<String>();

    String deviceSecret =
        res["deviceSecret"]
            .as<String>();

    String mqttUser =
        res["mqttUser"]
            .as<String>();

    String mqttPassword =
        res["mqttPassword"]
            .as<String>();

    Serial.println();
    Serial.println("Provisioned Data");

    Serial.print("Registered By: ");
    Serial.println(registeredBy);

    Serial.print("College ID: ");
    Serial.println(collegeId);

    Serial.print("Department ID: ");
    Serial.println(departmentId);

    Serial.print("MQTT User: ");
    Serial.println(mqttUser);

    preferences.putString(
        "registeredBy",
        registeredBy
    );

    preferences.putString(
        "collegeId",
        collegeId
    );

    preferences.putString(
        "departmentId",
        departmentId
    );

    preferences.putString(
        "deviceSecret",
        deviceSecret
    );

    preferences.putString(
        "mqttUser",
        mqttUser
    );

    preferences.putString(
        "mqttPassword",
        mqttPassword
    );

    preferences.putBool(
        "provisioned",
        true
    );

    http.end();

    return true;
}else {

    Serial.print(
        "Provision Failed. Code: "
    );

    Serial.println(
        responseCode
    );
}

  http.end();

  return false;
}


// ======================
// SETUP
// ======================
void setup() {

  Serial.begin(
      115200
  );

  preferences.begin(
      "venthen",
      false
  );

  bool configured =
      preferences.getBool(
          "wifi_done",
          false
      );

  if (!configured) {

    Serial.println(
        "No WiFi configured."
    );

    startAP();

    startServer();
  }
  else {

    bool connected =
        connectWiFi();

    if (!connected) {

      startAP();

      startServer();
    }
    else {

      bool provisioned =
          preferences.getBool(
              "provisioned",
              false
          );

      if (!provisioned) {

        Serial.println(
            "Provisioning..."
        );

        bool success =
            provisionDevice();

        if (success) {

          Serial.println(
              "Provision Success"
          );

          Serial.println(
              preferences.getString(
                  "collegeId"
              )
          );

          Serial.println(
              preferences.getString(
                  "departmentId"
              )
          );
        }
        else {

          Serial.println(
              "Provision Failed"
          );
        }
      }

      startServer();
    }
  }
}


// ======================
// LOOP
// ======================
void loop() {
  server.handleClient();
}