int samples[] = { 1, 2, 3, 4, 5, 6 };
int digitalPins[] = { 8, 7, 6, 5, 4, 3 };
int analogPins[] = { A0, A1, A2, A3, A4, A5 };
float resistorValues[] = {
  9960.00,   // 1
  9990.00,   // 2
  10010.00,  // 3
  10000.00,  // 4
  9970.00,   // 5
  9980.00    // 6
};

int pinDigital = 0;
int pinAnalog = A0;
float resistorValue = 0.00;
int sampleID = 0;

float refVoltageHigh = 4907.00; // mV


void setup() {
  pinMode(pinDigital, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  sampleID++;

  int numSamples = sizeof(samples) / sizeof(samples[0]);

  Serial.println("## --------------------------------------------------------- ##");
  Serial.println("");
  Serial.print("## Sample #");
  Serial.println(sampleID);
  Serial.println("## --------------------------------------------------------- ##");

  for (int i = 0; i < numSamples; i++) {
    int pinDigital = digitalPins[i];
    int pinAnalog = analogPins[i];
    float resistorValue = resistorValues[i];

    digitalWrite(pinDigital, HIGH);           // Aplica uma tensão aos eletrodos
    delay(2500);                              // Espera para estabilizar a leitura
    int sensorValue = analogRead(pinAnalog);  // Lê a tensão
    digitalWrite(pinDigital, LOW);            // Desliga a tensão

    float voltage = sensorValue * (refVoltageHigh / 1023.0);                  // Converte o valor lido para tensão
    float resistance = (refVoltageHigh - voltage) * resistorValue / voltage;  // Calcula a resistência
    float conductivity = (1.0000 / resistance) * 61000.0000;             // Calcula a condutividade (mS/cm)

    Serial.print("[");
    Serial.print(samples[i]);
    Serial.print("] ");
    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.print(" Resistence: ");
    Serial.print(resistance);
    Serial.print(" ohms, Conductivity: ");
    Serial.print(conductivity);
    Serial.println(" mS/cm");

    delay(2500);  // Espera antes de fazer a próxima leitura
  }
  delay(3000);
}
