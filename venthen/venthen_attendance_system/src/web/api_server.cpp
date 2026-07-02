#include "api_server.h"

#include <WebServer.h>
#include <ArduinoJson.h>

#include "../storage/storage.h"

WebServer server(80);

void handleStatus()
{

    JsonDocument doc;

    doc["configured"] =
        Storage::isWifiConfigured();

    doc["provisioned"] =
        Storage::isProvisioned();

    String response;

    serializeJson(
        doc,
        response);

    server.send(
        200,
        "application/json",
        response);
}

void handleInfo()
{

    JsonDocument doc;

    doc["deviceId"] =
        "VTH001";

    doc["firmware"] =
        "1.0.0";

    doc["wifiConfigured"] =
        Storage::isWifiConfigured();

    doc["provisioned"] =
        Storage::isProvisioned();

    doc["collegeId"] =
        Storage::getCollegeId();

    doc["departmentId"] =
        Storage::getDepartmentId();

    String response;

    serializeJson(
        doc,
        response);

    server.send(
        200,
        "application/json",
        response);
}

void handleWifi()
{

    if (!server.hasArg("plain"))
    {

        server.send(
            400,
            "application/json",
            "{\"success\":false}");

        return;
    }

    String body =
        server.arg("plain");

    JsonDocument doc;

    deserializeJson(
        doc,
        body);

    String ssid =
        doc["ssid"]
            .as<String>();

    String password =
        doc["password"]
            .as<String>();

    String jwt =
        doc["jwt"]
            .as<String>();

    Storage::setWifi(
        ssid,
        password,
        jwt);

    server.send(
        200,
        "application/json",
        "{\"success\":true}");

    delay(2000);

    ESP.restart();
}

void handleReset()
{

    Preferences preferences;

    preferences.begin(
        "venthen",
        false);

    preferences.clear();

    preferences.end();

    server.send(
        200,
        "application/json",
        "{\"success\":true}");

    delay(1000);

    ESP.restart();
}
void ApiServer::begin()
{

    server.on(
        "/status",
        HTTP_GET,
        handleStatus);

    server.on(
        "/info",
        HTTP_GET,
        handleInfo);

    server.on(
        "/wifi",
        HTTP_POST,
        handleWifi);
    server.on(
        "/reset",
        HTTP_POST,
        handleReset);
    server.begin();

    Serial.println(
        "HTTP Server Started");
}

void ApiServer::handle()
{

    server.handleClient();
}