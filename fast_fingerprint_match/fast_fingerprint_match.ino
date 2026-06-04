#include <Adafruit_Fingerprint.h>

#define FINGERPRINT_RX 16
#define FINGERPRINT_TX 17

HardwareSerial mySerial(2);
Adafruit_Fingerprint finger(&mySerial);

void setup() {

  Serial.begin(115200);
  delay(2000);

  mySerial.begin(
    57600,
    SERIAL_8N1,
    FINGERPRINT_RX,
    FINGERPRINT_TX);

  Serial.println("Checking sensor...");

  if (!finger.verifyPassword()) {

    Serial.println("Sensor Not Found");

    while (true)
      ;
  }

  Serial.println("Sensor Ready");
  Serial.println("Place Finger");
}

void loop() {

  int id = matchFingerprint();

  if (id > 0) {

    Serial.println();
    Serial.print("Matched ID: ");
    Serial.println(id);

    Serial.print("Confidence: ");
    Serial.println(
      finger.confidence);

    Serial.println();
    Serial.println("Place Finger");
  }

  delay(50);
}

int matchFingerprint() {

  // uint8_t p;

  // p = finger.getImage();

  // if (p != FINGERPRINT_OK)
  //   return -1;

  // p = finger.image2Tz();

  // if (p != FINGERPRINT_OK)
  //   return -1;

  // p = finger.fingerFastSearch();

  // if (p != FINGERPRINT_OK)
  //   return -1;

  // return finger.fingerID;


  unsigned long start;

  start = millis();

  uint8_t p = finger.getImage();

  Serial.print("getImage: ");
  Serial.print(millis() - start);
  Serial.println(" ms");

  if (p != FINGERPRINT_OK) return -1;

  start = millis();

  p = finger.image2Tz();

  Serial.print("image2Tz: ");
  Serial.print(millis() - start);
  Serial.println(" ms");

  if (p != FINGERPRINT_OK)
    return -1;

  start = millis();

  p = finger.fingerFastSearch();

  Serial.print("search: ");
  Serial.print(millis() - start);
  Serial.println(" ms");

  if (p == FINGERPRINT_OK) {
    Serial.print("ID: ");
    Serial.println(
      finger.fingerID);
  }
}