/**
 * @file HarvestPodDevice.cpp
 * @brief Implements the business logic of HarvestPod Advance.
 */

#include "HarvestPodDevice.h"

#define PIN_DHT 4
#define PIN_LDR 34
#define PIN_BTN_UP 12
#define PIN_BTN_DOWN 14
#define PIN_SERVO 13

#define LIGHT_THRESHOLD 200
#define TEMP_THRESHOLD_VENT 28.0
#define HUM_THRESHOLD_VENT 85.0

HarvestPodDevice::HarvestPodDevice()
    : Device(1000),
      dhtSensor(PIN_DHT, DHT22, this),
      lightSensor(PIN_LDR, this),
      btnUp(PIN_BTN_UP, this),
      btnDown(PIN_BTN_DOWN, this),
      ventilationServo(PIN_SERVO),
      wifiDriver("Wokwi-GUEST", ""),
      gatewayClient(wifiDriver, "https://iot-cloud.free.beeceptor.com"),
      lcd(0x27, 16, 2),
      targetHumidity(60),
      previousTelemetryMillis(0),
      telemetryIntervalMillis(5000), 
      previousUiMillis(0),
      uiIntervalMillis(100),         
      operationMode("DAY_MODE"),
      irrigationState("OPTIMAL") {}

void HarvestPodDevice::setup() {
    Serial.println("=========================================");
    Serial.println("Company: FreshPlot");
    Serial.println("Product: HarvestPod Advance");
    Serial.println("Developer: Carlos Fredy Fernandez Camayo");
    Serial.println("=========================================");

    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_RELAY, OUTPUT);
    
    setLeds(false, false);
    digitalWrite(PIN_RELAY, LOW);

    dhtSensor.begin();
    lightSensor.begin();
    btnUp.begin();
    btnDown.begin();

    ventilationServo.setTargetAngleBufferInDegrees(0);
    ventilationServo.handle(ServoMotor::DRIVE_TO_BUFFERED_ANGLE_COMMAND);

    lcd.init();
    lcd.backlight();
    
    if (!rtc.begin()) {
        Serial.println("WARNING: RTC Module not found!");
    }

    connectToNetwork();
    macAddress = WiFi.macAddress();
    Serial.println("HarvestPod System Online.");
}

void HarvestPodDevice::update() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousUiMillis >= uiIntervalMillis) {
        previousUiMillis = currentMillis;
        
        btnUp.on(Sensor::MEASURE_DATA_REQUESTED_EVENT);
        btnDown.on(Sensor::MEASURE_DATA_REQUESTED_EVENT);
        dhtSensor.on(Sensor::MEASURE_DATA_REQUESTED_EVENT);
        lightSensor.on(Sensor::MEASURE_DATA_REQUESTED_EVENT);
        
        if (btnUp.isPressed() && targetHumidity < 80) targetHumidity += 5;
        if (btnDown.isPressed() && targetHumidity > 40) targetHumidity -= 5;
        
        processRules();
    }

    if (currentMillis - previousTelemetryMillis >= telemetryIntervalMillis) {
        previousTelemetryMillis = currentMillis;
        sendTelemetry();
    }
}

void HarvestPodDevice::processRules() {
    int light = lightSensor.getLightLevel();
    float temp = dhtSensor.getTemperature();
    float hum = dhtSensor.getHumidity();

    if (light < LIGHT_THRESHOLD) {
        operationMode = "NIGHT_MODE";
        irrigationState = "NONE";
        
        lcd.noBacklight();
        lcd.clear();
        setLeds(false, false);
        digitalWrite(PIN_RELAY, LOW); 
        
        ventilationServo.setTargetAngleBufferInDegrees(0);
        ventilationServo.handle(ServoMotor::DRIVE_TO_BUFFERED_ANGLE_COMMAND);
        
    } else {
        operationMode = "DAY_MODE";
        lcd.backlight();
        
        if (hum < (targetHumidity - 5)) {
            irrigationState = "RIEGO";
            digitalWrite(PIN_RELAY, HIGH);
            setLeds(false, true); 
        } else if (hum >= (targetHumidity - 5) && hum <= (targetHumidity + 5)) {
            irrigationState = "OPTIMAL";
            digitalWrite(PIN_RELAY, LOW);
            setLeds(true, false); 
        } else {
            irrigationState = "VENTILANDO";
            digitalWrite(PIN_RELAY, LOW);
            setLeds(false, false); 
        }
        
        if (temp > TEMP_THRESHOLD_VENT || hum > HUM_THRESHOLD_VENT) {
            ventilationServo.setTargetAngleBufferInDegrees(90);
        } else {
            ventilationServo.setTargetAngleBufferInDegrees(0);
        }
        ventilationServo.handle(ServoMotor::DRIVE_TO_BUFFERED_ANGLE_COMMAND);

        updateLCD();
    }
}

void HarvestPodDevice::updateLCD() {
    float temp = dhtSensor.getTemperature();
    float hum = dhtSensor.getHumidity();

    lcd.setCursor(0, 0);
    lcd.print(temp, 1); lcd.print("C ");
    lcd.print(hum, 1); lcd.print("%   ");
    
    lcd.setCursor(0, 1);
    lcd.print("T:"); lcd.print(targetHumidity); lcd.print("% ");
    lcd.print(irrigationState);
    lcd.print("      "); 
}

void HarvestPodDevice::setLeds(bool green, bool red) {
    digitalWrite(PIN_LED_GREEN, green ? HIGH : LOW);
    digitalWrite(PIN_LED_RED, red ? HIGH : LOW);
}

void HarvestPodDevice::on(Event event) {}

void HarvestPodDevice::handle(Command command) {}

void HarvestPodDevice::connectToNetwork() {
    Serial.print("Connecting to WiFi");
    wifiDriver.connect();

    int attempts = 0;
    while (!wifiDriver.isConnected() && attempts < 30) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    Serial.println(wifiDriver.isConnected() ? "\nWiFi connected." : "\nWiFi connection failed.");
}

String HarvestPodDevice::getTimestamp() {
    DateTime now = rtc.now();
    char buf[25];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02d", 
             now.year(), now.month(), now.day(),
             now.hour(), now.minute(), now.second());
    return String(buf);
}

void HarvestPodDevice::sendTelemetry() {
    HarvestTelemetry telemetry(
        macAddress, 
        operationMode, 
        dhtSensor.getHumidity(), 
        dhtSensor.getTemperature(), 
        lightSensor.getLightLevel(), 
        irrigationState, 
        getTimestamp()
    );
    
    Serial.print("Sending Telemetry... ");
    bool sent = gatewayClient.sendTelemetryRecord(telemetry);
    Serial.println(sent ? "SUCCESS" : "FAILED");
}