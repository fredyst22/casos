/**
 * @file HarvestPodDevice.cpp
 * @brief Business rules and orchestration for HarvestPod Advance.
 * @author Carlos Fredy Fernandez Camayo
 * @student u202320083
 */

#include "HarvestPodDevice.h"

HarvestPodDevice::HarvestPodDevice()
    : Device(1000),
      dhtSensor(HarvestPins::DHT, DHT22, this),
      lightSensor(HarvestPins::LIGHT_SENSOR, this),
      buttonIncrease(HarvestPins::BUTTON_UP, this),
      buttonDecrease(HarvestPins::BUTTON_DOWN, this),
      ventilationServo(HarvestPins::SERVO),
      greenLed(HarvestPins::LED_GREEN),
      redLed(HarvestPins::LED_RED),
      irrigationRelay(HarvestPins::RELAY),
      wifiDriver(HarvestIdentity::WIFI_SSID, HarvestIdentity::WIFI_PASSWORD),
      lcd(HarvestConfig::LCD_I2C_ADDRESS, HarvestConfig::LCD_COLUMNS, HarvestConfig::LCD_ROWS),
      targetHumidity(HarvestConfig::DEFAULT_TARGET_HUMIDITY),
      previousTelemetryMillis(0),
      previousUiMillis(0),
      operationMode(HarvestStates::DAY_MODE),
      irrigationState(HarvestStates::OPTIMAL) {}

void HarvestPodDevice::setup() {
  Serial.println(F("========================================="));
  Serial.print(F("Company: "));
  Serial.println(HarvestIdentity::COMPANY);
  Serial.print(F("Product: "));
  Serial.println(HarvestIdentity::PRODUCT);
  Serial.print(F("Developer: "));
  Serial.println(HarvestIdentity::DEVELOPER);
  Serial.print(F("Student Code: "));
  Serial.println(HarvestIdentity::STUDENT_CODE);
  Serial.println(F("========================================="));

  dhtSensor.begin();
  lightSensor.begin();
  buttonIncrease.begin();
  buttonDecrease.begin();
  greenLed.begin();
  redLed.begin();
  irrigationRelay.begin();

  setVentilationOpen(false);
  setLedState(false, false);

  lcd.init();
  lcd.backlight();

  if (!rtc.begin()) {
    Serial.println(F("WARNING: RTC module not found. Using fallback timestamps."));
  } else if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  connectToNetwork();
  macAddress = WiFi.macAddress();

  Serial.print(F("Device MAC Address: "));
  Serial.println(macAddress);
  Serial.println(F("HarvestPod Advance system online."));
}

void HarvestPodDevice::update() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousUiMillis >= HarvestConfig::UI_INTERVAL_MS) {
    previousUiMillis = currentMillis;

    buttonIncrease.on(Sensor::MEASURE_DATA_REQUESTED_EVENT);
    buttonDecrease.on(Sensor::MEASURE_DATA_REQUESTED_EVENT);
    dhtSensor.on(Sensor::MEASURE_DATA_REQUESTED_EVENT);
    lightSensor.on(Sensor::MEASURE_DATA_REQUESTED_EVENT);

    if (buttonIncrease.isPressed()) {
      adjustTargetHumidity(HarvestConfig::TARGET_HUMIDITY_STEP);
    }

    if (buttonDecrease.isPressed()) {
      adjustTargetHumidity(-HarvestConfig::TARGET_HUMIDITY_STEP);
    }

    const int lightLevel = lightSensor.getLightLevel();
    const float temperature = dhtSensor.getTemperature();
    const float humidity = dhtSensor.getHumidity();

    if (lightLevel < HarvestConfig::LIGHT_NIGHT_THRESHOLD) {
      enterNightMode();
    } else {
      processDayModeRules(temperature, humidity);
    }
  }

  if (currentMillis - previousTelemetryMillis >= HarvestConfig::TELEMETRY_INTERVAL_MS) {
    previousTelemetryMillis = currentMillis;
    publishTelemetry();
  }
}

void HarvestPodDevice::enterNightMode() {
  operationMode = HarvestStates::NIGHT_MODE;

  irrigationRelay.handle(RelayActuator::DEACTIVATE_COMMAND);
  setVentilationOpen(false);
  setLedState(false, false);

  lcd.noBacklight();
  lcd.clear();
}

void HarvestPodDevice::processDayModeRules(float temperature, float humidity) {
  operationMode = HarvestStates::DAY_MODE;
  lcd.backlight();

  const int lowerBound = targetHumidity - HarvestConfig::TARGET_HUMIDITY_TOLERANCE;
  const int upperBound = targetHumidity + HarvestConfig::TARGET_HUMIDITY_TOLERANCE;

  if (humidity < lowerBound) {
    irrigationState = HarvestStates::IRRIGATION;
    irrigationRelay.handle(RelayActuator::ACTIVATE_COMMAND);
    setLedState(false, true);
  } else if (humidity <= upperBound) {
    irrigationState = HarvestStates::OPTIMAL;
    irrigationRelay.handle(RelayActuator::DEACTIVATE_COMMAND);
    setLedState(true, false);
  } else {
    irrigationState = HarvestStates::VENTILATING;
    irrigationRelay.handle(RelayActuator::DEACTIVATE_COMMAND);
    setLedState(false, false);
  }

  const bool shouldVentilate =
      (temperature > HarvestConfig::TEMPERATURE_VENTILATION_CELSIUS) ||
      (humidity > HarvestConfig::HUMIDITY_VENTILATION_PERCENT);

  setVentilationOpen(shouldVentilate);
  updateLcdDisplay(temperature, humidity);
}

void HarvestPodDevice::updateLcdDisplay(float temperature, float humidity) {
  lcd.setCursor(0, 0);
  lcd.print(temperature, 1);
  lcd.print(F("C "));
  lcd.print(humidity, 1);
  lcd.print(F("%   "));

  lcd.setCursor(0, 1);
  lcd.print(F("T:"));
  lcd.print(targetHumidity);
  lcd.print(F("% "));
  lcd.print(irrigationState);
  lcd.print(F("      "));
}

void HarvestPodDevice::setLedState(bool greenOn, bool redOn) {
  greenLed.handle(greenOn ? LedActuator::TURN_ON_COMMAND : LedActuator::TURN_OFF_COMMAND);
  redLed.handle(redOn ? LedActuator::TURN_ON_COMMAND : LedActuator::TURN_OFF_COMMAND);
}

void HarvestPodDevice::setVentilationOpen(bool isOpen) {
  const int angle = isOpen
      ? HarvestConfig::SERVO_OPEN_DEGREES
      : HarvestConfig::SERVO_CLOSED_DEGREES;

  ventilationServo.setTargetAngleBufferInDegrees(angle);
  ventilationServo.handle(ServoMotor::DRIVE_TO_BUFFERED_ANGLE_COMMAND);
}

void HarvestPodDevice::adjustTargetHumidity(int delta) {
  int nextValue = targetHumidity + delta;

  if (nextValue < HarvestConfig::TARGET_HUMIDITY_MIN) {
    nextValue = HarvestConfig::TARGET_HUMIDITY_MIN;
  }

  if (nextValue > HarvestConfig::TARGET_HUMIDITY_MAX) {
    nextValue = HarvestConfig::TARGET_HUMIDITY_MAX;
  }

  targetHumidity = nextValue;
}

void HarvestPodDevice::on(Event event) {
  (void)event;
}

void HarvestPodDevice::handle(Command command) {
  (void)command;
}

void HarvestPodDevice::connectToNetwork() {
  Serial.print(F("Connecting to WiFi"));
  wifiDriver.connect();

  int attempts = 0;
  while (!wifiDriver.isConnected() && attempts < 30) {
    delay(500);
    Serial.print(F("."));
    attempts++;
  }

  Serial.println(wifiDriver.isConnected() ? F("\nWiFi connected.") : F("\nWiFi connection failed."));
}

String HarvestPodDevice::getTimestamp() {
  DateTime now = rtc.now();

  // When the RTC is unavailable, DateTime fields may be zeroed.
  if (now.year() < 2000) {
    unsigned long totalSeconds = millis() / 1000UL;
    unsigned long hours = (totalSeconds / 3600UL) % 24UL;
    unsigned long minutes = (totalSeconds / 60UL) % 60UL;
    unsigned long seconds = totalSeconds % 60UL;

    char fallbackBuffer[25];
    snprintf(
        fallbackBuffer,
        sizeof(fallbackBuffer),
        "2026-07-14T%02lu:%02lu:%02lu",
        hours,
        minutes,
        seconds);
    return String(fallbackBuffer);
  }

  char buffer[25];
  snprintf(
      buffer,
      sizeof(buffer),
      "%04d-%02d-%02dT%02d:%02d:%02d",
      now.year(),
      now.month(),
      now.day(),
      now.hour(),
      now.minute(),
      now.second());
  return String(buffer);
}

void HarvestPodDevice::publishTelemetry() {
  HarvestTelemetry telemetry(
      macAddress,
      operationMode,
      dhtSensor.getHumidity(),
      dhtSensor.getTemperature(),
      lightSensor.getLightLevel(),
      irrigationState,
      getTimestamp());

  Serial.print(F("Telemetry JSON: "));
  telemetry.printToConsole();
}
