#ifndef COMMAND_DISPATCHER_H
#define COMMAND_DISPATCHER_H

#include "command_model.h"

class CommandDispatcher
{

public:
    static void dispatch(
        const Command &command);
};

#endif