#include "attendance_command.h"

#include <ArduinoJson.h>

void AttendanceCommand::handle(
    const String &action,
    const String &payload)
{

    JsonDocument doc;

    if (deserializeJson(doc, payload))
    {

        Serial.println("Attendance payload invalid");

        return;
    }

    String attendanceSessionId =
        doc["attendanceSessionId"] | "";

    String courseId =
        doc["courseId"] | "";

    String sectionId =
        doc["sectionId"] | "";

    int duration =
        doc["durationMinutes"] | 0;

    Serial.println();
    Serial.println("Attendance Command");

    Serial.print("Course: ");
    Serial.println(courseId);

    Serial.print("Section: ");
    Serial.println(sectionId);

    Serial.print("Duration: ");
    Serial.println(duration);

    if (action == "START")
    {

        Serial.println("Attendance Started");
    }
    else if (action == "STOP")
    {

        Serial.println("Attendance Stopped");
    }
}