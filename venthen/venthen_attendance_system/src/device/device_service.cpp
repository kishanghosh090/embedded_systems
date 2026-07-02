#include <Arduino.h>

#include "../../include/constants.h"

#include "device_service.h"

String DeviceService::getDeviceId()
{

    uint64_t chipId =
        ESP.getEfuseMac();

    uint16_t shortId =
        (chipId >> 32);

    return "VTH-" +
           String(
               shortId,
               HEX);
}

String DeviceService::getFirmware()
{

    return FIRMWARE_VERSION;
}

String DeviceService::getChipId()
{

    uint64_t chipId =
        ESP.getEfuseMac();

    return String(
               (uint32_t)(chipId >> 32),
               HEX) +
           String(
               (uint32_t)chipId,
               HEX);
}