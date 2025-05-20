const int triggerPin = 50;
const int echoPin = 51;

const int ledPiscante = 48; // LED abaixo da cachoeira
unsigned long tempoAnterior = 0;
const int intervaloPisca = 500;
bool estadoLed = false;

void setup() {
  Serial.begin(9600);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(ledPiscante, OUTPUT);
  digitalWrite(ledPiscante, LOW);
}

void loop() {
  float distancia = medirDistancia();

  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if (distancia > 0 && distancia < 10) {
    piscarLedIndependente();
  } else {
    digitalWrite(ledPiscante, LOW);
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

  long duracao = pulseIn(echoPin, HIGH, 25000);
  if (duracao == 0) return -1;

  float distancia = duracao * 0.0343 / 2;
  return distancia;
}
