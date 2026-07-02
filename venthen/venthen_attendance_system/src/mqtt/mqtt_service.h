#ifndef MQTT_SERVICE_H
#define MQTT_SERVICE_H

class MQTTService
{

public:
    static void begin();

    static void loop();

    static bool connected();

    static void publishStatus();
};

#endif