const int trigPin = 2;  // Pin 2 is connected to trigger pin
const int echoPin = 3;  // Pin 3 is connected to echo pin
const int ledPin = 13;  // Pin 13 is connected to the LED
volatile long echo_start;
volatile long echo_end;
volatile int distance;

void setup() {
  Serial.begin(9600);  // Serial communication initialization 
  pinMode(trigPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  // Attach interrupt to the echo pin
  attachInterrupt(digitalPinToInterrupt(echoPin), echo_interrupt, CHANGE);
}

void loop() {
  // The ultrasonic sensor is activated by sending a pulse.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // The pulse's duration is calculated using interrupts.
  delay(100);  // Wait for the pulse to return
  distance = (echo_end - echo_start) * 0.034 / 2;

  if (distance < 100) { // If distance is less than 100cm, turn the LED on
    digitalWrite(ledPin, HIGH); 
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm - Motion Detected");
    delay(1000);
  } else { 
    digitalWrite(ledPin, LOW);
    Serial.println("Distance: Motion not detected");
    delay(1000);
  }

  delay(500);
}

// Interrupt service routine for the echo pin
void echo_interrupt() {
  if (digitalRead(echoPin) == HIGH) {
    echo_start = micros();  // Save the start time
  } else {
    echo_end = micros();  // Save the end time
  }
}
