#ifndef DEVICE_SERVICE_H
#define DEVICE_SERVICE_H

#include <Arduino.h>

class DeviceService
{

public:
    static String getDeviceId();

    static String getFirmware();

    static String getChipId();
};

#endif