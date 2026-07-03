#include "mqtt_service.h"

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "../command/command_service.h"
#include "../../include/constants.h"

#include "../device/device_service.h"

WiFiClient wifiClient;

PubSubClient mqttClient(wifiClient);

String commandTopic =
    "venthen/device/" +
    DeviceService::getDeviceId() +
    "/command";

String statusTopic =
    "venthen/device/" +
    DeviceService::getDeviceId() +
    "/status";

void callback(
    char *topic,
    byte *payload,
    unsigned int length)
{

    String message;

    for (unsigned int i = 0; i < length; i++)
    {

        message += (char)payload[i];
    }

    Serial.println();
    Serial.print("Topic: ");
    Serial.println(topic);

    CommandService::handle(message);
}

void reconnect()
{

    while (
        !mqttClient.connected())
    {

        Serial.println(
            "Connecting MQTT...");

        if (
            mqttClient.connect(
                DeviceService::
                    getDeviceId()
                        .c_str()))
        {

            Serial.println(
                "MQTT Connected");

            mqttClient.subscribe(
                commandTopic.c_str());

            Serial.println(
                commandTopic);
        }
        else
        {

            Serial.println(
                "MQTT Failed");

            delay(3000);
        }
    }
}

void MQTTService::begin()
{

    mqttClient.setServer(
        MQTT_HOST,
        MQTT_PORT);

    mqttClient.setCallback(
        callback);

    reconnect();
}

void MQTTService::loop()
{

    if (
        !mqttClient.connected())
    {
        reconnect();
    }

    mqttClient.loop();
}

bool MQTTService::connected()
{

    return mqttClient.connected();
}

void MQTTService::publishStatus()
{

    JsonDocument doc;

    doc["deviceId"] =
        DeviceService::getDeviceId();

    doc["firmware"] =
        DeviceService::getFirmware();

    doc["ip"] =
        WiFi.localIP().toString();

    doc["online"] = true;

    String payload;

    serializeJson(
        doc,
        payload);

    mqttClient.publish(
        statusTopic.c_str(),
        payload.c_str());

    Serial.println(
        "Status Published");
}