
//Define the button pin and led pin
const uint8_t BTN_PIN = 2;
const uint8_t LED_PIN = 13;

//Define the variables to store the previous button state and current led state
uint8_t buttonPrevState = LOW;
uint8_t ledState = LOW;

//Runs once when the board starts 
void setup()
{
  //Set a pull-up resistor to the button pin's input configuration.
  pinMode(BTN_PIN, INPUT_PULLUP);
  //Configuring the led pin as output
  pinMode(LED_PIN, OUTPUT);
  //initlializing the serial communication
  Serial.begin(9600);
}

void loop()
{
  //Read the current state of the button 
  uint8_t buttonState = digitalRead(BTN_PIN);
  
  //printing the output in the serial monitor 
  Serial.print(buttonState);
  Serial.print(buttonPrevState);
  Serial.print(ledState);
  Serial.println("");
  
  
  //See if the button's status has changed.
  if(buttonState != buttonPrevState)
  {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }

  //update the previous state of the button for iteration
  buttonPrevState = buttonState;
    
  //add delay to avoid rapid toggling 
  delay(500);
}
