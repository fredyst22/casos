#ifndef HARVEST_TELEMETRY_H
#define HARVEST_TELEMETRY_H

#include <Arduino.h>
#include <ModestIoT.h>

/**
 * @file HarvestTelemetry.h
 * @brief Telemetry package layout for HarvestPod status.
 * @author Carlos Fredy Fernandez Camayo
 */
class HarvestTelemetry : public TelemetryPackage {
private:
    String macAddress;
    String operationMode;
    float currentHumidity;
    float currentTemperature;
    int lightLevel;
    String irrigationState;
    String createdAt;

public:
    HarvestTelemetry(String mac, String mode, float hum, float temp, int light, String state, String timeStr)
        : macAddress(mac), operationMode(mode), currentHumidity(hum), currentTemperature(temp), 
          lightLevel(light), irrigationState(state), createdAt(timeStr) {}

    void serialize(JsonDocument& document) const override {
        document["deviceMacAddress"] = macAddress;
        document["operationMode"] = operationMode;
        document["currentHumidity"] = currentHumidity;
        document["currentTemperature"] = currentTemperature;
        document["lightLevel"] = lightLevel;
        document["irrigationState"] = irrigationState;
        document["createdAt"] = createdAt;
    }
};

#endif