#include <Adafruit_Fingerprint.h>

#define FINGERPRINT_RX 16 
#define FINGERPRINT_TX 17 

HardwareSerial mySerial(2); 
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  // Wait longer for USB Serial to initialize
  Serial.begin(9600);
  delay(2000); 
  
  Serial.println(">>> SYSTEM STARTED <<<");
  Serial.println("Initializing UART2...");

  // Initialize Sensor UART
  mySerial.begin(57600, SERIAL_8N1, FINGERPRINT_RX, FINGERPRINT_TX);
  delay(100);

  Serial.println("Checking sensor...");
  
  if (finger.verifyPassword()) {
    Serial.println("✓ SUCCESS: Sensor Found!");
  } else {
    Serial.println("✗ FAILURE: Sensor Not Found.");
    Serial.println("   - Check TX/RX wiring (Crossed?)");
    Serial.println("   - Check 5V Power stability");
    Serial.println("   - Try Baud Rate 9600 instead of 57600");
  }
}

void loop() {
  // Empty loop to keep connection alive
  delay(1000);
  Serial.println("Alive...");
}


// RX2 esp32 ---> yellow wire Tx (R307s)
// TX2 esp32 ---> green wire Rx (R307s)