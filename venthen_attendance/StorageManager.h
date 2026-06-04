#pragma once

#include <Preferences.h>

class StorageManager {

private:

  Preferences preferences;

public:

  bool begin();

  void saveMode(int mode);

  int getMode();

  void saveStudentId(int studentId);

  int getStudentId();
};