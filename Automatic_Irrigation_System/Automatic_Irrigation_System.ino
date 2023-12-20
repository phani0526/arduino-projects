/* </p><p>Innovativetom.com
  Flower Pot Soil Mosture Sensor</p><p>A0 - Soil Mosture Sensor
  D2:D6 - LEDS 1,2,3,4,5</p><p>LED1 - Green
  /LED2 - Green
  LED3 - Green
  LED4 - YELLOW
  LED5 - RED
  /LED6 - RED
  Connect the Soil Mosture Sensor to anolog input pin 0,
  and your 5 led to digital out 2-6</p><p>*/
int led1 = 2;
int led2 = 3;
int led3 = 4;
int led4 = 5;
int motor = 6;
int GND = 7;
int buzzer = 8;
int buzzer_gnd = 9;


int mostureSensor = 0;
void setup() {
  // Serial Begin so we can see the data from the mosture sensor in our serial input window.
  Serial.begin(9600);
  // setting the led pins to outputs
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(GND, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(buzzer_gnd, OUTPUT);
  digitalWrite(GND, LOW);
  digitalWrite(buzzer, LOW);
  digitalWrite(buzzer_gnd, LOW);
}
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(mostureSensor);
  // print out the value you read:</p><p>  Serial.println(sensorValue);
  if (sensorValue >= 615)
  {
    //Full Dry
    digitalWrite(led1, HIGH);//Red
    digitalWrite(led2, LOW);//Yellow
    digitalWrite(led3, LOW);//Blue
    digitalWrite(led4, LOW);//Green
    if(!digitalRead(motor))
    {
    digitalWrite(buzzer, HIGH);
    delay(2000);
    digitalWrite(motor, HIGH);
    digitalWrite(buzzer, LOW);
    }
  }
  else if (sensorValue >= 410 && sensorValue < 650)
  {
    // Medium Level 1
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    if(digitalRead(motor))
    {
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer,LOW);
      delay(500);
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer,LOW);
    }
     digitalWrite(motor, LOW);
    
  }
  else if (sensorValue >= 250 && sensorValue < 410)
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, LOW);
    if(digitalRead(motor))
    {
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer,LOW);
      delay(500);
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer,LOW);
    }
    digitalWrite(motor, LOW);
  }
  else if (sensorValue >= 0 && sensorValue < 250)
  {
    //Full Wet
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, HIGH);
    if(digitalRead(motor))
    {
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer,LOW);
      delay(500);
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer,LOW);
    }
    digitalWrite(motor, LOW);
  }
  delay(500);        // delay 1 second between reads
  Serial.println(sensorValue);
}
