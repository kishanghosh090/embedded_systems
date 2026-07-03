#ifndef ATTENDANCE_COMMAND_H
#define ATTENDANCE_COMMAND_H

#include <Arduino.h>

class AttendanceCommand
{

public:
    static void handle(
        const String &action,
        const String &payload);
};

#endif