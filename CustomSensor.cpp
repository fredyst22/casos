/**
 * @file CustomSensors.cpp
 * @brief Implementation of the custom sensor classes.
 */

#include "CustomSensors.h"

// --- PodDhtSensor Implementation ---
PodDhtSensor::PodDhtSensor(int pin, uint8_t type, EventHandler* owner)
    : Sensor(pin, owner), dht(pin, type), temperature(0.0), humidity(0.0) {}

void PodDhtSensor::begin() { dht.begin(); }

void PodDhtSensor::measure() {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t)) temperature = t;
    if (!isnan(h)) humidity = h;
}

float PodDhtSensor::getTemperature() const { return temperature; }
float PodDhtSensor::getHumidity() const { return humidity; }

void PodDhtSensor::processEvent(Event& event) {
    if (event.identifier == Sensor::MEASURE_DATA_REQUESTED_EVENT_IDENTIFIER) {
        measure();
        event = Sensor::DATA_READ_EVENT;
    } else {
        event.identifier = -1;
    }
}

// --- LightSensor Implementation ---
LightSensor::LightSensor(int pin, EventHandler* owner)
    : Sensor(pin, owner), lightLevel(0) {}

void LightSensor::begin() { pinMode(pin, INPUT); }

void LightSensor::measure() {
    lightLevel = analogRead(pin);
    lightLevel = map(lightLevel, 0, 4095, 0, 1023); 
}

int LightSensor::getLightLevel() const { return lightLevel; }

void LightSensor::processEvent(Event& event) {
    if (event.identifier == Sensor::MEASURE_DATA_REQUESTED_EVENT_IDENTIFIER) {
        measure();
        event = Sensor::DATA_READ_EVENT;
    } else {
        event.identifier = -1;
    }
}

// --- ButtonSensor Implementation ---
ButtonSensor::ButtonSensor(int pin, EventHandler* owner)
    : Sensor(pin, owner), pressed(false), lastState(HIGH) {}

void ButtonSensor::begin() { pinMode(pin, INPUT_PULLUP); }

void ButtonSensor::measure() {
    bool currentState = digitalRead(pin);
    pressed = (currentState == LOW && lastState == HIGH); 
    lastState = currentState;
}

bool ButtonSensor::isPressed() {
    bool p = pressed;
    pressed = false; 
    return p;
}

void ButtonSensor::processEvent(Event& event) {
    if (event.identifier == Sensor::MEASURE_DATA_REQUESTED_EVENT_IDENTIFIER) {
        measure();
        event = Sensor::DATA_READ_EVENT;
    } else {
        event.identifier = -1;
    }
}