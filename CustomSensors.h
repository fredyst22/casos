#ifndef CUSTOM_SENSORS_H
#define CUSTOM_SENSORS_H

#include <Arduino.h>
#include <ModestIoT.h>
#include <DHT.h>

/**
 * @file CustomSensors.h
 * @brief Custom sensor classes inheriting from ModestIoT Sensor.
 */

// Renombrado a PodDhtSensor para evitar conflicto con la librería interna
class PodDhtSensor : public Sensor {
private:
    DHT dht;
    float temperature;
    float humidity;

protected:
    void processEvent(Event& event) override;

public:
    PodDhtSensor(int pin, uint8_t type, EventHandler* owner = nullptr);
    void begin();
    void measure();
    float getTemperature() const;
    float getHumidity() const;
};

class LightSensor : public Sensor {
private:
    int lightLevel;

protected:
    void processEvent(Event& event) override;

public:
    LightSensor(int pin, EventHandler* owner = nullptr);
    void begin();
    void measure();
    int getLightLevel() const;
};

class ButtonSensor : public Sensor {
private:
    bool pressed;
    bool lastState;

protected:
    void processEvent(Event& event) override;

public:
    ButtonSensor(int pin, EventHandler* owner = nullptr);
    void begin();
    void measure();
    bool isPressed();
};

#endif