/**
 * @file CustomActuators.cpp
 * @brief Implementation of LED and relay ModestIoT actuators.
 * @author Carlos Fredy Fernandez Camayo
 * @student u202320083
 */

#include "CustomActuators.h"

const Command LedActuator::TURN_ON_COMMAND(LedActuator::TURN_ON_COMMAND_ID);
const Command LedActuator::TURN_OFF_COMMAND(LedActuator::TURN_OFF_COMMAND_ID);

LedActuator::LedActuator(int pin, CommandHandler* owner)
    : Actuator(pin, owner) {}

void LedActuator::begin() {
  pinMode(pin, OUTPUT);
  turnOff();
}

void LedActuator::turnOn() {
  digitalWrite(pin, HIGH);
}

void LedActuator::turnOff() {
  digitalWrite(pin, LOW);
}

void LedActuator::handle(Command command) {
  if (command.identifier == TURN_ON_COMMAND_ID) {
    turnOn();
  } else if (command.identifier == TURN_OFF_COMMAND_ID) {
    turnOff();
  }
}

const Command RelayActuator::ACTIVATE_COMMAND(RelayActuator::ACTIVATE_COMMAND_ID);
const Command RelayActuator::DEACTIVATE_COMMAND(RelayActuator::DEACTIVATE_COMMAND_ID);

RelayActuator::RelayActuator(int pin, CommandHandler* owner)
    : Actuator(pin, owner) {}

void RelayActuator::begin() {
  pinMode(pin, OUTPUT);
  deactivate();
}

void RelayActuator::activate() {
  digitalWrite(pin, HIGH);
}

void RelayActuator::deactivate() {
  digitalWrite(pin, LOW);
}

void RelayActuator::handle(Command command) {
  if (command.identifier == ACTIVATE_COMMAND_ID) {
    activate();
  } else if (command.identifier == DEACTIVATE_COMMAND_ID) {
    deactivate();
  }
}
