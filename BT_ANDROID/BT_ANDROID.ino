#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);

  SerialBT.begin("Venthen-ESP32");

  Serial.println("Bluetooth Started");
  Serial.println("Waiting for connection...");
}

void loop() {

  if (SerialBT.available()) {
  Serial.println(SerialBT.getBtAddressString());
    String message =
      SerialBT.readStringUntil('\n');

    Serial.print("Received: ");
    Serial.println(message);

    SerialBT.println(
      "ESP32 Received -> " + message
    );
  }
}