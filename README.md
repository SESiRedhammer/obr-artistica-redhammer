
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

# 🌊🤖 Projeto "Paraíso das Cachoeiras" - OBR Artística 2025                                                                                 

---
## 👨‍🎓 Equipe
**REDHAMMER - OBR Artística 2025**  
Juh
Isaac
Nicolas
Bea

---
**Controle do Robô Artístico - Equipe REDHAMMER**  
**Escola SESI Antônio Scalon - CE 423 - Presidente Prudente (SP)**  
**Versão do Código: `1.15.6`**

---

## ✨ Sobre o Projeto

Este código foi desenvolvido para o robô artístico apresentado na modalidade **OBR Artística 2025**, com o tema _"Paraíso das Cachoeiras"_. A proposta une **movimento mecânico**, **interação sensorial** e **efeitos visuais com LEDs**, criando uma instalação robótica que encanta pela sincronia e fluidez das ações.

> 🎭 A beleza da arte se une à precisão da robótica.

---

## 🎯 Objetivo

Criar uma experiência artística e sensorial que simula uma **cachoeira interativa**, ativada por proximidade. A instalação conta com:

- LEDs sequenciais simulando o **fluxo de água**
- Um servo motor que movimenta o boneco "Kevin" em resposta ao visitante
- Um micro:bit ativado automaticamente para efeitos adicionais
- Efeitos de luz intermitente simulando o brilho da água

---

## 🔧 Tecnologias e Componentes Usados

- **Arduino Mega**
- **Sensor Ultrassônico (HC-SR04)**
- **Servo Motor**
- **Micro:bit (acionado via sinal de 5V)**
- **LEDs 5mm** organizados em uma estrutura em cascata
- **Protoboard + Fios + Resistores**
- **Estrutura física artística feita com materiais recicláveis e elementos naturais (como pedras e folhas artificiais)**

---

## 🧠 Como Funciona?

Ao detectar um visitante a menos de 10cm da instalação:

1. 💡 O LED principal começa a piscar independentemente.
2. 🌈 A cascata de LEDs inicia um efeito de descida.
3. 🦾 O boneco "Kevin" se movimenta suavemente com o servo.
4. 🔌 O micro:bit é ativado para efeitos visuais ou sonoros complementares.

Quando não há ninguém por perto:

- Todos os LEDs são desligados.
- O servo retorna à posição inicial.
- O micro:bit é automaticamente desligado.

---

## 📂 Organização do Código

O código está **minuciosamente comentado**, com cada trecho explicando:

- A função dos pinos e variáveis
- As decisões de lógica e controle
- Os efeitos visuais e motores
- O funcionamento do sensor de proximidade

> 🧼 Nosso objetivo foi garantir que **qualquer avaliador** consiga entender **a lógica completa** com uma simples leitura.

---

## 🧪 Testado e Aprovado

O sistema foi **rigorosamente testado** em diversas condições de luz e distância, garantindo:

✅ Alta responsividade  
✅ Estabilidade na ativação do servo  
✅ Efeitos visuais sincronizados  
✅ Segurança e confiabilidade durante a exposição

---

## 💡 Destaques Técnicos

- 🌀 **Função `moverServoSuave()`**: garante movimentos fluídos e naturais
- 🧠 **Função `medirDistancia()`**: com tratamento de erro para falsos positivos
- 🔁 **Cascata de LEDs**: simula água caindo com ritmo suave e realista
- 🧩 **Código Modular**: separado por funções para facilitar manutenção e evolução

---

## 📸 Imagens do Projeto (Adicione Aqui!)

- Foto do robô finalizado
- Vídeo da cachoeira em funcionamento
- Bastidores da construção

---

## 📥 Clonando o Projeto

```bash
git clone https://github.com/sua-conta/redhammer-paraiso-cachoeiras.git
