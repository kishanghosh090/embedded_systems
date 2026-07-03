#include "command_parser.h"

#include <ArduinoJson.h>

bool CommandParser::parse(
    const String &json,
    Command &command)
{

    JsonDocument doc;

    DeserializationError error =
        deserializeJson(doc, json);

    if (error)
    {

        Serial.println("JSON Parse Failed");

        return false;
    }

    command.version =
        doc["version"] | 1;

    command.commandId =
        doc["commandId"] | "";

    command.type =
        doc["type"] | "";

    command.action =
        doc["action"] | "";

    command.issuedBy =
        doc["issuedBy"] | "";

    command.issuedAt =
        doc["issuedAt"] | "";

    JsonObject payload =
        doc["payload"];

    command.attendance.attendanceSessionId =
        payload["attendanceSessionId"] | "";

    command.attendance.courseId =
        payload["courseId"] | "";

    command.attendance.sectionId =
        payload["sectionId"] | "";

    command.attendance.durationMinutes =
        payload["durationMinutes"] | 0;

    return true;
}