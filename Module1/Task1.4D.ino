#define Soil_Pin A0

const int trigPin = 2;    // Pin 2 is connected to trigger pin
const int echoPin = 3;    // Pin 3 is connected to echo pin
const int pirPin = 4;     // Pin 4 is connected to the output pin of the PIR motion sensor

const int ledPin = 13;    // Pin 13 is connected to the LED for distance detection
const int pirLedPin = 11; // Pin 11 is connected to the LED for motion detection
const int soilLedPin = 5; // Pin 5 is connected to the LED for soil moisture detection
const int timerLedPin = 7; // Pin 7 is connected to the LED for timer interrupt

volatile long echo_start;
volatile long echo_end;
volatile int distance;
volatile int motion;
volatile int soilMoisture;

unsigned long previousMillis = 0;
const long interval = 5000; // Interval for timer interrupt (1 second)

void setup() {
  Serial.begin(9600);      // Serial communication initialization 
  pinMode(trigPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(pirLedPin, OUTPUT);
  pinMode(soilLedPin, OUTPUT);
  pinMode(timerLedPin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(Soil_Pin, INPUT);

  // Enable Pin Change Interrupts for Soil Moisture sensor
  PCMSK1 |= (1 << PCINT8);
  PCICR |= (1 << PCIE1);
  
  // Attach interrupts to the echo pin and the pir pin
  attachInterrupt(digitalPinToInterrupt(echoPin), distance_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pirPin), motion_interrupt, CHANGE);
  
  // Initialize timer interrupt
  Timer1_initialize();
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if it's time to update the LED for the timer interrupt
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    digitalWrite(timerLedPin, !digitalRead(timerLedPin)); // Toggle LED state
  }

  // A pulse is sent to the ultrasonic sensor to activate it
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Interrupts are used to compute the pulse's duration
  delay(1000);  // Wait for the pulse to return
  distance = (echo_end - echo_start) * 0.034 / 2;

  // Output the motion and distance information to the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm - ");
  if (distance < 100) {
    Serial.print("Distance Detected");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.print("Distance Not Detected");
    digitalWrite(ledPin, LOW);
  }
  Serial.print(" |Motion ");
  motion_interrupt(); // To output the motion value, use the motion interrupt
  delay(500);
}

// Disrupt the echo pin's servicing procedure.
void distance_interrupt() {
  if (digitalRead(echoPin) == HIGH) {
    echo_start = micros();  // Save the start time
  } else {
    echo_end = micros();  // Save the end time
  }
}

// Disrupt the pir pin's normal servicing
void motion_interrupt() {
  // Obtain the PIR motion sensor's digital value
  motion = digitalRead(pirPin);
  // Print motion value to Serial Monitor
  Serial.print("Value: " + String(motion));
  // Turn on the LED if the motion value is HIGH
  if (motion == HIGH) {
    Serial.println(" | Motion Detected");
    digitalWrite(pirLedPin, HIGH);
  } else {
    Serial.println(" | Motion Not Detected");
    digitalWrite(pirLedPin, LOW);
  }
}

// PCINT for Soil Moisture sensor
ISR(PCINT1_vect) {
  soilMoisture = analogRead(Soil_Pin);
  if (soilMoisture < 500) {
    digitalWrite(soilLedPin, HIGH);
    Serial.println("Soil Moisture Low!");
  } else {
    digitalWrite(soilLedPin, LOW);
    Serial.println("Soil Moisture Normal");
  }
}

// Timer1 initialize - 1Hz
void Timer1_initialize() {
  // Set up Timer1 for the timer interrupt
  TCCR1A = 0; // Set entire TCCR1A register to 0
  TCCR1B = 0; // Same for TCCR1B
  
  // Set compare match register for 1Hz increments
  OCR1A = 15624; // = (16*10^6) / (1*1024) - 1 (must be <65536)
  // Turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // Enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
}

// Timer1 ISR
ISR(TIMER1_COMPA_vect) {
  
}
