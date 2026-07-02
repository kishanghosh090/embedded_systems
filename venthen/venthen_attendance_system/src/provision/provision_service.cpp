#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "../../include/constants.h"

#include "../storage/storage.h"

#include "provision_service.h"
#include "../device/device_service.h"

bool ProvisionService::provision()
{

    HTTPClient http;

    http.begin(
        BACKEND_URL);

    http.addHeader(
        "Content-Type",
        "application/json");

    JsonDocument doc;

    doc["deviceId"] =
        DeviceService::getDeviceId();

    doc["firmware"] =
        DeviceService::getFirmware();

    doc["jwt"] =
        Storage::getJWT();

    String body;

    serializeJson(
        doc,
        body);

    int responseCode =
        http.POST(body);

    Serial.print(
        "Response Code: ");

    Serial.println(
        responseCode);

    if (
        responseCode == 200)
    {

        String response =
            http.getString();

        JsonDocument res;

        deserializeJson(
            res,
            response);

        Storage::saveProvision(

            res["registeredBy"]
                .as<String>(),

            res["collegeId"]
                .as<String>(),

            res["departmentId"]
                .as<String>(),

            res["deviceSecret"]
                .as<String>(),

            res["mqttUser"]
                .as<String>(),

            res["mqttPassword"]
                .as<String>());

        http.end();

        return true;
    }

    http.end();

    return false;
}