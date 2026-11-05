/*
  JalRakshak - Flood Warning System
  Computer Exhibition 2026

  Hardware:
  - Arduino UNO
  - HC-SR04 ultrasonic sensor
  - Green, yellow and red LEDs (each with a 220 ohm resistor)
  - Active buzzer

  Wiring:
  HC-SR04: TRIG -> D9, ECHO -> D10, VCC -> 5V, GND -> GND
  Green LED anode -> D3 through 220 ohm resistor; cathode -> GND
  Yellow LED anode -> D4 through 220 ohm resistor; cathode -> GND
  Red LED anode -> D5 through 220 ohm resistor; cathode -> GND
  Active buzzer (+) -> D6, (-) -> GND

  IMPORTANT:
  WARNING = 30 cm
  DANGER  = 12 cm
  These are demonstration settings, not universal flood-safety limits.

  Reliability updates:
  - Median filter over the latest three valid distance samples.
  - Reject out-of-range and missing-echo measurements.
  - Require at least two valid samples in the filter window.
  - Buzzer is ALWAYS OFF during SENSOR ERROR.
  - Sensor error is shown by a blinking yellow LED only.
  - Alarm and sampling are non-blocking.
*/

const byte TRIG_PIN = 9;
const byte ECHO_PIN = 10;

const byte GREEN_LED_PIN = 3;
const byte YELLOW_LED_PIN = 4;
const byte RED_LED_PIN = 5;
const byte BUZZER_PIN = 6;

const float WARNING_DISTANCE_CM = 30.0f;
const float DANGER_DISTANCE_CM = 12.0f;

const unsigned long ECHO_TIMEOUT_US = 3000UL;
const unsigned long SAMPLE_INTERVAL_MS = 100UL;

const unsigned long ALARM_CYCLE_MS = 300UL;
const unsigned long ALARM_ON_MS = 100UL;
const unsigned long ERROR_BLINK_MS = 500UL;

const float MIN_VALID_DISTANCE_CM = 2.0f;
const float MAX_VALID_DISTANCE_CM = 400.0f;

float samples[3] = {0.0f, 0.0f, 0.0f};
byte sampleCount = 0;
byte nextSampleIndex = 0;

unsigned long lastSampleTime = 0;
unsigned long alarmCycleStart = 0;

enum SystemState : byte {
  STATE_NORMAL,
  STATE_WARNING,
  STATE_DANGER,
  STATE_SENSOR_ERROR
};

SystemState currentState = STATE_SENSOR_ERROR;

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (duration == 0UL) {
    return -1.0f;
  }

  const float distance = (duration * 0.0343f) / 2.0f;

  if (distance < MIN_VALID_DISTANCE_CM ||
      distance > MAX_VALID_DISTANCE_CM) {
    return -1.0f;
  }

  return distance;
}

float filteredDistanceCm() {
  if (sampleCount < 2) {
    return -1.0f;
  }

  if (sampleCount == 2) {
    return (samples[0] + samples[1]) / 2.0f;
  }

  // Median of three values, robust against one unusually high/low reading.
  float a = samples[0];
  float b = samples[1];
  float c = samples[2];

  if (a > b) { const float t = a; a = b; b = t; }
  if (b > c) { const float t = b; b = c; c = t; }
  if (a > b) { const float t = a; a = b; b = t; }

  return b;
}

void addSample(float distanceCm) {
  if (distanceCm < 0.0f) {
    return;
  }

  samples[nextSampleIndex] = distanceCm;
  nextSampleIndex = (nextSampleIndex + 1) % 3;

  if (sampleCount < 3) {
    sampleCount++;
  }
}

void applyStateIndicators(unsigned long now) {
  switch (currentState) {
    case STATE_NORMAL:
      digitalWrite(GREEN_LED_PIN, HIGH);
      digitalWrite(YELLOW_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      break;

    case STATE_WARNING:
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(YELLOW_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      break;

    case STATE_DANGER: {
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(YELLOW_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, HIGH);

      if (now - alarmCycleStart >= ALARM_CYCLE_MS) {
        alarmCycleStart = now;
      }

      const bool buzzerOn = (now - alarmCycleStart) < ALARM_ON_MS;
      digitalWrite(BUZZER_PIN, buzzerOn ? HIGH : LOW);
      break;
    }

    case STATE_SENSOR_ERROR:
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(YELLOW_LED_PIN,
                   ((now / ERROR_BLINK_MS) % 2) == 0 ? HIGH : LOW);
      // Critical: a missing/invalid sensor reading must never sound the
      // flood danger alarm.
      digitalWrite(BUZZER_PIN, LOW);
      break;
  }
}

void printState(float distanceCm) {
  Serial.print(F("Distance: "));

  if (distanceCm < 0.0f) {
    Serial.print(F("--"));
  } else {
    Serial.print(distanceCm, 1);
  }

  Serial.print(F(" cm | "));

  switch (currentState) {
    case STATE_NORMAL:
      Serial.println(F("NORMAL"));
      break;
    case STATE_WARNING:
      Serial.println(F("WARNING - Water level rising"));
      break;
    case STATE_DANGER:
      Serial.println(F("DANGER - High water level"));
      break;
    case STATE_SENSOR_ERROR:
      Serial.println(F("SENSOR ERROR - Check sensor and wiring"));
      break;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  const unsigned long now = millis();
  alarmCycleStart = now;
  applyStateIndicators(now);

  Serial.println(F("======================================"));
  Serial.println(F("       JALRAKSHAK FLOOD WARNING       "));
  Serial.println(F("       Computer Exhibition 2026      "));
  Serial.println(F("======================================"));
  Serial.println(F("Monitoring water surface distance..."));
  Serial.println(F("Warning: 30 cm | Danger: 12 cm"));
  Serial.println(F("Filter: median of latest 3 valid samples"));
  Serial.println(F("Sensor error: yellow blink; buzzer OFF"));
}

void loop() {
  const unsigned long now = millis();

  applyStateIndicators(now);

  if (now - lastSampleTime < SAMPLE_INTERVAL_MS) {
    return;
  }
  lastSampleTime = now;

  const float rawDistance = readDistanceCm();
  addSample(rawDistance);

  const float distanceCm = filteredDistanceCm();

  if (distanceCm < 0.0f) {
    currentState = STATE_SENSOR_ERROR;
    digitalWrite(BUZZER_PIN, LOW);
    printState(-1.0f);
    applyStateIndicators(millis());
    return;
  }

  if (distanceCm <= DANGER_DISTANCE_CM) {
    if (currentState != STATE_DANGER) {
      alarmCycleStart = millis();
    }
    currentState = STATE_DANGER;
  } else if (distanceCm <= WARNING_DISTANCE_CM) {
    currentState = STATE_WARNING;
  } else {
    currentState = STATE_NORMAL;
  }

  printState(distanceCm);
  applyStateIndicators(millis());
}
