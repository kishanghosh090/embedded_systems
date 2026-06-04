#include "FingerprintManager.h"
#include "Config.h"

#include <Adafruit_Fingerprint.h>

HardwareSerial fingerSerial(2);

Adafruit_Fingerprint finger(
  &fingerSerial);

bool FingerprintManager::begin() {
  fingerSerial.begin(
    FINGERPRINT_BAUD,
    SERIAL_8N1,
    FINGERPRINT_RX,
    FINGERPRINT_TX);

  return finger.verifyPassword();
}

int FingerprintManager::match() {
  uint8_t p;

  p = finger.getImage();

  if (
    p != FINGERPRINT_OK) {
    return -1;
  }

  p = finger.image2Tz();

  if (
    p != FINGERPRINT_OK) {
    return -1;
  }

  p = finger.fingerFastSearch();

  if (
    p != FINGERPRINT_OK) {
    return -1;
  }

  return finger.fingerID;
}

bool FingerprintManager::enroll(
  int id) {
  Serial.println(
    "Enrollment Coming Soon");

  return true;
}

bool FingerprintManager::remove(
  int id) {
  return finger.deleteModel(
           id)
         == FINGERPRINT_OK;
}