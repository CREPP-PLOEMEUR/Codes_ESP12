
#define DEBOUNCE_TIME 200      //'Anti-rebond'

const uint8_t buttonPin = D2;
const uint8_t ledPin = D4;

volatile bool stateLed = false; //State of LED

uint16_t currentTime = 0; //Current time
uint16_t previousTime = 0;

//ISR = Interrupt Service Routine
void IRAM_ATTR blinkLed()
{
  //Update
  previousTime = currentTime;
  currentTime = millis();

  if(currentTime > (previousTime + DEBOUNCE_TIME) )
  {
    stateLed =! stateLed;         //Swap led state 
    digitalWrite(ledPin, stateLed);  //Update state
    Serial.println("Changing led state");
  }

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




