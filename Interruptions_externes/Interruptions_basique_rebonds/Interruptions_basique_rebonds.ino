
const uint8_t buttonPin = D2;
const uint8_t ledPin = D4;

volatile bool stateLed = false; //State of LED

//ISR = Interrupt Service Routine
void IRAM_ATTR blinkLed()
{
  stateLed =! stateLed;         //Swap led state 
  digitalWrite(ledPin, stateLed);  //Update state
  Serial.println("Changing led state");
}


void setup() 
{

  pinMode(ledPin, OUTPUT);      //Set led on OUTPUT
  attachInterrupt(digitalPinToInterrupt(buttonPin), blinkLed, RISING); //Start interrupt service

  Serial.begin(9600);
  

}

void loop() 
{
  //Nothing in this loop...

}




