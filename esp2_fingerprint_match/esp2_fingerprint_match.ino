#include <Adafruit_Fingerprint.h>

#define FINGERPRINT_RX 16
#define FINGERPRINT_TX 17

HardwareSerial mySerial(2);
Adafruit_Fingerprint finger(&mySerial);

void setup() {

  Serial.begin(9600);
  delay(2000);

  mySerial.begin(
    57600,
    SERIAL_8N1,
    FINGERPRINT_RX,
    FINGERPRINT_TX
  );
//   mySerial.begin(
//   115200,
//   SERIAL_8N1,
//   16,
//   17
// );

  if (!finger.verifyPassword()) {

    Serial.println("Sensor Not Found");

    while (true);
  }

  Serial.println("Ready For Matching");
}

void loop() {

  int result = getFingerprintID();

  if (result > 0) {

    Serial.print("Matched ID: ");

    Serial.println(result);

    delay(2000);
  }
}

int getFingerprintID() {

  uint8_t p =
    finger.getImage();

  if (p != FINGERPRINT_OK)
    return -1;

  p =
    finger.image2Tz();

  if (p != FINGERPRINT_OK)
    return -1;

  p =
    finger.fingerSearch();

  if (p != FINGERPRINT_OK) {

    Serial.println("No Match");

    return -1;
  }

  Serial.print("Confidence: ");

  Serial.println(
    finger.confidence
  );

  return finger.fingerID;
}