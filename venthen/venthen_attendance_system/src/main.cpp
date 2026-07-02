#include <Arduino.h>
#include "mqtt/mqtt_service.h"
#include "storage/storage.h"
#include "wifi/wifi_manager.h"
#include "ap/ap_manager.h"
#include "web/api_server.h"
#include "provision/provision_service.h"

void setup()
{

    Serial.begin(115200);

    Storage::begin();

    if (
        !Storage::isWifiConfigured())
    {
        Serial.println(
            "wifi not configured");
        APManager::begin();

        ApiServer::begin();
    }
    else
    {

        bool connected =
            WifiManager::connect();

        if (!connected)
        {

            APManager::begin();

            ApiServer::begin();
        }
        else
        {

            if (
                !Storage::isProvisioned())
            {

                Serial.println(
                    "Provisioning...");

                bool success =
                    ProvisionService::
                        provision();

                if (success)
                {

                    Serial.println(
                        "Provision Success");
                }
            }
            MQTTService::begin();
            ApiServer::begin();
        }
    }
}

void loop()
{

    ApiServer::handle();
    MQTTService::loop();
}