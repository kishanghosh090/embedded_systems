#pragma once

#include "Models.h"
#include "StorageManager.h"

class DeviceStateManager {

private:

  DeviceState state;

  StorageManager* storage;

public:

  void begin(
    StorageManager* storageManager);

  void setMode(
    DeviceMode mode);

  DeviceMode getMode();

  void setStudentId(
    int studentId);

  int getStudentId();

  DeviceState getState();
};