

const int buttonTimeSwitchPin = 6;
const int buttonLedSwitchPin = 7;

const int ledGreenPin = 4;
const int ledYellowPin = 3;
const int ledRedPin = 2;

bool ledGreenState = LOW;
bool ledYellowState = LOW;
bool ledRedState = LOW;

bool buttonTimeSwitchState = LOW;
bool buttonLedSwitchState = LOW;

unsigned long tsButtonLastChangeTime = 0;
unsigned long lsButtonLastChangeTime = 0;
unsigned long blinkTime = 0;

const unsigned int debounceDelay = 50;

int ledFreqs[4] = { 1000, 500, 250, 125};
int ledFreq = 1000;
int ledFreqsIdx = 0;

int ledFlashes[7][3] = {
  { 1, 0, 0 },
  { 0, 1, 0 },
  { 0, 0, 1 },
  { 1, 1, 0 },
  { 0, 1, 1 },
  { 1, 0, 1 },
  { 1, 1, 1 }
};
int ledFlashesIdx = 0;
//int ledFlash[1][3] = { 1, 0, 0 };

int ledFlashGr = 1;
int ledFlashYl = 0;
int ledFlashRd = 0;


void setup() {
  
  //Serial.begin(9600);
  // Buttons
  pinMode(buttonTimeSwitchPin, INPUT);
  pinMode(buttonLedSwitchPin, INPUT);
  // Initialize Buttons
  digitalWrite(buttonTimeSwitchPin, buttonTimeSwitchState);
  digitalWrite(buttonLedSwitchPin, buttonLedSwitchState);
  // LEDs
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledYellowPin, OUTPUT);
  pinMode(ledRedPin, OUTPUT);
  // Initialize LEDs
  digitalWrite(ledGreenPin, ledGreenState);
  digitalWrite(ledYellowPin, ledYellowState);
  digitalWrite(ledRedPin, ledRedState);
}

void loop() {


  // -------------------------------------------------------------- //
  // Change which LEDs blink
  // -------------------------------------------------------------- //
  bool lsButtonRead = digitalRead(buttonLedSwitchPin);

  if (lsButtonRead == HIGH) {
    if ((millis() - lsButtonLastChangeTime) > debounceDelay) {
      ledFlashesIdx = ledFlashesIdx + 1;
      if (ledFlashesIdx >= 7) {
        ledFlashesIdx = 0;
      }
    }
    lsButtonLastChangeTime = millis();
  }

  // -------------------------------------------------------------- //
  // Change the frequency at which LEDs blink
  // -------------------------------------------------------------- //
  bool tsButtonRead = digitalRead(buttonTimeSwitchPin);

  if (tsButtonRead == HIGH) {
    if ((millis() - tsButtonLastChangeTime) > debounceDelay) {
      ledFreqsIdx = ledFreqsIdx + 1;
      if (ledFreqsIdx >= 4) {
        ledFreqsIdx = 0;
      }
    }
    tsButtonLastChangeTime = millis();
  }

  /*ledFlashGr = ledFlashes[ledFlashesIdx][0];
  ledFlashYl = ledFlashes[ledFlashesIdx][1];
  ledFlashRd = ledFlashes[ledFlashesIdx][2];
  */
  ledFreq = ledFreqs[ledFreqsIdx];

  for (int i = 0; i < 3; i++) {

    if (ledFlashes[ledFlashesIdx][i] == 1) {

      //Serial.println("BlinkTime + Freq:      " + String((blinkTime + ledFreq)));
      //Serial.println("Millis:                " + String(millis()));


      if ((blinkTime + ledFreq) > millis()) {
        digitalWrite(i + 2, LOW);
        //blinkTime = millis();
      } else {
        digitalWrite(i + 2, HIGH);
        blinkTime = millis();
      }
      //blinkTime = millis();
    } else {
      digitalWrite(i + 2, LOW);
    }
  }
}
