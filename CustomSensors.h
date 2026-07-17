/**
 * @file CustomSensors.h
 * @brief Custom ModestIoT Sensor descendants for HarvestPod Advance.
 * @author Carlos Fredy Fernandez Camayo
 * @student u202320083
 */

#ifndef CUSTOM_SENSORS_H
#define CUSTOM_SENSORS_H

#include <Arduino.h>
#include <ModestIoT.h>
#include <DHT.h>
#include "HarvestConstants.h"

/**
 * @class PodDhtSensor
 * @brief DHT22 temperature and humidity sensor adapter for ModestIoT.
 */
class PodDhtSensor : public Sensor {
private:
  DHT dht;
  float temperature;
  float humidity;

protected:
  void processEvent(Event& event) override;

public:
  /**
   * @brief Creates a DHT sensor bound to a GPIO pin.
   * @param pin GPIO pin connected to the DHT data line.
   * @param type DHT sensor type (for example DHT22).
   * @param owner Optional event handler owner.
   */
  PodDhtSensor(int pin, uint8_t type, EventHandler* owner = nullptr);

  /** @brief Initializes the underlying DHT driver. */
  void begin();

  /** @brief Reads temperature and humidity, ignoring NaN samples. */
  void measure();

  /** @return Last valid temperature in Celsius. */
  float getTemperature() const;

  /** @return Last valid relative humidity percentage. */
  float getHumidity() const;
};

/**
 * @class LightSensor
 * @brief Photoresistor sensor that exposes a 0-1023 light level.
 */
class LightSensor : public Sensor {
private:
  int lightLevel;

protected:
  void processEvent(Event& event) override;

public:
  /**
   * @brief Creates a light sensor bound to an analog GPIO pin.
   * @param pin Analog GPIO pin connected to the photoresistor AO output.
   * @param owner Optional event handler owner.
   */
  LightSensor(int pin, EventHandler* owner = nullptr);

  /** @brief Configures the analog input pin. */
  void begin();

  /** @brief Samples and scales the analog reading to 0-1023. */
  void measure();

  /** @return Current light level in the 0-1023 range. */
  int getLightLevel() const;
};

/**
 * @class ButtonSensor
 * @brief Tactile button sensor with rising-edge press detection.
 */
class ButtonSensor : public Sensor {
private:
  bool pressed;
  bool lastState;

protected:
  void processEvent(Event& event) override;

public:
  /**
   * @brief Creates a button sensor bound to a digital GPIO pin.
   * @param pin Digital GPIO pin connected to the push button.
   * @param owner Optional event handler owner.
   */
  ButtonSensor(int pin, EventHandler* owner = nullptr);

  /** @brief Configures the pin with internal pull-up. */
  void begin();

  /** @brief Detects a press edge (HIGH to LOW with pull-up wiring). */
  void measure();

  /**
   * @brief Consumes and returns whether a press edge was detected.
   * @return true if a press occurred since the last call.
   */
  bool isPressed();
};

#endif
