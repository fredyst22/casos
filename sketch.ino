/**
 * @file sketch.ino
 * @brief HarvestPod Advance entry point using ModestIoT Device inheritance.
 *
 * @details Creates one HarvestPodDevice instance and delegates Arduino
 * setup/loop responsibilities to object-oriented device methods.
 *
 * @author Carlos Fredy Fernandez Camayo
 * @student u202320083
 * @company FreshPlot
 */

#include <Arduino.h>
#include "HarvestPodDevice.h"

/** @brief Pointer to the main HarvestPod Advance device instance. */
HarvestPodDevice* harvestPodDevice = nullptr;

/**
 * @brief Boots the serial console and initializes the IoT device.
 */
void setup() {
  Serial.begin(115200);
  delay(500);

  harvestPodDevice = new HarvestPodDevice();
  harvestPodDevice->setup();
}

/**
 * @brief Continuously executes the device update cycle.
 */
void loop() {
  harvestPodDevice->update();
}
