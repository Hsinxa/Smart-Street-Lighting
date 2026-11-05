// Arduino Starter Sketch for Wokwi Simulation
// Computer Exhibition 2026

const int LED_PIN = 13;
int counter = 0;

void setup() {
  // Initialize Serial communication at 115200 baud
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  Serial.println("==========================================");
  Serial.println("  Welcome to Computer Exhibition 2026!   ");
  Serial.println("  Wokwi Arduino Simulator Initialized   ");
  Serial.println("==========================================");
}

void loop() {
  // Turn LED on
  digitalWrite(LED_PIN, HIGH);
  Serial.print("[TICK ");
  Serial.print(++counter);
  Serial.println("] LED is ON");
  delay(1000);

  // Turn LED off
  digitalWrite(LED_PIN, LOW);
  Serial.print("[TICK ");
  Serial.print(counter);
  Serial.println("] LED is OFF");
  delay(1000);
}

