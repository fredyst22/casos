/**
 * @file HarvestPodDevice.h
 * @brief Main HarvestPod Advance device class based on ModestIoT Device.
 * @author Carlos Fredy Fernandez Camayo
 * @student u202320083
 */

#ifndef HARVEST_POD_DEVICE_H
#define HARVEST_POD_DEVICE_H

#include <Arduino.h>
#include <WiFi.h>
#include <ModestIoT.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include "CustomSensors.h"
#include "CustomActuators.h"
#include "HarvestTelemetry.h"
#include "HarvestConstants.h"

/**
 * @class AppWiFiDriver
 * @brief Concrete WiFiConnectivityDriver required by ModestIoT networking APIs.
 */
class AppWiFiDriver : public WiFiConnectivityDriver {
public:
  AppWiFiDriver(const char* ssid, const char* password)
      : WiFiConnectivityDriver(ssid, password) {}

  /**
   * @brief Satisfies the abstract transmit contract for the simulator runtime.
   * @return Always true in this embedded prototype.
   */
  bool transmit(const char* target, const char* data) override {
    (void)target;
    (void)data;
    return true;
  }
};

/**
 * @class HarvestPodDevice
 * @brief Embedded application that orchestrates sensors, actuators, modes and telemetry.
 */
class HarvestPodDevice : public Device {
private:
  PodDhtSensor dhtSensor;
  LightSensor lightSensor;
  ButtonSensor buttonIncrease;
  ButtonSensor buttonDecrease;
  ServoMotor ventilationServo;
  LedActuator greenLed;
  LedActuator redLed;
  RelayActuator irrigationRelay;
  AppWiFiDriver wifiDriver;
  LiquidCrystal_I2C lcd;
  RTC_DS1307 rtc;

  int targetHumidity;
  unsigned long previousTelemetryMillis;
  unsigned long previousUiMillis;
  String operationMode;
  String irrigationState;
  String macAddress;

  void connectToNetwork();
  void publishTelemetry();
  void processDayModeRules(float temperature, float humidity);
  void enterNightMode();
  void updateLcdDisplay(float temperature, float humidity);
  void setLedState(bool greenOn, bool redOn);
  void setVentilationOpen(bool isOpen);
  void adjustTargetHumidity(int delta);
  String getTimestamp();

public:
  /** @brief Constructs the device and wires all hardware collaborators. */
  HarvestPodDevice();

  /** @brief Initializes serial banner, peripherals, network and initial state. */
  void setup();

  /** @brief Executes UI sampling, rule evaluation and periodic telemetry. */
  void update();

  /** @brief Receives ModestIoT events raised by owned sensors. */
  void on(Event event) override;

  /** @brief Receives ModestIoT commands routed to this device. */
  void handle(Command command) override;
};

#endif
