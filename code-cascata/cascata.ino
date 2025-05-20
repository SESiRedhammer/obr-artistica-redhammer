const int triggerPin = 50; // Cabo Laranja
const int echoPin = 51;    // Cabo Vermelho

void setup() {
  Serial.begin(9600);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  float distancia = medirDistancia();

  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  delay(500);
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
