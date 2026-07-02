#include "storage.h"

Preferences preferences;

void Storage::begin() {

    preferences.begin(
        "venthen",
        false
    );
}

bool Storage::isWifiConfigured() {

    return preferences.getBool(
        "wifi_done",
        false
    );
}

bool Storage::isProvisioned() {

    return preferences.getBool(
        "provisioned",
        false
    );
}

void Storage::setWifi(
    String ssid,
    String password,
    String jwt
) {

    preferences.putString(
        "ssid",
        ssid
    );

    preferences.putString(
        "password",
        password
    );

    preferences.putString(
        "jwt",
        jwt
    );

    preferences.putBool(
        "wifi_done",
        true
    );
}

String Storage::getSSID() {

    return preferences.getString(
        "ssid",
        ""
    );
}

String Storage::getPassword() {

    return preferences.getString(
        "password",
        ""
    );
}

String Storage::getJWT() {

    return preferences.getString(
        "jwt",
        ""
    );
}

void Storage::saveProvision(
    String registeredBy,
    String collegeId,
    String departmentId,
    String deviceSecret,
    String mqttUser,
    String mqttPassword
) {

    preferences.putString(
        "registeredBy",
        registeredBy
    );

    preferences.putString(
        "collegeId",
        collegeId
    );

    preferences.putString(
        "departmentId",
        departmentId
    );

    preferences.putString(
        "deviceSecret",
        deviceSecret
    );

    preferences.putString(
        "mqttUser",
        mqttUser
    );

    preferences.putString(
        "mqttPassword",
        mqttPassword
    );

    preferences.putBool(
        "provisioned",
        true
    );
}

String Storage::getCollegeId() {

    return preferences.getString(
        "collegeId",
        ""
    );
}

String Storage::getDepartmentId() {

    return preferences.getString(
        "departmentId",
        ""
    );
}