#include <vector>

struct AttendanceJob {
  int studentId;
};

std::vector<AttendanceJob> queue;

void heartbeatTask(void *pvParameters) {
  while (true) {
    Serial.println(
      "[Heartbeat] Sent");

    vTaskDelay(
      pdMS_TO_TICKS(10000));
  }
}

void serialTask(void *pvParameters) {
  while (true) {
    if (Serial.available()) {
      String input =
        Serial.readStringUntil('\n');

      input.trim();

      int studentId =
        input.toInt();

      if (studentId > 0) {
        AttendanceJob job;

        job.studentId =
          studentId;

        queue.push_back(job);

        Serial.print(
          "[Serial] Queued ");

        Serial.println(
          studentId);
      }
    }

    vTaskDelay(
      pdMS_TO_TICKS(100));
  }
}

void queueWorkerTask(
  void *pvParameters) {
  while (true) {
    if (!queue.empty()) {
      AttendanceJob job =
        queue.front();

      queue.erase(
        queue.begin());

      Serial.print(
        "[Worker] Processing ");

      Serial.println(
        job.studentId);
    }

    vTaskDelay(
      pdMS_TO_TICKS(1000));
  }
}

void setup() {
  Serial.begin(115200);

  xTaskCreate(
    heartbeatTask,
    "Heartbeat",
    4096,
    NULL,
    1,
    NULL);

  xTaskCreate(
    serialTask,
    "Serial",
    4096,
    NULL,
    1,
    NULL);

  xTaskCreate(
    queueWorkerTask,
    "Worker",
    4096,
    NULL,
    1,
    NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}
