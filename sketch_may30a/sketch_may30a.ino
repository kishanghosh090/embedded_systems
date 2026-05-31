// #include <Adafruit_Fingerprint.h>
// #include <SoftwareSerial.h>

// SoftwareSerial mySerial(2, 3);
// Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// uint8_t id = 1; // Store as ID 1

// void setup() {
//   Serial.begin(115200);

//   finger.begin(57600);

//   if (!finger.verifyPassword()) {
//     Serial.println("Sensor NOT Found!");
//     while (1);
//   }

//   Serial.println("Place finger to enroll...");
// }

// void loop() {

//   if (getFingerprintEnroll()) {
//     Serial.println("Enrollment Success!");
//     while (1);
//   }
// }

// uint8_t getFingerprintEnroll() {

//   int p = -1;

//   Serial.println("Place finger");

//   while (p != FINGERPRINT_OK) {
//     p = finger.getImage();
//   }

//   p = finger.image2Tz(1);

//   if (p != FINGERPRINT_OK) return false;

//   Serial.println("Remove finger");

//   delay(2000);

//   while (finger.getImage() != FINGERPRINT_NOFINGER);

//   Serial.println("Place same finger again");

//   p = -1;

//   while (p != FINGERPRINT_OK) {
//     p = finger.getImage();
//   }

//   p = finger.image2Tz(2);

//   if (p != FINGERPRINT_OK) return false;

//   p = finger.createModel();

//   if (p != FINGERPRINT_OK) {
//     Serial.println("Fingerprints don't match");
//     return false;
//   }

//   p = finger.storeModel(id);

//   if (p == FINGERPRINT_OK) {
//     Serial.print("Stored as ID ");
//     Serial.println(id);
//     return true;
//   }

//   return false;
// }

/// -------------------------
// #include <Adafruit_Fingerprint.h>
// #include <SoftwareSerial.h>

// SoftwareSerial mySerial(2, 3);
// Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// void setup() {

//   Serial.begin(115200);

//   finger.begin(57600);

//   if (!finger.verifyPassword()) {
//     Serial.println("Sensor NOT Found!");
//     while (1);
//   }

//   Serial.println("Ready for matching...");
// }

// void loop() {

//   int p = finger.getImage();

//   if (p != FINGERPRINT_OK)
//     return;

//   p = finger.image2Tz();

//   if (p != FINGERPRINT_OK)
//     return;

//   p = finger.fingerFastSearch();

//   if (p == FINGERPRINT_OK) {

//     Serial.print("Match Found! ID: ");
//     Serial.println(finger.fingerID);

//     Serial.print("Confidence: ");
//     Serial.println(finger.confidence);

//   } else {

//     Serial.println("No Match Found");

//   }

//   delay(1500);
// }
