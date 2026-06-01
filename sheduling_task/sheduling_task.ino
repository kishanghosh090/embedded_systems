// 192.168.1.8
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <vector>

/*
|--------------------------------------------------------------------------
| WIFI CONFIG
|--------------------------------------------------------------------------
*/


const char* WIFI_SSID = "Airtel_Sabita";
const char* WIFI_PASSWORD = "Kishan@2005";

/*
|--------------------------------------------------------------------------
| BACKEND
|--------------------------------------------------------------------------
*/

const char* BACKEND_URL =
  "http://192.168.1.8:3000";

/*
|--------------------------------------------------------------------------
| DEVICE
|--------------------------------------------------------------------------
*/

const char* DEVICE_ID =
  "CSE_LAB_1";

/*
|--------------------------------------------------------------------------
| HTTP SERVER
|--------------------------------------------------------------------------
*/

WebServer server(80);

/*
|--------------------------------------------------------------------------
| ATTENDANCE QUEUE
|--------------------------------------------------------------------------
*/

struct AttendanceJob {

  int studentId;

  unsigned long timestamp;
};

std::vector<AttendanceJob>
  attendanceQueue;

/*
|--------------------------------------------------------------------------
| HEARTBEAT SCHEDULER
|--------------------------------------------------------------------------
*/

unsigned long
  lastHeartbeat = 0;

const unsigned long
  HEARTBEAT_INTERVAL =
    10000;

/*
|--------------------------------------------------------------------------
| WIFI RECONNECT
|--------------------------------------------------------------------------
*/

unsigned long
  lastReconnectAttempt = 0;

const unsigned long
  RECONNECT_INTERVAL =
    5000;

/*
|--------------------------------------------------------------------------
| WIFI STATE
|--------------------------------------------------------------------------
*/

bool wasConnected = false;

/*
|--------------------------------------------------------------------------
| CONNECT WIFI
|--------------------------------------------------------------------------
*/

void connectWifi() {

  WiFi.mode(WIFI_STA);

  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD);

  Serial.println(
    "Connecting WiFi...");
}

/*
|--------------------------------------------------------------------------
| WIFI MONITOR
|--------------------------------------------------------------------------
*/

void monitorWifi() {

  bool connected =
    WiFi.status() == WL_CONNECTED;

  if (
    connected && !wasConnected) {

    Serial.println();
    Serial.println(
      "WiFi Connected");

    Serial.print(
      "IP: ");

    Serial.println(
      WiFi.localIP());
  }

  if (
    !connected && wasConnected) {

    Serial.println();
    Serial.println(
      "WiFi Lost");
  }

  wasConnected =
    connected;
}

/*
|--------------------------------------------------------------------------
| WIFI RECONNECT TASK
|--------------------------------------------------------------------------
*/

void reconnectWifiTask() {

  if (
    WiFi.status() == WL_CONNECTED) {
    return;
  }

  if (
    millis() - lastReconnectAttempt < RECONNECT_INTERVAL) {
    return;
  }

  lastReconnectAttempt =
    millis();

  Serial.println(
    "Trying Reconnect...");

  WiFi.disconnect();

  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD);
}

/*
|--------------------------------------------------------------------------
| HEARTBEAT HTTP
|--------------------------------------------------------------------------
*/

void sendHeartbeat() {

  if (
    WiFi.status() != WL_CONNECTED) {
    return;
  }

  HTTPClient http;

  String url =
    String(BACKEND_URL) + "/heartbeat";

  http.begin(url);

  http.addHeader(
    "Content-Type",
    "application/json");

  String body =
    "{"
    "\"deviceId\":\""
    + String(DEVICE_ID) + "\""
                          "}";

  int code =
    http.POST(body);

  Serial.println();
  Serial.println(
    "Heartbeat Sent");

  Serial.print(
    "Response: ");

  Serial.println(code);

  http.end();
}

/*
|--------------------------------------------------------------------------
| HEARTBEAT SCHEDULER
|--------------------------------------------------------------------------
*/

void heartbeatTask() {

  if (
    millis() - lastHeartbeat < HEARTBEAT_INTERVAL) {
    return;
  }

  lastHeartbeat =
    millis();

  sendHeartbeat();
}

/*
|--------------------------------------------------------------------------
| ATTENDANCE QUEUE
|--------------------------------------------------------------------------
*/

void addAttendance(
  int studentId) {

  AttendanceJob job;

  job.studentId =
    studentId;

  job.timestamp =
    millis();

  attendanceQueue.push_back(
    job);

  Serial.println();

  Serial.print(
    "Attendance Queued: ");

  Serial.println(
    studentId);

  Serial.print(
    "Queue Size: ");

  Serial.println(
    attendanceQueue.size());
}

/*
|--------------------------------------------------------------------------
| SEND ATTENDANCE
|--------------------------------------------------------------------------
*/

bool sendAttendance(
  AttendanceJob job) {

  if (
    WiFi.status() != WL_CONNECTED) {
    return false;
  }

  HTTPClient http;

  String url =
    String(BACKEND_URL) + "/attendance";

  http.begin(url);

  http.addHeader(
    "Content-Type",
    "application/json");

  String body =
    "{"
    "\"deviceId\":\""
    + String(DEVICE_ID) + "\","
                          "\"studentId\":"
    + String(job.studentId) + "}";

  int code =
    http.POST(body);

  http.end();

  return code == 200;
}

/*
|--------------------------------------------------------------------------
| QUEUE WORKER
|--------------------------------------------------------------------------
*/

void processQueueTask() {

  if (
    attendanceQueue.empty()) {
    return;
  }

  AttendanceJob job =
    attendanceQueue.front();

  bool success =
    sendAttendance(job);

  if (
    success) {

    Serial.println();

    Serial.print(
      "Synced Student: ");

    Serial.println(
      job.studentId);

    attendanceQueue.erase(
      attendanceQueue.begin());

    Serial.print(
      "Remaining: ");

    Serial.println(
      attendanceQueue.size());
  }
}

/*
|--------------------------------------------------------------------------
| SERIAL SIMULATOR
|--------------------------------------------------------------------------
*/

void serialTask() {

  if (
    !Serial.available()) {
    return;
  }

  String input =
    Serial.readStringUntil(
      '\n');

  input.trim();

  int studentId =
    input.toInt();

  if (
    studentId <= 0) {

    Serial.println(
      "Invalid Student");

    return;
  }

  addAttendance(
    studentId);
}

/*
|--------------------------------------------------------------------------
| HTTP SERVER
|--------------------------------------------------------------------------
*/

void handleStatus() {

  String response =
    "{";

  response +=
    "\"queueSize\":" + String(attendanceQueue.size());

  response += "}";

  server.send(
    200,
    "application/json",
    response);
}

void startServer() {

  server.on(
    "/status",
    HTTP_GET,
    handleStatus);

  server.begin();

  Serial.println(
    "HTTP Server Started");
}

/*
|--------------------------------------------------------------------------
| SETUP
|--------------------------------------------------------------------------
*/

void setup() {

  Serial.begin(115200);

  connectWifi();

  startServer();

  Serial.println();
  Serial.println(
    "Type Student ID");

  Serial.println(
    "Example:");

  Serial.println(
    "101");
}

/*
|--------------------------------------------------------------------------
| LOOP
|--------------------------------------------------------------------------
*/

void loop() {

  server.handleClient();

  monitorWifi();

  reconnectWifiTask();

  heartbeatTask();

  serialTask();

  processQueueTask();
}