/**
 * @file CustomSensors.cpp
 * @brief Implementation of HarvestPod custom ModestIoT sensors.
 * @author Carlos Fredy Fernandez Camayo
 * @student u202320083
 */

#include "CustomSensors.h"

PodDhtSensor::PodDhtSensor(int pin, uint8_t type, EventHandler* owner)
    : Sensor(pin, owner), dht(pin, type), temperature(0.0f), humidity(0.0f) {}

void PodDhtSensor::begin() {
  dht.begin();
}

void PodDhtSensor::measure() {
  float measuredTemperature = dht.readTemperature();
  float measuredHumidity = dht.readHumidity();

  if (!isnan(measuredTemperature)) {
    temperature = measuredTemperature;
  }

  if (!isnan(measuredHumidity)) {
    humidity = measuredHumidity;
  }
}

float PodDhtSensor::getTemperature() const {
  return temperature;
}

float PodDhtSensor::getHumidity() const {
  return humidity;
}

void PodDhtSensor::processEvent(Event& event) {
  if (event.identifier == Sensor::MEASURE_DATA_REQUESTED_EVENT_IDENTIFIER) {
    measure();
    event = Sensor::DATA_READ_EVENT;
  } else {
    event.identifier = -1;
  }
}

LightSensor::LightSensor(int pin, EventHandler* owner)
    : Sensor(pin, owner), lightLevel(0) {}

void LightSensor::begin() {
  pinMode(pin, INPUT);
}

void LightSensor::measure() {
  int rawValue = analogRead(pin);
  lightLevel = map(
      rawValue,
      0,
      HarvestConfig::ADC_RAW_MAX,
      0,
      HarvestConfig::ADC_SCALED_MAX);
}

int LightSensor::getLightLevel() const {
  return lightLevel;
}

void LightSensor::processEvent(Event& event) {
  if (event.identifier == Sensor::MEASURE_DATA_REQUESTED_EVENT_IDENTIFIER) {
    measure();
    event = Sensor::DATA_READ_EVENT;
  } else {
    event.identifier = -1;
  }
}

ButtonSensor::ButtonSensor(int pin, EventHandler* owner)
    : Sensor(pin, owner), pressed(false), lastState(HIGH) {}

void ButtonSensor::begin() {
  pinMode(pin, INPUT_PULLUP);
}

void ButtonSensor::measure() {
  bool currentState = digitalRead(pin);
  pressed = (currentState == LOW && lastState == HIGH);
  lastState = currentState;
}

bool ButtonSensor::isPressed() {
  bool wasPressed = pressed;
  pressed = false;
  return wasPressed;
}

void ButtonSensor::processEvent(Event& event) {
  if (event.identifier == Sensor::MEASURE_DATA_REQUESTED_EVENT_IDENTIFIER) {
    measure();
    event = Sensor::DATA_READ_EVENT;
  } else {
    event.identifier = -1;
  }
}
