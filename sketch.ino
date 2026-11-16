/*
  HSINXA Smart Street Lighting
  Version: 1.1
  Board: Arduino Uno
  Inputs: Two PIR motion sensors
  Output: Four existing street-light LEDs on the shared output line

  Hardware scope:
  - Keeps the existing Arduino Uno and sensor/LED components.
  - Keeps the existing LED wiring arrangement unchanged by decision.
  - The LED wiring arrangement is a known limitation and is not certified
    for a physical installation by this software update.
*/

const byte STREET_LIGHTS_PIN = 6;
const byte SENSOR_ENTRY_PIN = 2;
const byte SENSOR_EXIT_PIN = 3;

const unsigned long LIGHT_HOLD_TIME_MS = 5000UL;

unsigned long lastMotionTime = 0;
bool lightsAreOn = false;

bool previousEntryState = LOW;
bool previousExitState = LOW;

void setStreetLights(bool turnOn) {
  if (turnOn == lightsAreOn) {
    return;
  }

  digitalWrite(STREET_LIGHTS_PIN, turnOn ? HIGH : LOW);
  lightsAreOn = turnOn;

  if (turnOn) {
    Serial.println(F("Motion detected: street lights ON."));
  } else {
    Serial.println(F("Area clear for 5 seconds: street lights OFF."));
  }
}

void logSensorChanges(bool entryActive, bool exitActive) {
  if (entryActive != previousEntryState) {
    Serial.print(F("Entry sensor: "));
    Serial.println(entryActive ? F("MOTION") : F("clear"));
    previousEntryState = entryActive;
  }

  if (exitActive != previousExitState) {
    Serial.print(F("Exit sensor: "));
    Serial.println(exitActive ? F("MOTION") : F("clear"));
    previousExitState = exitActive;
  }
}

void setup() {
  pinMode(STREET_LIGHTS_PIN, OUTPUT);
  pinMode(SENSOR_ENTRY_PIN, INPUT);
  pinMode(SENSOR_EXIT_PIN, INPUT);

  digitalWrite(STREET_LIGHTS_PIN, LOW);

  Serial.begin(9600);
  Serial.println(F("--------------------------------"));
  Serial.println(F("HSINXA Smart Street Lighting v1.1"));
  Serial.println(F("Arduino Uno | 2 PIR sensors"));
  Serial.println(F("Waiting for motion..."));
  Serial.println(F("--------------------------------"));

  previousEntryState = digitalRead(SENSOR_ENTRY_PIN);
  previousExitState = digitalRead(SENSOR_EXIT_PIN);
}

void loop() {
  const unsigned long now = millis();

  const bool entryActive = (digitalRead(SENSOR_ENTRY_PIN) == HIGH);
  const bool exitActive = (digitalRead(SENSOR_EXIT_PIN) == HIGH);
  const bool motionDetected = entryActive || exitActive;

  logSensorChanges(entryActive, exitActive);

  if (motionDetected) {
    lastMotionTime = now;
    setStreetLights(true);
  } else if (lightsAreOn && (now - lastMotionTime >= LIGHT_HOLD_TIME_MS)) {
    setStreetLights(false);
  }
}
