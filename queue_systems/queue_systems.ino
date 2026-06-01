// #include <WiFi.h>
// #include <vector>

// /*
// |--------------------------------------------------------------------------
// | WIFI CONFIG
// |--------------------------------------------------------------------------
// */

// const char* WIFI_SSID = "Airtel_Sabita";
// const char* WIFI_PASSWORD = "Kishan@2005";

// /*
// |--------------------------------------------------------------------------
// | ATTENDANCE JOB
// |--------------------------------------------------------------------------
// */

// struct AttendanceJob {
//   int studentId;
//   unsigned long timestamp;
// };

// /*
// |--------------------------------------------------------------------------
// | QUEUE
// |--------------------------------------------------------------------------
// */

// std::vector<AttendanceJob> queue;

// /*
// |--------------------------------------------------------------------------
// | WIFI STATE
// |--------------------------------------------------------------------------
// */

// bool wasConnected = false;

// /*
// |--------------------------------------------------------------------------
// | CONNECT WIFI
// |--------------------------------------------------------------------------
// */

// void connectWifi() {

//   WiFi.mode(WIFI_STA);

//   WiFi.begin(
//     WIFI_SSID,
//     WIFI_PASSWORD);

//   Serial.println();
//   Serial.println(
//     "Connecting WiFi...");
// }

// /*
// |--------------------------------------------------------------------------
// | WIFI MONITOR
// |--------------------------------------------------------------------------
// */

// void monitorWifi() {

//   bool connected =
//     WiFi.status() == WL_CONNECTED;

//   if (
//     connected && !wasConnected) {

//     Serial.println();
//     Serial.println(
//       "=================================");

//     Serial.println(
//       "WiFi Connected");

//     Serial.print(
//       "IP: ");

//     Serial.println(
//       WiFi.localIP());

//     Serial.println(
//       "=================================");
//   }

//   if (
//     !connected && wasConnected) {

//     Serial.println();
//     Serial.println(
//       "=================================");

//     Serial.println(
//       "WiFi Lost");

//     Serial.println(
//       "=================================");
//   }

//   wasConnected =
//     connected;
// }

// /*
// |--------------------------------------------------------------------------
// | ADD ATTENDANCE
// |--------------------------------------------------------------------------
// */

// void addAttendance(
//   int studentId) {

//   AttendanceJob job;

//   job.studentId =
//     studentId;

//   job.timestamp =
//     millis();

//   queue.push_back(job);

//   Serial.println();
//   Serial.println(
//     "Attendance Queued");

//   Serial.print(
//     "Student ID: ");

//   Serial.println(
//     studentId);

//   Serial.print(
//     "Queue Size: ");

//   Serial.println(
//     queue.size());
// }

// /*
// |--------------------------------------------------------------------------
// | VPS SIMULATION
// |--------------------------------------------------------------------------
// */

// bool sendToServer(
//   AttendanceJob job) {

//   /*
//    * Later:
//    *
//    * HTTP POST
//    * to VPS
//    */

//   Serial.println();

//   Serial.print(
//     "Syncing Student ");

//   Serial.println(
//     job.studentId);

//   delay(500);

//   Serial.println(
//     "Success");

//   return true;
// }

// /*
// |--------------------------------------------------------------------------
// | PROCESS QUEUE
// |--------------------------------------------------------------------------
// */

// void processQueue() {

//   if (
//     WiFi.status() != WL_CONNECTED) {
//     return;
//   }

//   if (
//     queue.empty()) {
//     return;
//   }

//   AttendanceJob job =
//     queue.front();

//   bool success =
//     sendToServer(job);

//   if (
//     success) {

//     queue.erase(
//       queue.begin());

//     Serial.print(
//       "Remaining Jobs: ");

//     Serial.println(
//       queue.size());
//   }
// }

// /*
// |--------------------------------------------------------------------------
// | SERIAL INPUT
// |--------------------------------------------------------------------------
// */

// void handleSerialInput() {

//   if (
//     !Serial.available()) {
//     return;
//   }

//   String input =
//     Serial.readStringUntil(
//       '\n');

//   input.trim();

//   if (
//     input.length() == 0) {
//     return;
//   }

//   int studentId =
//     input.toInt();

//   if (
//     studentId <= 0) {

//     Serial.println(
//       "Invalid Student ID");

//     return;
//   }

//   addAttendance(
//     studentId);
// }

// /*
// |--------------------------------------------------------------------------
// | WIFI RECONNECT
// |--------------------------------------------------------------------------
// */

// unsigned long
//   lastReconnectAttempt = 0;

// void reconnectWifi() {

//   if (
//     WiFi.status() == WL_CONNECTED) {
//     return;
//   }

//   if (
//     millis() - lastReconnectAttempt < 5000) {
//     return;
//   }

//   lastReconnectAttempt =
//     millis();

//   Serial.println(
//     "Trying Reconnect...");

//   WiFi.disconnect();

//   WiFi.begin(
//     WIFI_SSID,
//     WIFI_PASSWORD);
// }

// /*
// |--------------------------------------------------------------------------
// | SETUP
// |--------------------------------------------------------------------------
// */

// void setup() {

//   Serial.begin(115200);

//   connectWifi();

//   Serial.println();
//   Serial.println(
//     "Enter Student IDs");

//   Serial.println(
//     "Example:");

//   Serial.println(
//     "101");

//   Serial.println(
//     "102");
// }

// /*
// |--------------------------------------------------------------------------
// | LOOP
// |--------------------------------------------------------------------------
// */

// void loop() {

//   monitorWifi();

//   reconnectWifi();

//   handleSerialInput();

//   processQueue();
// }










/// heart beat