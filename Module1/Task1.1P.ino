const int trigPin = 2;  // Pin 2 is connected to trigger pin
const int echoPin = 3;  // Pin 3 is connected to echo pin
const int ledPin = 13;  // Pin 13 is connected to the LED

void setup() {
  Serial.begin(9600);  // Serial communication initialization 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // The ultrasonic sensor is activated by sending a pulse.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Using the echo pin, calculate the pulse's duration.
  long duration = pulseIn(echoPin, HIGH);

  // Convert the time to centimeters of distance.
  int distance = duration * 0.034 / 2;


  if (distance < 100){ //if distance is less than 100cm then turn the led on
    digitalWrite(ledPin, HIGH); 
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm");
  Serial.println(" Motion Detected");
    delay(1000);
  } else { 
    digitalWrite(ledPin, LOW);
    Serial.println("Motion not detected");
    delay(1000);
  }

  delay(500);
}
