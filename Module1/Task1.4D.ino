// Arduino's pins connected to the ultrasonic sensor

#define Soil_Pin A0
const int trigPin = 4;    // Pin 4 is connected to trigger pin
const int echoPin = 2;    // Pin 2 is connected to echo pin

// Arduino's pins connected to the PIR motion sensor
const int pirPin = 3;     // Pin 3 is connected to the output pin of the PIR motion sensor

// Arduino's pins connected to the LEDs
const int ledPin = 13;    // Pin 13 is connected to the LED for distance detection
const int pirLedPin = 11; // Pin 11 is connected to the LED for motion detection
const int soilLedPin=5;
const int timerpin=7;

// Variables for storing the sensor values
volatile long echo_start;
volatile long echo_end;
volatile int previous_distance = -1;  // Store the previous distance value
volatile int soilMoisture;
volatile bool ledState = false;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(pirLedPin, OUTPUT);
  pinMode(soilLedPin, OUTPUT);
  
  // Enable Pin Change Interrupts for Soil Moisture sensor
  PCMSK1 |= (1 << PCINT8);
  PCICR |= (1 << PCIE1);
  
  // Attach interrupts to the echo pin and the pir pin
  attachInterrupt(digitalPinToInterrupt(echoPin), distance_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pirPin), motion_interrupt, CHANGE);
  
  //Initialize Timer1
  cli(); // Disable interrupts
  TCCR1A = 0; // Set Timer1 control registers to default
  TCCR1B = 0;
  OCR1A = 31249; // Set compare match register for 5Hz frequency (every 5 seconds)
  TCCR1B |= (1 << WGM12); // Turn on CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Set prescaler to 1024 and start the timer
  TIMSK1 |= (1 << OCIE1A); // Enable timer compare interrupt
  sei(); // Enable interrupts
}

void loop() {

  // The ultrasonic sensor is activated by sending a pulse.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // The distance is calculated within the interrupt service routine.
  delay(500); // Delay to allow interrupts to trigger
}
// Timer1 interrupt service routine
ISR(TIMER1_COMPA_vect) {
  
  // Toggle LED state
  ledState = !ledState;
  
  // Update LED based on the state
  digitalWrite(timerpin, ledState ? HIGH : LOW);
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
