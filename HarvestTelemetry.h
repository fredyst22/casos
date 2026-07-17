/**
 * @file HarvestTelemetry.h
 * @brief Telemetry package layout for HarvestPod Advance status updates.
 * @author Carlos Fredy Fernandez Camayo
 * @student u202320083
 */

#ifndef HARVEST_TELEMETRY_H
#define HARVEST_TELEMETRY_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ModestIoT.h>

/**
 * @class HarvestTelemetry
 * @brief Serializable status payload printed to the serial console every 5 seconds.
 */
class HarvestTelemetry : public TelemetryPackage {
private:
  String deviceMacAddress;
  String operationMode;
  float currentHumidity;
  float currentTemperature;
  int lightLevel;
  String irrigationState;
  String createdAt;

public:
  /**
   * @brief Builds a telemetry record with the current device state.
   * @param mac Device MAC address.
   * @param mode DAY_MODE or NIGHT_MODE.
   * @param humidity Current relative humidity percentage.
   * @param temperature Current temperature in Celsius.
   * @param light Analog light level (0-1023).
   * @param state Irrigation display state (RIEGO, OPTIMAL, VENTILANDO).
   * @param timestamp ISO-like timestamp from the RTC module.
   */
  HarvestTelemetry(
      const String& mac,
      const String& mode,
      float humidity,
      float temperature,
      int light,
      const String& state,
      const String& timestamp)
      : deviceMacAddress(mac),
        operationMode(mode),
        currentHumidity(humidity),
        currentTemperature(temperature),
        lightLevel(light),
        irrigationState(state),
        createdAt(timestamp) {}

  /**
   * @brief Serializes the telemetry fields into a JSON document.
   * @param document Destination ArduinoJson document.
   */
  void serialize(JsonDocument& document) const override {
    document["deviceMacAddress"] = deviceMacAddress;
    document["operationMode"] = operationMode;
    document["currentHumidity"] = currentHumidity;
    document["currentTemperature"] = currentTemperature;
    document["lightLevel"] = lightLevel;
    document["irrigationState"] = irrigationState;
    document["createdAt"] = createdAt;
  }

  /**
   * @brief Prints the telemetry payload as compact JSON to the serial console.
   */
  void printToConsole() const {
    StaticJsonDocument<384> document;
    serialize(document);
    serializeJson(document, Serial);
    Serial.println();
  }
};

#endif
