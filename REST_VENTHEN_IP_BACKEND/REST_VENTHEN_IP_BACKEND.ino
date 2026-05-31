#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <Preferences.h>
#define SERVICE_UUID "12345678-1234-1234-1234-123456789abc"
/*
|--------------------------------------------------------------------------
| WIFI CONFIG
|--------------------------------------------------------------------------
*/

const char* WIFI_SSID = "Airtel_Sabita";
const char* WIFI_PASSWORD = "Kishan@2005";

/*
|--------------------------------------------------------------------------
| BACKEND CONFIG
|--------------------------------------------------------------------------
|
| Replace with your VPS or local Express IP
|
*/

const char* BACKEND_URL =
  "http://192.168.1.8:3000";

/*
|--------------------------------------------------------------------------
| DEVICE CONFIG
|--------------------------------------------------------------------------
*/

const String DEVICE_ID =
  "CSE_LAB_1";

/*
|--------------------------------------------------------------------------
| GLOBALS
|--------------------------------------------------------------------------
*/

WebServer server(80);
Preferences prefs;

/*
|--------------------------------------------------------------------------
| DEVICE STATE
|--------------------------------------------------------------------------
*/

struct DeviceState {
  String mode;
  int studentId;
};

/*
|--------------------------------------------------------------------------
| STORAGE
|--------------------------------------------------------------------------
*/

void saveState(
  String mode,
  int studentId) {

  prefs.begin("device", false);

  prefs.putString(
    "mode",
    mode);

  prefs.putInt(
    "studentId",
    studentId);

  prefs.end();
}

DeviceState getState() {

  DeviceState state;

  prefs.begin("device", true);

  state.mode =
    prefs.getString(
      "mode",
      "IDLE");

  state.studentId =
    prefs.getInt(
      "studentId",
      -1);

  prefs.end();

  return state;
}

/*
|--------------------------------------------------------------------------
| REGISTER DEVICE
|--------------------------------------------------------------------------
*/

void registerDevice() {

  if (
    WiFi.status() != WL_CONNECTED) {
    return;
  }

  HTTPClient http;

  String url =
    String(BACKEND_URL)
    + "/api/devices/register";

  http.begin(url);

  http.addHeader(
    "Content-Type",
    "application/json");

  String body =
    "{"
    "\"deviceId\":\""
    + DEVICE_ID + "\","
                  "\"ip\":\""
    + WiFi.localIP().toString()
    + "\""
      "}";

  Serial.println();
  Serial.println(
    "Registering Device...");

  int code =
    http.POST(body);

  Serial.print(
    "Response Code: ");

  Serial.println(code);

  http.end();
}

/*
|--------------------------------------------------------------------------
| MODE API
|--------------------------------------------------------------------------
|
| POST
|
| /mode?mode=ENROLL&studentId=123
|
| /mode?mode=ATTENDANCE
|
*/

void handleMode() {

  if (
    !server.hasArg("mode")) {

    server.send(
      400,
      "application/json",
      "{\"message\":\"mode required\"}");

    return;
  }

  String mode =
    server.arg("mode");

  mode.toUpperCase();

  if (
    mode == "ENROLL") {

    if (
      !server.hasArg(
        "studentId")) {

      server.send(
        400,
        "application/json",
        "{\"message\":\"studentId required\"}");

      return;
    }

    int studentId =
      server.arg(
              "studentId")
        .toInt();

    saveState(
      "ENROLL",
      studentId);

    Serial.println();
    Serial.println(
      "===============");

    Serial.println(
      "MODE: ENROLL");

    Serial.print(
      "STUDENT: ");

    Serial.println(
      studentId);

    Serial.println(
      "===============");
  }

  else if (
    mode == "ATTENDANCE") {

    saveState(
      "ATTENDANCE",
      -1);

    Serial.println();
    Serial.println(
      "===============");

    Serial.println(
      "MODE: ATTENDANCE");

    Serial.println(
      "===============");
  }

  else {

    saveState(
      "IDLE",
      -1);

    Serial.println();
    Serial.println(
      "MODE: IDLE");
  }

  server.send(
    200,
    "application/json",
    "{\"success\":true}");
}

/*
|--------------------------------------------------------------------------
| STATUS API
|--------------------------------------------------------------------------
*/

void handleStatus() {

  DeviceState state =
    getState();

  String response =
    "{";

  response +=
    "\"deviceId\":\""
    + DEVICE_ID + "\",";

  response +=
    "\"mode\":\""
    + state.mode + "\",";

  response +=
    "\"studentId\":"
    + String(
      state.studentId);

  response += "}";

  server.send(
    200,
    "application/json",
    response);
}

/*
|--------------------------------------------------------------------------
| SIMULATED FINGERPRINT
|--------------------------------------------------------------------------
*/

void simulateScan() {

  DeviceState state =
    getState();

  Serial.println();
  Serial.println(
    "Fingerprint Detected");

  if (
    state.mode == "ENROLL") {

    Serial.print(
      "Enroll Student ");

    Serial.println(
      state.studentId);
  }

  else if (
    state.mode == "ATTENDANCE") {
    sendAttendance(1);
    Serial.println(
      "Match Fingerprint");
  }

  else {

    Serial.println(
      "Device Idle");
  }
}


void sendAttendance(
  int studentId) {

  HTTPClient http;

  String url =
    String(BACKEND_URL)
    + "/api/attendance";

  http.begin(url);

  http.addHeader(
    "Content-Type",
    "application/json");

  String body =
    "{"
    "\"deviceId\":\"CSE_LAB_1\","
    "\"studentId\":"
    + String(studentId)
    + "}";

  int code =
    http.POST(body);

  Serial.println(code);

  http.end();
}

/*
|--------------------------------------------------------------------------
| WIFI
|--------------------------------------------------------------------------
*/

void connectWifi() {

  WiFi.mode(
    WIFI_STA);

  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD);

  Serial.println(
    "Connecting WiFi...");

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
}

/*
|--------------------------------------------------------------------------
| SETUP
|--------------------------------------------------------------------------
*/

void setup() {

  Serial.begin(115200);

  connectWifi();

  registerDevice();

  server.on(
    "/mode",
    HTTP_POST,
    handleMode);

  server.on(
    "/status",
    HTTP_GET,
    handleStatus);

  server.begin();

  Serial.println();
  Serial.println(
    "HTTP Server Started");

  Serial.println(
    "Type SCAN in Serial Monitor");
}

/*
|--------------------------------------------------------------------------
| LOOP
|--------------------------------------------------------------------------
*/

void loop() {

  server.handleClient();

  if (
    Serial.available()) {

    String cmd =
      Serial.readStringUntil(
        '\n');

    cmd.trim();

    cmd.toUpperCase();

    if (
      cmd == "SCAN") {

      simulateScan();
    }
  }
}