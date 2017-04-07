int ledPin = 13;
int buttonPin = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(buttonPin) == LOW){
    digitalWrite(ledPin, HIGH);
  }
  if(digitalRead(buttonPin) == HIGH){
    digitalWrite(ledPin, LOW);
  }
  

}
