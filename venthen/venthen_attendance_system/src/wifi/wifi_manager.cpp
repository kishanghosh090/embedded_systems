#include <WiFi.h>

#include "../storage/storage.h"

#include "wifi_manager.h"

bool WifiManager::connect() {

    WiFi.mode(WIFI_STA);

    WiFi.begin(
        Storage::getSSID().c_str(),
        Storage::getPassword().c_str()
    );

    int retries = 0;

    while (
        WiFi.status() != WL_CONNECTED &&
        retries < 20
    ) {

        delay(500);

        Serial.print(".");

        retries++;
    }

    if (
        WiFi.status() ==
        WL_CONNECTED
    ) {

        Serial.println();

        Serial.println(
            WiFi.localIP()
        );

        return true;
    }

    return false;
}