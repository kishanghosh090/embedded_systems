#ifndef STORAGE_H
#define STORAGE_H

#include <Preferences.h>

class Storage {

public:

    static void begin();

    static bool isWifiConfigured();

    static bool isProvisioned();

    static void setWifi(
        String ssid,
        String password,
        String jwt
    );

    static String getSSID();

    static String getPassword();

    static String getJWT();

    static void saveProvision(
        String registeredBy,
        String collegeId,
        String departmentId,
        String deviceSecret,
        String mqttUser,
        String mqttPassword
    );

    static String getCollegeId();

    static String getDepartmentId();
};

#endif