#ifndef COMMAND_MODEL_H
#define COMMAND_MODEL_H

#include <Arduino.h>

struct AttendancePayload
{

    String attendanceSessionId;

    String courseId;

    String sectionId;

    int durationMinutes = 0;
};

struct Command
{

    int version = 1;

    String commandId;

    String type;

    String action;

    String issuedBy;

    String issuedAt;

    AttendancePayload attendance;
};

#endif