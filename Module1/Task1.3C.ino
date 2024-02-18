// Arduino's pins connected to the ultrasonic sensor
const int trigPin = 4;    // Pin 4 is connected to trigger pin
const int echoPin = 2;    // Pin 2 is connected to echo pin

// Arduino's pins connected to the PIR motion sensor
const int pirPin = 3;     // Pin 3 is connected to the output pin of the PIR motion sensor

// Arduino's pins connected to the LEDs
const int ledPin = 13;    // Pin 13 is connected to the LED for distance detection
const int pirLedPin = 11; // Pin 11 is connected to the LED for motion detection

// Variables for storing the sensor values
volatile long echo_start;
volatile long echo_end;
volatile int previous_distance = -1;  // Store the previous distance value

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(pirLedPin, OUTPUT);

  // Attach interrupts to the echo pin and the pir pin
  attachInterrupt(digitalPinToInterrupt(echoPin), distance_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pirPin), motion_interrupt, CHANGE);
}

void loop() {
  // The ultrasonic sensor is activated by sending a pulse.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // The pulse's duration is calculated using interrupts.
  // delay(100);  // Wait for the pulse to return

  // The distance is calculated within the interrupt service routine.
  delay(500); // Delay to allow interrupts to trigger
}

// Interrupt service routine for the echo pin
void distance_interrupt() {
  // Disable interrupts for PIR pin
  detachInterrupt(digitalPinToInterrupt(pirPin));

  if (digitalRead(echoPin) == HIGH) {
    echo_start = micros();  // Save the start time
  } else {
    echo_end = micros();    // Save the end time
    // Calculate the distance
    int distance = (echo_end - echo_start) * 0.034 / 2;

    // Check if the distance has changed and if it's less than 100
    if (distance != previous_distance && distance < 100) {
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.print(" cm ");
      Serial.println("Distance Detected");
      previous_distance = distance;  // Update the previous distance

      // Turn on the LED if the distance is less than 100 cm
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
  }

  // Enable interrupts for PIR pin
  attachInterrupt(digitalPinToInterrupt(pirPin), motion_interrupt, CHANGE);
}

// Interrupt service routine for the pir pin
void motion_interrupt() {
  // Disable interrupts for echo pin
  detachInterrupt(digitalPinToInterrupt(echoPin));

  // Read the digital value from the PIR motion sensor
  int motion = digitalRead(pirPin);

  // Turn on the LED if the motion value is HIGH
  if (motion == HIGH) {
    Serial.print("Motion Value: ");
    Serial.print(motion);
    Serial.println(" | Motion Detected");
    digitalWrite(pirLedPin, HIGH);
  } else {
    digitalWrite(pirLedPin, LOW);
  }

  // Enable interrupts for echo pin
  attachInterrupt(digitalPinToInterrupt(echoPin), distance_interrupt, CHANGE);
}
