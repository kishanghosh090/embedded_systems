#include "DeviceStateManager.h"

void DeviceStateManager::begin(
  StorageManager* storageManager) {
  storage = storageManager;

  state.mode =
    (DeviceMode)
      storage->getMode();

  state.studentId =
    storage->getStudentId();
}

void DeviceStateManager::setMode(
  DeviceMode mode) {
  state.mode = mode;

  storage->saveMode(
    (int)mode);
}

DeviceMode DeviceStateManager::getMode() {
  return state.mode;
}

void DeviceStateManager::setStudentId(
  int studentId) {
  state.studentId =
    studentId;

  storage->saveStudentId(
    studentId);
}

int DeviceStateManager::getStudentId() {
  return state.studentId;
}

DeviceState
DeviceStateManager::getState() {
  return state;
}