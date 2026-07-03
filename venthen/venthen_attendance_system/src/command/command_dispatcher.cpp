#include "command_dispatcher.h"

void CommandDispatcher::dispatch(
    const Command &command)
{

    Serial.println();
    Serial.println("========== COMMAND ==========");

    Serial.print("Version: ");
    Serial.println(command.version);

    Serial.print("Command ID: ");
    Serial.println(command.commandId);

    Serial.print("Type: ");
    Serial.println(command.type);

    Serial.print("Action: ");
    Serial.println(command.action);

    if (command.type == "ATTENDANCE")
    {

        Serial.println();

        Serial.println("Attendance Payload");

        Serial.print("Session: ");
        Serial.println(command.attendance.attendanceSessionId);

        Serial.print("Course: ");
        Serial.println(command.attendance.courseId);

        Serial.print("Section: ");
        Serial.println(command.attendance.sectionId);

        Serial.print("Duration: ");
        Serial.println(command.attendance.durationMinutes);

        if (command.action == "START")
        {

            Serial.println("Start Attendance");
        }
        else if (command.action == "STOP")
        {

            Serial.println("Stop Attendance");
        }
    }

    Serial.println("==============================");
}