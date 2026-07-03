#include <string.h>


struct Ack
{

    int version = 1;

    String commandId;

    String deviceId;

    String status;

    String message;

    String timestamp;
};