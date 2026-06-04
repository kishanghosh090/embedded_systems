#include <WiFi.h>

/*
|--------------------------------------------------------------------------
| WIFI
|--------------------------------------------------------------------------
*/

const char *WIFI_SSID = "YOUR_WIFI";
const char *WIFI_PASSWORD = "YOUR_PASSWORD";

/*
|--------------------------------------------------------------------------
| TCP SERVER
|--------------------------------------------------------------------------
*/

WiFiServer server(5000);

/*
|--------------------------------------------------------------------------
| ATTENDANCE JOB
|--------------------------------------------------------------------------
*/

struct AttendanceJob {

  int studentId;
};

/*
|--------------------------------------------------------------------------
| FREERTOS QUEUE
|--------------------------------------------------------------------------
*/

QueueHandle_t attendanceQueue;

/*
|--------------------------------------------------------------------------
| TCP SHELL TASK
|--------------------------------------------------------------------------
*/

void tcpTask(void *parameter) {
  while (true) {
    WiFiClient client =
      server.available();

    if (client) {
      Serial.println(
        "Client Connected");

      client.println(
        "Venthen Device");

      client.println(
        "Commands:");

      client.println(
        "add <studentId>");

      while (client.connected()) {
        if (client.available()) {
          String command =
            client.readStringUntil('\n');

          command.trim();

          /*
           * add 101
           */

          if (
            command.startsWith("add ")) {
            int studentId =
              command.substring(4)
                .toInt();

            AttendanceJob job;

            job.studentId =
              studentId;

            BaseType_t result =
              xQueueSend(
                attendanceQueue,
                &job,
                0);

            if (
              result == pdTRUE) {
              client.print(
                "Queued Student ");

              client.println(
                studentId);

              Serial.print(
                "[TCP] Queued ");

              Serial.println(
                studentId);
            } else {
              client.println(
                "Queue Full");
            }
          }

          else {
            client.println(
              "Unknown Command");
          }
        }

        vTaskDelay(
          pdMS_TO_TICKS(100));
      }

      Serial.println(
        "Client Disconnected");

      client.stop();
    }

    vTaskDelay(
      pdMS_TO_TICKS(100));
  }
}

/*
|--------------------------------------------------------------------------
| WORKER TASK
|--------------------------------------------------------------------------
*/

void workerTask(void *parameter) {
  AttendanceJob job;

  while (true) {
    /*
     * Wait forever
     * until a job arrives
     */

    if (
      xQueueReceive(
        attendanceQueue,
        &job,
        portMAX_DELAY)
      == pdTRUE) {
      Serial.print(
        "[WORKER] Processing Student ");

      Serial.println(
        job.studentId);

      /*
       * Simulate fingerprint sync
       */

      vTaskDelay(
        pdMS_TO_TICKS(2000));

      Serial.print(
        "[WORKER] Completed ");

      Serial.println(
        job.studentId);
    }
  }
}

/*
|--------------------------------------------------------------------------
| HEARTBEAT TASK
|--------------------------------------------------------------------------
*/

void heartbeatTask(
  void *parameter) {
  while (true) {
    Serial.println(
      "[Heartbeat] Alive");

    vTaskDelay(
      pdMS_TO_TICKS(10000));
  }
}

/*
|--------------------------------------------------------------------------
| SETUP
|--------------------------------------------------------------------------
*/

void setup() {
  Serial.begin(115200);

  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD);

  Serial.print(
    "Connecting WiFi");

  while (
    WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();

  Serial.print(
    "IP: ");

  Serial.println(
    WiFi.localIP());

  server.begin();

  /*
   * Create Queue
   *
   * Capacity = 10 jobs
   */

  attendanceQueue =
    xQueueCreate(
      10,
      sizeof(
        AttendanceJob));

  /*
   * TCP Task
   */

  xTaskCreatePinnedToCore(
    tcpTask,
    "TCP",
    4096,
    NULL,
    1,
    NULL,
    1);

  /*
   * Worker Task
   */

  xTaskCreatePinnedToCore(
    workerTask,
    "Worker",
    4096,
    NULL,
    1,
    NULL,
    1);

  /*
   * Heartbeat
   */

  xTaskCreatePinnedToCore(
    heartbeatTask,
    "Heartbeat",
    2048,
    NULL,
    1,
    NULL,
    0);
}

/*
|--------------------------------------------------------------------------
| LOOP
|--------------------------------------------------------------------------
*/

void loop() {
}