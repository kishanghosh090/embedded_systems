#include "Config.h"
#include "Models.h"

#include "StorageManager.h"
#include "DeviceStateManager.h"

StorageManager storage;

DeviceStateManager deviceState;

void printState() {
  DeviceState state =
    deviceState.getState();

  Serial.println();
  Serial.println(
    "===== STATE =====");

  Serial.print(
    "Mode: ");

  switch (state.mode) {
    case IDLE:
      Serial.println("IDLE");
      break;

    case ATTENDANCE:
      Serial.println("ATTENDANCE");
      break;

    case ENROLL:
      Serial.println("ENROLL");
      break;
  }

  Serial.print(
    "Student ID: ");

  Serial.println(
    state.studentId);

  Serial.println(
    "=================");
}

void setup() {
  Serial.begin(
    SERIAL_BAUD);

  delay(2000);

  storage.begin();

  deviceState.begin(
    &storage);

  Serial.println(
    "Venthen Device Started");

  printState();

  Serial.println();
  Serial.println(
    "Commands:");

  Serial.println(
    "idle");

  Serial.println(
    "attendance");

  Serial.println(
    "enroll 101");

  Serial.println(
    "state");
}

void loop() {
  if (!Serial.available()) {
    return;
  }

  String command =
    Serial.readStringUntil(
      '\n');

  command.trim();

  if (command == "idle") {
    deviceState.setMode(
      IDLE);
  }

  else if (
    command == "attendance") {
    deviceState.setMode(
      ATTENDANCE);
  }

  else if (
    command.startsWith(
      "enroll ")) {
    int studentId =
      command.substring(7)
        .toInt();

    deviceState.setMode(
      ENROLL);

    deviceState.setStudentId(
      studentId);
  }

  else if (
    command == "state") {
  }

  printState();
}