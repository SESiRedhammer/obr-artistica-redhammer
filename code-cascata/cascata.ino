#include <Servo.h> // Biblioteca para controlar o servo motor que ativa o kevin

// --- Sensor Ultrassônico ---
const int triggerPin = 50; //pino que emite um pulso HIGH a cada 10 microsegundos e então cria uma onda de 40kHz
const int echoPin = 51; //Pino que recebe o pulso de retorno, ele envia sinal HIGH quando a ona é refletida, o tempo que ele fica em HIGH é o tempo de ida e volta da onda

// --- LED Piscante ---
const int ledPiscante = 48; //pino conectado ao led da base da cachoeira
unsigned long tempoAnterior = 0; //controle do tempo de inciio do acendimento do LED
const int intervaloPisca = 500; //tempo de ligar e desligar LED
bool estadoLed = false; //indicação se o LED está ligado ou desligado

// --- LEDs da Cascata ---
const int leds[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; //pinos em que cada linha estão conectados
const int numLeds = sizeof(leds) / sizeof(leds[0]); //definição do tamanho do array de cima
int tempoDelay = 500; //tempo de inicio da sequencia 

// --- Servo Motor ---
Servo servoMotor; //chamada de inserção de servo motor pela biblioteca inserida no topo do código
const int servoPin = 46; //pino que o controle do servo motor esta ligado

// --- Controle do Micro:bit (5V na porta A0) ---
const int microbitPin = A0; //pino em que o positivo do microbit está conectado
bool servoAtivado = false; // definição se o microbit está ligado ou desligado

void setup() { //definição do que é cada váriavél declarada acima
  Serial.begin(9600); //iniciar cominicação em 9600 bits por segundo serial entre o arduino/compuitador/sensores

  pinMode(triggerPin, OUTPUT); //definir o pino do trigger do sensor ultrassonico como saída
  pinMode(echoPin, INPUT);

  pinMode(ledPiscante, OUTPUT);
  digitalWrite(ledPiscante, LOW);

  pinMode(microbitPin, OUTPUT);
  digitalWrite(microbitPin, LOW); // Microbit inicialmente desligado

  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  servoMotor.attach(servoPin);
  servoMotor.write(0); // Posição inicial
}

void loop() {
  float distancia = medirDistancia();

  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if (distancia > 0 && distancia < 13) {
    if (!servoAtivado) {
      moverServoSuave(0, 92);   // Movimento suave para 92
      digitalWrite(microbitPin, HIGH); // Liga o microbit
      servoAtivado = true;
    }

    piscarLedIndependente();
    cascataLED();

  } else {
    if (servoAtivado) {
      moverServoSuave(92, 0);   // Movimento suave de volta
      digitalWrite(microbitPin, LOW); // Desliga o microbit
      servoAtivado = false;
    }

    apagarTodos();
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

void cascataLED() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(leds[i], HIGH);
    delay(tempoDelay);
    digitalWrite(leds[i], LOW);
  }
}

void apagarTodos() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(leds[i], LOW);
  }
}

void moverServoSuave(int inicio, int fim) {
  if (inicio < fim) {
    for (int pos = inicio; pos <= fim; pos++) {
      servoMotor.write(pos);
      delay(15); // Controla a suavidade
    }
  } else {
    for (int pos = inicio; pos >= fim; pos--) {
      servoMotor.write(pos);
      delay(15); // Controla a suavidade
    }
  }
}
