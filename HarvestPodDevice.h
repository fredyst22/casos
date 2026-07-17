#ifndef HARVEST_POD_DEVICE_H
#define HARVEST_POD_DEVICE_H

#include <Arduino.h>
#include <WiFi.h>
#include <ModestIoT.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include "CustomSensors.h"
#include "HarvestTelemetry.h"

// Clase concreta para solucionar el error de abstracción del WiFiDriver
class AppWiFiDriver : public WiFiConnectivityDriver {
public:
    AppWiFiDriver(const char* ssid, const char* password) : WiFiConnectivityDriver(ssid, password) {}
    bool transmit(const char* target, const char* data) override {
        return true; // Satisface el requisito de la nueva librería
    }
};

/**
 * @file HarvestPodDevice.h
 * @brief Main application logic, handles rules, modes, and network.
 */
class HarvestPodDevice : public Device {
private:
    PodDhtSensor dhtSensor; // Usamos el nombre corregido
    LightSensor lightSensor;
    ButtonSensor btnUp;
    ButtonSensor btnDown;
    ServoMotor ventilationServo;
    
    AppWiFiDriver wifiDriver; // Usamos nuestro driver corregido
    HttpGatewayClient gatewayClient; // Nombre actualizado según la nueva librería
    LiquidCrystal_I2C lcd;
    RTC_DS1307 rtc;

    int targetHumidity;
    unsigned long previousTelemetryMillis;
    unsigned long telemetryIntervalMillis;
    unsigned long previousUiMillis;
    unsigned long uiIntervalMillis;
    
    String operationMode;
    String irrigationState;
    String macAddress;

    static const int PIN_LED_GREEN = 25;
    static const int PIN_LED_RED = 26;
    static const int PIN_RELAY = 27;

    void connectToNetwork();
    void sendTelemetry();
    void processRules();
    void updateLCD();
    void setLeds(bool green, bool red);
    String getTimestamp();

public:
    HarvestPodDevice();
    void setup();
    void update();
    void on(Event event) override;
    void handle(Command command) override;
};

#endif