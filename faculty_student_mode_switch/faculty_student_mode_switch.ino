#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

/*
|--------------------------------------------------------------------------
| WIFI CONFIG
|--------------------------------------------------------------------------
*/

const char* WIFI_SSID = "Airtel_Sabita";
const char* WIFI_PASSWORD = "Kishan@2005";

/*
|--------------------------------------------------------------------------
| GLOBAL OBJECTS
|--------------------------------------------------------------------------
*/

WebServer server(80);
Preferences preferences;

/*
|--------------------------------------------------------------------------
| DEVICE MODES
|--------------------------------------------------------------------------
*/

enum DeviceMode {
  IDLE,
  ENROLL,
  ATTENDANCE
};

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
| STORAGE HELPERS
|--------------------------------------------------------------------------
*/

/*
 * Save current mode and studentId
 * Data survives reboot
 */
void saveState(String mode, int studentId) {

  preferences.begin("device", false);

  preferences.putString("mode", mode);
  preferences.putInt("studentId", studentId);

  preferences.end();

  Serial.println("State saved");
}

/*
 * Read current state
 */
DeviceState getState() {

  DeviceState state;

  preferences.begin("device", true);

  state.mode = preferences.getString("mode", "IDLE");
  state.studentId = preferences.getInt("studentId", -1);

  preferences.end();

  return state;
}

/*
|--------------------------------------------------------------------------
| BUSINESS LOGIC
|--------------------------------------------------------------------------
*/

/*
 * Enter Enrollment Mode
 */
void startEnrollment(int studentId) {

  saveState("ENROLL", studentId);

  Serial.println();
  Serial.println("==================================");
  Serial.println("DEVICE MODE : ENROLL");
  Serial.print("STUDENT ID  : ");
  Serial.println(studentId);
  Serial.println("==================================");
}

/*
 * Enter Attendance Mode
 */
void startAttendance() {

  saveState("ATTENDANCE", -1);

  Serial.println();
  Serial.println("==================================");
  Serial.println("DEVICE MODE : ATTENDANCE");
  Serial.println("==================================");
}

/*
 * Stop Device
 */
void stopDevice() {

  saveState("IDLE", -1);

  Serial.println();
  Serial.println("==================================");
  Serial.println("DEVICE MODE : IDLE");
  Serial.println("==================================");
}

/*
|--------------------------------------------------------------------------
| HTTP ROUTES
|--------------------------------------------------------------------------
*/

/*
 * POST /mode?mode=ENROLL&studentId=123
 *
 * Examples:
 *
 * /mode?mode=ENROLL&studentId=123
 * /mode?mode=ATTENDANCE
 * /mode?mode=IDLE
 */
void handleModeChange() {

  if (!server.hasArg("mode")) {

    server.send(
      400,
      "application/json",
      "{\"success\":false,\"message\":\"mode required\"}");

    return;
  }

  String mode = server.arg("mode");

  mode.toUpperCase();

  if (mode == "ENROLL") {

    if (!server.hasArg("studentId")) {

      server.send(
        400,
        "application/json",
        "{\"success\":false,\"message\":\"studentId required\"}");

      return;
    }

    int studentId =
      server.arg("studentId").toInt();

    startEnrollment(studentId);
  }

  else if (mode == "ATTENDANCE") {

    startAttendance();
  }

  else if (mode == "IDLE") {

    stopDevice();
  }

  else {

    server.send(
      400,
      "application/json",
      "{\"success\":false,\"message\":\"invalid mode\"}");

    return;
  }

  server.send(
    200,
    "application/json",
    "{\"success\":true}");
}

/*
 * GET /status
 */
void handleStatus() {

  DeviceState state =
    getState();

  String response = "{";
  response += "\"mode\":\"" + state.mode + "\",";
  response += "\"studentId\":" + String(state.studentId);
  response += "}";

  server.send(
    200,
    "application/json",
    response);
}

/*
|--------------------------------------------------------------------------
| SIMULATED FINGERPRINT SCAN
|--------------------------------------------------------------------------
|
| Later replace with R307S code
|
*/

void simulateFingerprintScan() {

  DeviceState state =
    getState();

  Serial.println();
  Serial.println("Fingerprint Detected");

  if (state.mode == "ENROLL") {

    Serial.print(
      "Enroll fingerprint for student: ");

    Serial.println(
      state.studentId);

    /*
      Future:

      fingerprint.enroll(
        state.studentId
      );
    */
  }

  else if (
    state.mode == "ATTENDANCE") {

    Serial.println(
      "Match fingerprint...");

    /*
      Future:

      fingerprint.match();
    */
  }

  else {

    Serial.println(
      "Device is IDLE");
  }
}

/*
|--------------------------------------------------------------------------
| WIFI
|--------------------------------------------------------------------------
*/

void connectWiFi() {

  WiFi.mode(WIFI_STA);

  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD);

  Serial.println();
  Serial.println("Connecting to WiFi");

  while (
    WiFi.status() != WL_CONNECTED) {

    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("WiFi Connected");

  Serial.print("IP Address: ");
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

  connectWiFi();

  server.on(
    "/mode",
    HTTP_POST,
    handleModeChange);

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

  /*
   * Simulate fingerprint scan
   */
  if (Serial.available()) {

    String command =
      Serial.readStringUntil('\n');

    command.trim();

    command.toUpperCase();

    if (command == "SCAN") {

      simulateFingerprintScan();
    }
  }
}