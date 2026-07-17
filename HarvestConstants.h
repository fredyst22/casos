/**
 * @file HarvestConstants.h
 * @brief Shared constants for HarvestPod Advance to avoid magic values.
 * @author Carlos Fredy Fernandez Camayo
 * @student u202320083
 */

#ifndef HARVEST_CONSTANTS_H
#define HARVEST_CONSTANTS_H

#include <Arduino.h>

/** @brief GPIO pin assignments for HarvestPod Advance hardware. */
namespace HarvestPins {
  static const int DHT = 4;
  static const int LIGHT_SENSOR = 34;
  static const int BUTTON_UP = 12;
  static const int BUTTON_DOWN = 14;
  static const int SERVO = 13;
  static const int LED_GREEN = 25;
  static const int LED_RED = 26;
  static const int RELAY = 27;
}

/** @brief Business-rule thresholds and configuration limits. */
namespace HarvestConfig {
  static const int LIGHT_NIGHT_THRESHOLD = 200;
  static const float TEMPERATURE_VENTILATION_CELSIUS = 28.0f;
  static const float HUMIDITY_VENTILATION_PERCENT = 85.0f;
  static const int TARGET_HUMIDITY_MIN = 40;
  static const int TARGET_HUMIDITY_MAX = 80;
  static const int TARGET_HUMIDITY_STEP = 5;
  static const int TARGET_HUMIDITY_TOLERANCE = 5;
  static const int DEFAULT_TARGET_HUMIDITY = 60;
  static const int SERVO_CLOSED_DEGREES = 0;
  static const int SERVO_OPEN_DEGREES = 90;
  static const unsigned long TELEMETRY_INTERVAL_MS = 5000UL;
  static const unsigned long UI_INTERVAL_MS = 100UL;
  static const int LCD_I2C_ADDRESS = 0x27;
  static const int LCD_COLUMNS = 16;
  static const int LCD_ROWS = 2;
  static const int ADC_RAW_MAX = 4095;
  static const int ADC_SCALED_MAX = 1023;
}

/** @brief Named operation and irrigation states used by rules and telemetry. */
namespace HarvestStates {
  static const char* DAY_MODE = "DAY_MODE";
  static const char* NIGHT_MODE = "NIGHT_MODE";
  static const char* IRRIGATION = "RIEGO";
  static const char* OPTIMAL = "OPTIMAL";
  static const char* VENTILATING = "VENTILANDO";
}

/** @brief Company and developer identity shown at boot. */
namespace HarvestIdentity {
  static const char* COMPANY = "FreshPlot";
  static const char* PRODUCT = "HarvestPod Advance";
  static const char* DEVELOPER = "Carlos Fredy Fernandez Camayo";
  static const char* STUDENT_CODE = "u202320083";
  static const char* WIFI_SSID = "Wokwi-GUEST";
  static const char* WIFI_PASSWORD = "";
}

#endif
