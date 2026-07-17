/**
 * @file CustomActuators.h
 * @brief Custom ModestIoT Actuator descendants for LEDs and relay.
 * @author Carlos Fredy Fernandez Camayo
 * @student u202320083
 */

#ifndef CUSTOM_ACTUATORS_H
#define CUSTOM_ACTUATORS_H

#include <Arduino.h>
#include <ModestIoT.h>

/**
 * @class LedActuator
 * @brief Digital LED actuator controlled through ModestIoT commands.
 */
class LedActuator : public Actuator {
public:
  static const int TURN_ON_COMMAND_ID = 2101;
  static const int TURN_OFF_COMMAND_ID = 2102;
  static const Command TURN_ON_COMMAND;
  static const Command TURN_OFF_COMMAND;

  /**
   * @brief Creates an LED actuator on the given GPIO pin.
   * @param pin Digital GPIO pin connected to the LED anode path.
   * @param owner Optional command handler owner.
   */
  LedActuator(int pin, CommandHandler* owner = nullptr);

  /** @brief Configures the pin as digital output and turns the LED off. */
  void begin();

  /** @brief Turns the LED on. */
  void turnOn();

  /** @brief Turns the LED off. */
  void turnOff();

  /**
   * @brief Handles LED on/off commands.
   * @param command Command to execute.
   */
  void handle(Command command) override;
};

/**
 * @class RelayActuator
 * @brief Relay module actuator used to drive the irrigation system.
 */
class RelayActuator : public Actuator {
public:
  static const int ACTIVATE_COMMAND_ID = 2201;
  static const int DEACTIVATE_COMMAND_ID = 2202;
  static const Command ACTIVATE_COMMAND;
  static const Command DEACTIVATE_COMMAND;

  /**
   * @brief Creates a relay actuator on the given GPIO pin.
   * @param pin Digital GPIO pin connected to the relay IN line.
   * @param owner Optional command handler owner.
   */
  RelayActuator(int pin, CommandHandler* owner = nullptr);

  /** @brief Configures the pin as digital output and deactivates the relay. */
  void begin();

  /** @brief Activates irrigation (relay ON). */
  void activate();

  /** @brief Deactivates irrigation (relay OFF). */
  void deactivate();

  /**
   * @brief Handles relay activate/deactivate commands.
   * @param command Command to execute.
   */
  void handle(Command command) override;
};

#endif
