/**
   @file sketch.ino
   @brief HarvestPod Advance System implementation using ModestIoT.

   @details Creates the main HarvestPodDevice instance and delegates
   execution to its object-oriented methods.

   @author Carlos Fredy Fernandez Camayo
*/

#include <Arduino.h>
#include "HarvestPodDevice.h"

// Pointer to the main device object.
HarvestPodDevice* podDevice = nullptr;

/**
   @brief Initializes the serial monitor and the Harvest Pod device.
*/
void setup() {
  Serial.begin(115200);

  podDevice = new HarvestPodDevice();
  podDevice->setup();
}

/**
   @brief Executes the main device update cycle continuously.
*/
void loop() {
  podDevice->update();
}