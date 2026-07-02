#include <WiFi.h>

#include "../../include/constants.h"
#include "../device/device_service.h"
#include "ap_manager.h"

void APManager::begin() {

    String ssid =
        "Venthen-" +
        DeviceService::getDeviceId();
    WiFi.mode(WIFI_AP);

    WiFi.softAP(
        ssid.c_str(),
        AP_PASSWORD
    );

    Serial.println(
        WiFi.softAPIP()
    );
}