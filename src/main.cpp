#include <Arduino.h>
#include <Adafruit_VL53L0X.h>

/*
* TRUTH TABLE FOR DISTANCE SELECTION
* SELECTOR_PIN_1  SELECTOR_PIN_2  DISTANCE
* 0               0               3cm (default)
* 0               1               6cm
* 1               0               9cm
*/

/*
* NOTES FOR AI AGENT
* Always use Serial.printf() for logging instead of Serial.print() or Serial.println()

*/


#define SELECTOR_PIN_1 25  // USE INTERRUPT INSTEAD OF POLLING FOR THIS PIN
#define SELECTOR_PIN_2 26 // USE INTERRUPT INSTEAD OF POLLING FOR THIS PIN
#define STEPPER_DIR_PIN 27
#define STEPPER_STEP_PIN 14
#define STEPPER_ENABLE_PIN 12

#define LIMIT_SWITCH_PIN_1 32 // USE INTERRUPT INSTEAD OF POLLING FOR THIS PIN
#define LIMIT_SWITCH_PIN_2 33 // USE INTERRUPT INSTEAD OF POLLING FOR THIS PIN

#define TOF_SENSOR_I2C_ADDRESS 0x29

bool isLimitSwitch1Pressed = false;
bool isLimitSwitch2Pressed = false;
bool isSelectorPin1Pressed = false;
bool isSelectorPin2Pressed = false;
int currentDistanceSetting = 0; // 1: default (3cm), 2: setting1 (6cm), 3: setting2 (9cm)


int getDistanceSettingFromSelectorPins() {
  if (isSelectorPin1Pressed && isSelectorPin2Pressed) {
    return 1; // 3cm
  } else if (!isSelectorPin1Pressed && isSelectorPin2Pressed) {
    return 2; // 6cm
  } else if (isSelectorPin1Pressed && !isSelectorPin2Pressed) {
    return 3; // 9cm
  } else {
    return 1; // default to 3cm if both are not pressed
  }
}

int setDistanceSetting(int newSetting) {
  if (newSetting < 1 || newSetting > 3) {
    Serial.printf("Invalid distance setting: %d\n", newSetting);
    return currentDistanceSetting; // Return current setting if invalid
  }
  currentDistanceSetting = newSetting;
  Serial.printf("Distance setting changed to: %d\n", currentDistanceSetting);
  return currentDistanceSetting;
}
// OBJECTS
Adafruit_VL53L0X tofSensor = Adafruit_VL53L0X();

void setupTofSensor() {
  if (!tofSensor.begin(TOF_SENSOR_I2C_ADDRESS)) {
    Serial.printf("Failed to initialize TOF sensor!\n");
    while (1);
  }
  Serial.printf("TOF sensor initialized successfully.\n");
}

float getLanceToSoilDistanceCm() {
  // Implement the logic to get the distance from the TOF sensor
  // For now, return a dummy value
  return 10.0; // Replace with actual distance reading
}

bool isSelectorChangeDetected () {
  return isSelectorPin1Pressed || isSelectorPin2Pressed;
}


void selectorPin1ISR() {
  isSelectorPin1Pressed = digitalRead(SELECTOR_PIN_1) == LOW;
}

void selectorPin2ISR() {
  isSelectorPin2Pressed = digitalRead(SELECTOR_PIN_2) == LOW;
}

void limitSwitch1ISR() {
  isLimitSwitch1Pressed = digitalRead(LIMIT_SWITCH_PIN_1) == LOW;
}

void limitSwitch2ISR() {
  isLimitSwitch2Pressed = digitalRead(LIMIT_SWITCH_PIN_2) == LOW;
}

void setup() {
  Serial.begin(115200);
  pinMode(SELECTOR_PIN_1, INPUT_PULLUP);
  pinMode(SELECTOR_PIN_2, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_PIN_1, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_PIN_2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SELECTOR_PIN_1), selectorPin1ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(SELECTOR_PIN_2), selectorPin2ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LIMIT_SWITCH_PIN_1), limitSwitch1ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LIMIT_SWITCH_PIN_2), limitSwitch2ISR, CHANGE);

  setupTofSensor();
}

void loop() {
  if (isSelectorChangeDetected()) {

  } 

  if (isLimitSwitch1Pressed) {
    Serial.printf("Limit Switch 1: %s\n", isLimitSwitch1Pressed ? "Pressed" : "Released");
    isLimitSwitch1Pressed = false;
  }
  if (isLimitSwitch2Pressed) {
    Serial.printf("Limit Switch 2: %s\n", isLimitSwitch2Pressed ? "Pressed" : "Released");
    isLimitSwitch2Pressed = false;
  }



}

