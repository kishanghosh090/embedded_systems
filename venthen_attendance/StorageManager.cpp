#include "StorageManager.h"
#include "Config.h"

bool StorageManager::begin() {
  return preferences.begin(
    PREF_NAMESPACE,
    false);
}

void StorageManager::saveMode(
  int mode) {
  preferences.putInt(
    "mode",
    mode);
}

int StorageManager::getMode() {
  return preferences.getInt(
    "mode",
    0);
}

void StorageManager::saveStudentId(
  int studentId) {
  preferences.putInt(
    "studentId",
    studentId);
}

int StorageManager::getStudentId() {
  return preferences.getInt(
    "studentId",
    -1);
}