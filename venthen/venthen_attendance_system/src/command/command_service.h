#ifndef COMMAND_SERVICE_H
#define COMMAND_SERVICE_H

#include <Arduino.h>

class CommandService
{

public:
    static void handle(
        const String &payload);
};

#endif