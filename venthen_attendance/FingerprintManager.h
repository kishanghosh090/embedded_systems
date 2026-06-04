#pragma once

class FingerprintManager {

public:

  bool begin();

  int match();

  bool enroll(int id);

  bool remove(int id);
};