
const int buttonPin = 2;
const int ledPin = 13;

bool ledState = LOW;
bool buttonState = LOW;

unsigned long lastChangeTime = 0;
int debounceDelay = 50;

void setup() {  
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
}

void loop() {

  bool buttonRead = digitalRead(buttonPin);

  if(buttonRead == HIGH){
    if((millis() - lastChangeTime) > debounceDelay){
        ledState = !ledState;
        digitalWrite(ledPin, ledState);
    }
    lastChangeTime = millis();
  }

  Serial.println(buttonRead);

}
