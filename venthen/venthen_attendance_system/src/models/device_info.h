#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H

#include <Arduino.h>

struct DeviceInfo {

    String deviceId;

    String firmware;

    String registeredBy;

    String collegeId;

    String departmentId;

    String deviceSecret;

    String mqttUser;

    String mqttPassword;

    bool provisioned;
};

#endif