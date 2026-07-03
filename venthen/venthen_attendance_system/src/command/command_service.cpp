#include "command_service.h"

#include "command_parser.h"
#include "command_dispatcher.h"

void CommandService::handle(
    const String &payload)
{

    Command command;

    bool ok =
        CommandParser::parse(
            payload,
            command);

    if (!ok)
    {

        return;
    }

    CommandDispatcher::dispatch(
        command);
}