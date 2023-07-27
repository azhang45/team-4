int solenoidPin = 6;                    //This is the output pin on the Arduino

void setup() 
{
  pinMode(6, OUTPUT);          //Sets that pin as an output
  pinMode(9, OUTPUT);          //Sets that pin as an output
  pinMode(10, OUTPUT);          //Sets that pin as an output
  pinMode(11, OUTPUT);          //Sets that pin as an output
  pinMode(12, OUTPUT);          //Sets that pin as an output
  pinMode(13, OUTPUT);          //Sets that pin as an output
}

void loop() 
{
  digitalWrite(6, HIGH);      //Switch Solenoid ON
  digitalWrite(9, HIGH);      //Switch Solenoid ON
  digitalWrite(10, HIGH);      //Switch Solenoid ON
  digitalWrite(11, HIGH);      //Switch Solenoid ON
  digitalWrite(12, HIGH);      //Switch Solenoid ON
  digitalWrite(13, HIGH);      //Switch Solenoid ON
  delay(1000);                          //Wait 1 Second
  digitalWrite(6, LOW);       //Switch Solenoid OFF
  digitalWrite(9, LOW);       //Switch Solenoid OFF
  digitalWrite(10, LOW);       //Switch Solenoid OFF
  digitalWrite(11, LOW);       //Switch Solenoid OFF
  digitalWrite(12, LOW);       //Switch Solenoid OFF
  digitalWrite(13, LOW);       //Switch Solenoid OFF
  delay(1000);                          //Wait 1 Second
}