/*
                        ███████╗███████╗███████╗██╗
                        ██╔════╝██╔════╝██╔════╝██║
                        ███████╗█████╗  ███████╗██║
                        ╚════██║██╔══╝  ╚════██║██║
                        ███████║███████╗███████║██║
                        ╚══════╝╚══════╝╚══════╝╚═╝
                           

    ██████╗ ███████╗██████╗     ██╗  ██╗ █████╗ ███╗   ███╗███╗   ███╗███████╗██████╗ 
    ██╔══██╗██╔════╝██╔══██╗    ██║  ██║██╔══██╗████╗ ████║████╗ ████║██╔════╝██╔══██╗
    ██████╔╝█████╗  ██║  ██║    ███████║███████║██╔████╔██║██╔████╔██║█████╗  ██████╔╝
    ██╔══██╗██╔══╝  ██║  ██║    ██╔══██║██╔══██║██║╚██╔╝██║██║╚██╔╝██║██╔══╝  ██╔══██╗
    ██║  ██║███████╗██████╔╝    ██║  ██║██║  ██║██║ ╚═╝ ██║██║ ╚═╝ ██║███████╗██║  ██║
    ╚═╝  ╚═╝╚══════╝╚═════╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═╝
                                                                                      
                                                                                   
Código controle robô paraido das cachoeiras - OBr artitsica 2025
Equipe REDHAMMER 
Escola SESI Antonio Escalon - CE 423 - Presidente Prudente
versão de controle 1.15.6

*/



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

// --- Controle do Micro:bit ---
const int microbitPin = A0; //pino em que o positivo do microbit está conectado
bool servoAtivado = false; // definição se o microbit está ligado ou desligado

void setup() { //definição do que é cada váriavél declarada acima
  Serial.begin(9600); //iniciar cominicação em 9600 bits por segundo serial entre o arduino/compuitador/sensores

  pinMode(triggerPin, OUTPUT); //definir o pino do trigger do sensor ultrassonico como saída
  pinMode(echoPin, INPUT); //definir pino do eco como entrada

  pinMode(ledPiscante, OUTPUT); //definir pino do led como saída
  digitalWrite(ledPiscante, LOW); //Iniciar LED no modo desligado (0V)

  pinMode(microbitPin, OUTPUT); //definir pino do microbit como saída
  digitalWrite(microbitPin, LOW); // Microbit inicialmente desligado

  for (int i = 0; i < numLeds; i++) { //laço de repetição
    pinMode(leds[i], OUTPUT); //todos os leds como saída
    digitalWrite(leds[i], LOW); //todos inciiam desligados
  }

  servoMotor.attach(servoPin); //uso da biblioteca do servo para configurar um servo motor
  servoMotor.write(0); // Posição inicial em 0º
}

void loop() { //inicio dos loops do código
  float distancia = medirDistancia(); //função de medir distancia do sensor ultrassonico  

  Serial.print("Distância: "); //indicação da distancia para o serial monitor
  Serial.print(distancia); //escreve o valor da variavel distancia
  Serial.println(" cm"); //adiciona a unidade cm ao valor calculado

  if (distancia > 0 && distancia < 10) { //ativação do sistema quando a distancia variar entre 0 e 10 cm
    if (!servoAtivado) { //após a deteção de um objeto realiza ativação do servo quando não ativo (estdo incial)
      moverServoSuave(0, 92);   // Moviemnta o servo motor até 92º e depois retorna a 0º criando um efeito de movimento
      digitalWrite(microbitPin, HIGH); // Liga o microbit após a detecção do objeto
      servoAtivado = true; //altera o estado do servo motor para evitar repetir o codigo
    }

    piscarLedIndependente(); //chama função piscar led 
    cascataLED(); //chama função cascata de led

  } else { //quando não detectar o objeto desliga todos os pinos
    if (servoAtivado) { //função para desligar o servo automaticamente quando o objeto não for detectado ou removido da frente do sensor ultrassonico
      moverServoSuave(92, 0);   // move o sensor para a posição incial (0º)
      digitalWrite(microbitPin, LOW); // Desliga o microbit
      servoAtivado = false; //volta o estado do servo motor para o estado inicial
    }

    apagarTodos(); //chama a função para apagar os leds da cascata 
    digitalWrite(ledPiscante, LOW); //apaga o led psicando da cachoeira
  }

  delay(100); //delay de leitura para nao sobrecarregar o processamento arduino
}

void piscarLedIndependente() { //função independente para o led da cachoeira - sem interferir no restante do programa
  unsigned long agora = millis(); //definir o tempo atual da ligação do programa
  if (agora - tempoAnterior >= intervaloPisca) { //verificação para evitar que o led acenda imediatamente ou que fique ligado após a não detecção do objeto
    tempoAnterior = agora; //atualiza a váriavél do tempo de oscilação do led
    estadoLed = !estadoLed; //altera o estado incial do ledpiscante para acender ele 
    digitalWrite(ledPiscante, estadoLed); //envia uma confirmação para o serial monitor indicando que o led acendeu 
  }
}

float medirDistancia() { //função para medir a disntancia detectada pelo sensor em cm 
//usar float para conseguir usar casas decimais
  digitalWrite(triggerPin, LOW); //define o pino trigger como 0V 
  delayMicroseconds(2); //delay para estabilizar o pino e evitar o uso de resistor pulldown (economia)
  digitalWrite(triggerPin, HIGH); //envia um pulso pelo sensor
  delayMicroseconds(10); //tempo de envio do pulso (tempo definido pelo fabricante)
  digitalWrite(triggerPin, LOW); //retorno ao 0V

  long duracao = pulseIn(echoPin, HIGH, 25000);
// Calcula o tempo que o pino Echo ficou em HIGH (tempo que o som levou para ir e voltar)
// Se não receber o eco em 25.000 microssegundos (25 ms), a função retorna 0 (timeout)  if (duracao == 0) return -1;
//Adicionar condição de ativação do código ser maior que 0, para evitar ativar quando o sensor retornar erro
//indicação de 0 da distancia no serial monitor indica erro do interno do sensor -- possivelmente ligação errada
  float distancia = duracao * 0.0343 / 2; //formula indicada pelo fabricante para converter o tempo do pulso em cm
  return distancia; //indica o valor da variavlél distancia que foi calculado nessa função
}

void cascataLED() { //função para ligar efeito de cascata nos leds da cachoeira
  for (int i = 0; i < numLeds; i++) { //laço de repetição para poder percorrer todas as linhas de led com incremento de 1 (i++)
    digitalWrite(leds[i], HIGH); //liga o pino na posição com mesmo valor da variavle i no momento
    delay(tempoDelay); //tempo que este permanece acesso
    digitalWrite(leds[i], LOW); //desliga esse pino e retorno ao começo, dessa vez com i com valor de i+1
  }
}

void apagarTodos() { //desliga todos os leds
  for (int i = 0; i < numLeds; i++) { //laço para manter os leds da cascata apagados
    digitalWrite(leds[i], LOW); //define os leds do aray em estado com 0V
  }
}

void moverServoSuave(int inicio, int fim) { //definição do movimento do servo motor conforme biblioteca de movimento
  if (inicio < fim) { //após chamar a função de ligar condiciona o inciio do programam a posição inicial de 0º para a posição final de 92º
    for (int pos = inicio; pos <= fim; pos++) { //laço de aumento da angulação de forma suave, incrmentando o valor do angulo
      servoMotor.write(pos); //joga a posição incrementada na posiçao do servo motor
      delay(100); // tempo de incremento de posição
    }
  } else {
    for (int pos = inicio; pos >= fim; pos--) { //condição para quando não acionado o servo motor fique na posição inicial
      servoMotor.write(pos); //retorna a posição de 0º
      delay(100); // delay de retorno
    }
  }
}




/*                                                                                                                                                      
                                                                                                                                                      
                                                                                                                                                      
                                                                                                                                                      
                                                                                                                                                      
                                                ##++                                      ####..                                                      
                                                ....##                                    ........                                                    
                                            ##........::                              ##........##                                                    
                                            ##........##                              ##........##                                                    
                                            ##......  mm                                mm....--                                                      
                                              ######  ..                            ##..######                                                        
                                                      ..++                        MM..##                                                              
                                                      ##..##                      ##..                                                                
                                                ####    MM..                    --..##      ######--                                                  
                                              ##mmMM##  ##  ##                  ##..      ##++mmMM##                                                  
                                              ##mmMMMM##  --##      ##########  ..@@    ##mmmmMMmmMM                                                  
                                              MMmm##mmMM####--##MM@@MMMMMMMM@@@@..####  ++MMMMmmMMmm##                                                
                                              MMmm..MMmm####MMMMMMMMMMMMMMMMMM####MMMM##mmMM##..MMmm##                                                
                                            ::mmmm..##@@@@MMMMMMMM@@MMMMMMMMMMMM@@MM##mmmm##----MMMM##                                                
                                            ::mmMM--##MMMMMMMM@@####MMMMMM@@MM@@MM##mmmm##------MMMM##                                                
                                              MMMM--MMMM##@@MMMMMMMMMM@@MM##MMMMMMMM++mm##..----mmMM##                                                
                                              MMMM####@@MMmm############mmMMMM##MMMMMM##----..--MMmm##                                                
                                              @@MM##MM@@##MMmmMMMMmmMMMMmm####MMMMMM@@MM--..----mmMM##                                                
                                              ####@@##MMMMMMMMmmMMMMmmmmmmMMMM##MMMMMMMM##------MMmm..                                                
                                              ####MMMM####mmMM@@mmmmmm######mmMM##MMmmMMMM##--..mm##                                                  
                                                MM##MMMMMM##MMMMMMMM##MMmm##mmmmmm##@@mmMMMM##mmmm##                                                  
                                              ##@@##mmMMMMmmMMMMmmmmMMMMMMMM##mmMMmmMMMMMMMM@@@@##MM                                                  
                                              ##MM##mmmmMMMMmmMMmmMMMMMMmmmmMMMMMMmm##MMMMMMMM@@mm@@                                                  
                                              ##MMmmmm####++MMmmmmmm######MM@@MMmmmm##MM@@MM@@MMMMMM                                                  
                                              ##MMmmMM##  ##MMmmMMmm##  ##MMMMmmmmmmMM@@MM@@MMMMMMMM                                                  
                                              @@MMMMMM######MMMMmmMM######MMMMmmmmmmmmMMMMMMMMMMMMMM                                                  
                                            ##mmMMMMmmmm##mmMMmmmmmmmm####mmmmMM..--MM##MMMM@@@@MM@@                                                  
                                          ##MMMM########mmmmmmmmMMMMmmMMmm----------mmmm##MMMMMMMM@@                                                  
                                          ##MM############MMmmMMMMMMmm....------------..MMMM##MM@@MM                        MM########                
                                            ##############----..--::..----##..----------mmmm@@##MMMM                ####::##########@@                
                                            ##  ########--..----------....####----::--MMmm++##@@MM@@              ##################                  
                                          ##MM++--####------------------##------------MMMMmm##MMMMMM            ##################                    
                                            ####::--##----------------##------------..MMmmmm##@@MMMM          ########@@############                  
                                                ##..@@------------####----..------..MMMM####@@@@MM##          ##########@@@@@@####                    
                                                MM################----..--..----..MMmmMM##MMMMMMMM##      ########################                    
                                                  ####--------------------------mmmm####MMMMMMMM##        ########################                    
                                                    ####@@----::::----..##########MMMMMMMM####          ######################@@##                    
                                                          ##mmmmmmMMMM@@MMMMMMMMMM@@@@@@##              ##################@@####--                    
                                                  ######MM##mmMMMMMMMM@@MMMMMM@@MMMMMM@@############    ################@@######..                    
                                                MMMMMMMM##########################@@MMMMMMMMMMMM######  ####################@@@@##                    
                                              ##MMMMMMMMMMMMMMMMmmMMMMMMMMMMMM@@MMMMMM@@MMMMMMMM@@mm@@  ######@@##@@######@@@@@@##                    
                                            ####MMMMMMMM@@MMMMmmMM@@@@MMMMMMMM@@MMMMMMMM##MMMM##MMMM@@##  ####@@##########MM@@@@##  ##                
                                          ##MM##MMMMMMMM##    ####@@@@MM########MMMMMMMM@@MM##MMMMMMMMMM##@@########MM####MM######  MM                
                                        ..MMMM##@@MMMM--..........####--........##MMMMMMMMMM##MMMMMMMMMMMM####@@####MM##@@MMMM##MM##mm##              
                                        ##MMMM@@MMMM##........................    ##MMMMMM@@##@@MMMMMM@@@@MM####MM##MMmm@@mmMMMMMMmmMM##              
                                      ##MMMMmm@@MMMM--....MMMMMM##....####..####....@@MMMMMM##MM@@MMMMMMMMMMMM##MMMMMMMMmmmmMMmmMMmmmmmmMM            
                                      MMMMMM##MMMMmm......MMMM@@MM##  MM##..MM@@....##MMMMMM##MMMM@@@@@@MMMMMMMMMMmmmmmmMMMMmmMMmmmmmmmm##            
                                    ##MMMM@@##MM@@MM......@@##..MMMM  MM##..MMMM    ##MMMM####MM@@MM@@@@@@MM@@MM##mmmmMMMMMMMMMMmmMMMMmm##            
                                    MMMMMMMM##MM@@MM......MM####MMMM..MM####MM@@....##MMMM##MMMMMMMMMMMM@@MMMMMMMM##MMMMMMmmmmMMmmmmMMMM##            
                                  ##MMMMMMMM##MMMMMM##....MM@@MMMM##..MMMMMMMMMM  ..MMMMMMMMMM##MMMM@@MMMMMMMMMMMMmmMMmmMMmmmmMMMMMMmmmm##  ##        
                                  MM@@MMMMMM@@MMMMMM##....MM##MMmm....MM##..MMMM..  MMMMMMMMMMMM##@@MM@@MMMMMMMMMMMM##mmMMmmMMmmmmmmMMMM######        
                                ##@@MMMM@@MMMMMM@@MM@@++..@@####@@++..@@##  MMMM..##MMMMMMMMMM@@##mm@@MMMMMM@@MMMMMMmmmmmmMMmmmmmmMMmmMM##MM##        
                              ##MMMMMMMM@@##MM@@@@@@MM##..MM##..MM##..MM##..MM@@  MMMMMMMMMMMMMMMM##@@MM@@MMMMMMMMMMMMmmmmmmMMMMmmmmmmmm##mm##        
                              MMMMMMMMMMMM##MMMMMMMMMM@@##......mm@@..####..######MM@@MMMMMMmmMMMM@@MMMMMMMMMMMMMM@@MMmmmmMMmmmmMMMMMMmmMMMM          
                            ##MMMMMMMMMM@@##@@MM@@MMMMMM@@##..................##MMMMMMMMMMMM##@@MMMM##@@MMMMMMMMMMMM@@mmMMMMmmMMmmMM++mmMM##          
                          ####mmMM@@MMMMMM##MM@@MMMMMMMM@@MM--..............##MMMMMMMM@@MMMM##MMMM@@##MMMMMMMM@@MMMMMMMMmmmmMMMMmmMMmmMMmm##          
                          MM######MMMMMMMMMMMMMMMMMM@@MMMMMMMM@@..........##MMMMMMMMMMMMMMMM##MMMMMM##MMMMMMMMMMMMMMMMmmmmmmmmmmmmMMMMmmmm            
                    ########  ..##MMMMMMMMMMMM@@MMMMmmMMMMMMMMMM##....##mmMMMMMMMMMMMMMMMMMMmmMMMMMM##MMMMMMMMMMMM@@mmMMMMmmMMMMMMmmmmmm##            
                ##........  ####..####MM##MMMMMMMMMMMMMMMMMMMM@@MM####mmMMMMMMMMMMMMMMMMMMMMMM@@@@MM##@@MMMM@@@@MMMM@@mmmmmmMMMMmmmmMM######          
              ##....MM--##..  ##....MM@@##MMMMMM@@MMMMMMMMMMMMMMMMMMMMMMMMMM@@MMMMMM@@MMMMMMMM@@MMMM##MMMMMMMMMMMMMM##mmmmmmMMmmMMMM++##mm##          
              ....::::::::++....####MM####MM@@MMMM@@MMMMMM@@MMMMMMMMMMMMMMMMMMMMMM@@@@MMMMMMMMMMMM@@##mmMM####mm##MM##mmMMmmMMMMmmMMMMMM##            
            ##..##::::----MM........##  ##MMMMMMMMMMmmMMMM@@MMMMMMMMMM@@MMMM@@MM@@mmMMMM@@MMMM@@MM@@##@@........##MM##MMmmMMmmMMmmmmmm##              
            ##..--::::::........  ##      ##MMMMMM##############@@mmMMMMMMMMMMMMMM@@MMMM@@MMMM##@@@@####........####mmmmMMMMMMMMmmMM##                
            ##++::::::##  ..##......      @@MMMM@@MMMMMMMMMMMMMMMMMMMMMMMMmm@@##############@@####MM##....########MMMMMMMMmmMMmm++##                  
              ##--::mm##  ##......        @@MM@@MMMMMM@@MMMMMM@@MMMMMMMMMMMMMMMMMMMMMMMMMMMM##MMmm  ........  --..--++mmMMmm++##                      
                ####  ##  ........##      @@MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM@@MMMM@@@@MMMMMM##@@##..##::::##..##......mmmmMMmmMM##                    
                        ##......##          ##@@############@@MMMMMMMM@@MMMMMMMMMMMMMMMMMMMM##mm..##::----MM..  ......##MM####                        
                            ####            MM@@MMMMMMMMMM@@MMMMMMMMmm################MM@@MM####..##--::::    ........##                              
                                          ##MM@@@@MMMMMMMMMMMMMM##MMMM##MMMMMMMMMMMMMMMMMM@@MM##  ##::::##  --  ......##                              
                                          ##MMMMMMMMMMMMMMMMMMMM##MMMMMMMM@@MMMMMMMMMMMMMMMM@@@@....::::####..##......                                
                                          ##MMMMMMMMMMMM@@@@MMMMMMMM@@@@@@MM@@MM@@@@@@MMMMMM@@MM####::--####........##                                
                                          ##mm@@MMMMMM@@MMmm@@MMMM@@MMMMMMMMMMMMMM@@MMMMMM@@##@@MMMMMM@@##    ######                                  
                                          ##MMMMMM@@@@@@@@MMMMMMmmMMMMMMMMMMMMMMMMMMMMMMMMMM##MMMMMMMMMM##                                            
                                          ##MMMMMM@@MMMMMMMMMMMM##MMMMMMMMMMMMMM@@MMMMMMMMMM##MMMMMM@@MM##                                            
                                            @@MMMMMMMMMMMMMMMMMM##MMMM##MMMMMMMMMMMMMMMM@@MM@@MM@@MMMMMM##                                            
                                            MMMMMMMM########MMMM@@MMMM##MMMMMMMMMMMM@@MMMMMM@@MMMM@@@@MM##                                            
                                            MMMM@@MMMMMMMMMMMMMMMM############MMMM@@MMMMMMMMMM##MMMMMMMM##                                            
                                            @@MMMMMMMMMMMMMMMM@@MM##MMMM@@MMMMMMMMMMMMMMmm######MMMMMM@@##                                            
                                            MMMM@@MM@@MMMM@@MMMMMM##@@MM@@@@MM@@MMMMMMMMMMMMMMMMMMMMMMMM##                                            
                                          MMMMmm@@MMMMMMMMMMMMMM@@##MMMM@@MMMMMMMMMMMMMMMM@@MMMMMMMMMMMM##                                            
                                          @@MMMMMMMMMMMMMMMMMMMMMM##MM@@@@@@MMMM@@MMMMMMMM@@MMMMMMMMMMMM##                                            
                                          ##MM@@MMMMMMMMMMMMMMMM@@##MMMM@@MMMMMMMMMMMMMMMMMM@@MMMM@@MMMM##                                            
                                          ##..  ....######mmMMMM@@##mm##@@@@MMMMMMMMMM@@@@@@@@MMmmMMMMMM##                                            
                                          ##......................##::::######mmMMMMMMMMMMMMMMMMMMMMMM::##                                            
                                        ##..  ######  ............##--::++............  @@####@@@@MM....##                                            
                                          MM####++..  ......  ######::::mm..  ..................mm......@@                                            
                                          ..      ......  ####@@..--::##  @@####......  ..  ....MM......--                                            
                                        ..##MMMM########......--::::##mm######  ........  @@####MM......                                              
                                        ..MM####@@##mmMMMM  ..::::--##..--..........--..####mm  ........                                              
                                        --mmMMMM@@##MM##MM##..++::::##..##@@@@########  ....@@..........                                              
                                      ####mmMMMM######MMMM  ..MM--::##..MM##@@MM##@@@@MM  ..MM............                                            
                                    @@..  ##..##mm..........##@@--++##..MMMMMMMM##MM##mm##..::..........--                                            
                                  ##..........##..............++::::######mm####mm##MM@@##..............##                                            
                                ##..............##............##..::++##--##@@MM##........##  ......##....                                            
                                ##  ....  ........mm......##::::::..........##..............  ....  ......                                            
                                ##....####........++##--::::::::##............##....  ......####..  ..##                                              
                                ##......................::..##    ..............##........##......@@##                                                
                                  MM##  ..............####    ..  ##..................##........##                                                    
                                          @@######@@          ..........  @@####mm..........##                                                        
                                                                ##  ....................####                                                          
                                                                  ::####mm  ....  @@####                                                              
                                                                                                                                                      
                                                                                                                                                      
                                                                                                                                                      
*/                                                                                                                          
                                                                                                                                                      
                                                                                                                                                      
