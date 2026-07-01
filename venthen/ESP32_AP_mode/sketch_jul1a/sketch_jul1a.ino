#include <WiFi.h>
#include <Preferences.h>

Preferences preferences;

String deviceId = "VTH001";

void startAP() {
  String ssid = "Venthen-" + deviceId;
  String password = "12345678";

  WiFi.mode(WIFI_AP);

  bool result = WiFi.softAP(
      ssid.c_str(),
      password.c_str()
  );

  if (result) {
    Serial.println("AP Started");

    Serial.print("SSID: ");
    Serial.println(ssid);

    Serial.print("Password: ");
    Serial.println(password);

    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("AP Failed");
  }
}

void setup() {
  Serial.begin(115200);

  preferences.begin("venthen", false);

  bool wifiConfigured =
      preferences.getBool(
          "wifi_done",
          false
      );

  if (!wifiConfigured) {
    Serial.println(
        "No WiFi configured."
    );

    startAP();
  }
  else {
    Serial.println(
        "WiFi already configured."
    );
  }
}

void loop() {

}


