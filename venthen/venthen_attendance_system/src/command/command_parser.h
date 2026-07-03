#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "command_model.h"

class CommandParser
{

public:
    static bool parse(
        const String &json,
        Command &command);
};

#endif