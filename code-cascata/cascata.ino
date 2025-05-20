// Pinos do sensor ultrassônico
const int triggerPin = 50; // Cabo Laranja
const int echoPin = 51; // Cabo Vermelho

// LED piscante
const int ledPiscante = 48; //LED abaixo da cachoeira
unsigned long tempoAnterior = 0; 
const int intervaloPisca = 500;
bool estadoLed = false;

// Pinos dos LEDs da cascata
const int leds[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; 
const int numLeds = sizeof(leds) / sizeof(leds[0]);

// Delay entre LEDs da cascata
int tempoDelay = 500; //Alterar tempo de fade aqui

void setup() {
  Serial.begin(9600); 

  // Configura sensor
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // LED piscante
  pinMode(ledPiscante, OUTPUT);
  digitalWrite(ledPiscante, LOW);

  // LEDs da cascata
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
}

void loop() {
  float distancia = medirDistancia();

  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if (distancia > 0 && distancia < 10) {
    piscarLedIndependente(); // só pisca quando detecta objeto perto
    cascataLED();            // faz a cascata uma vez
  } else {
    apagarTodos();
    digitalWrite(ledPiscante, LOW); // mantém o LED piscante apagado
  }

  delay(100);
}

void piscarLedIndependente() {
  unsigned long agora = millis();
  if (agora - tempoAnterior >= intervaloPisca) {
    tempoAnterior = agora;
    estadoLed = !estadoLed;
    digitalWrite(ledPiscante, estadoLed);
  }
}

float medirDistancia() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duracao = pulseIn(echoPin, HIGH, 25000); // timeout de 25ms

  if (duracao == 0) return -1; // erro na leitura

  float distancia = duracao * 0.0343 / 2;
  return distancia;
}

void cascataLED() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(leds[i], HIGH);  // Acende o LED atual
    delay(tempoDelay);            // Espera
    digitalWrite(leds[i], LOW);   // Apaga o LED atual
  }
}

void apagarTodos() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(leds[i], LOW);
  }
}
