// The ultrasonic sensor is attached to the pins on the Arduino
const int trigPin = 2;  // Pin 2 is connected to trigger pin
const int echoPin = 3;  // Pin 3 is connected to echo pin

// Pins on the Arduino connected to the PIR motion sensor
const int pirPin = 4; // Pin 4 is connected to the output pin of the PIR motion sensor

// Pins on the Arduino linked to the LEDs
const int ledPin = 13;  // Pin 13 is connected to the LED for distance detection
const int pirLedPin = 11; // Pin 11 is connected to the LED for motion detection

// Variables to store the values from the sensor
volatile long echo_start;
volatile long echo_end;
volatile int distance;
volatile int motion;

void setup() {
  Serial.begin(9600);  // Serial communication initialization 
  pinMode(trigPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(pirLedPin, OUTPUT);
  
  // Attach interrupts to the echo pin and the pir pin
  attachInterrupt(digitalPinToInterrupt(echoPin), distance_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pirPin), motion_interrupt, CHANGE);
}

void loop() {
  // A pulse is sent to the ultrasonic sensor to activate it
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Interrupts are used to compute the pulse's duration
  delay(100);  // Wait for the pulse to return
  distance = (echo_end - echo_start) * 0.034 / 2;

  // Output the motion and distance information to the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm - ");
  if (distance < 100) {
    Serial.print("Distance Detected");
  } else {
    Serial.print("Distance Not Detected");
  }
  Serial.print(" || Motion: ");
  motion_interrupt(); // To output the motion value, use the motion interrupt
  delay(500);
}

// Disrupt the echo pin's servicing procedure.
void distance_interrupt() {
  if (digitalRead(echoPin) == HIGH) {
    echo_start = micros();  // Save the start time
  } else {
    echo_end = micros();  // Save the end time
    // Turn on the LED if the distance is less than 100 cm
    if (distance < 100) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
  }
}

// Disrupt the pir pin's normal servicing
void motion_interrupt() {
  // Obtain the PIR motion sensor's digital value
  motion = digitalRead(pirPin);
  // Print motion value to Serial Monitor
  Serial.print("Motion Value: " + String(motion));
  // Turn on the LED if the motion value is HIGH
  if (motion == HIGH) {
    Serial.println(" | Motion Detected");
    digitalWrite(pirLedPin, HIGH);
  } else {
    Serial.println(" | Motion Not Detected");
    digitalWrite(pirLedPin, LOW);
  }
}
