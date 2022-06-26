// Define os pinos do arduino para os leds
#define LED_VERDE 2 
#define LED_AMARELO 3
#define LED_VERMELHO 4
#define LED_AZUL 5


// Define os pinos do arduino para os botões
#define BOTAO_VERDE 8
#define BOTAO_AMARELO 9
#define BOTAO_VERMELHO 10
#define BOTAO_AZUL 11

// Define as medidas de tempo para o jogo
#define UM_SEGUNDO 1000
#define MEIO_SEGUNDO 500
#define QUARTO_SEGUNDO 250

// Tamanho do jogo
#define TAMANHO_SEQUENCIA 4

// Ind
#define INDEFINIDO -1

enum Estados { // Enumeração dos estados do jogo
  PRONTO_PARA_PROXIMA_RODADA,
  USUARIO_RESPONDENDO,
  JOGO_FINALIZADO_SUCESSO,
  JOGO_FINALIZADO_FALHA
};

int sequenciaLuzes[TAMANHO_SEQUENCIA];

int rodada = 0;

int ledsRespondidos = 0;

void setup(){
  Serial.begin(9600);
  
  iniciaPortas();
  iniciaJogo();

}

void iniciaPortas(){
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  pinMode(BOTAO_VERDE, INPUT_PULLUP);
  pinMode(BOTAO_AMARELO, INPUT_PULLUP);
  pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
  pinMode(BOTAO_AZUL, INPUT_PULLUP);

}

void iniciaJogo(){
  
  int jogo = analogRead(0); //Lê o valor do sensor de luz
  randomSeed(jogo); //Inicia o gerador de números aleatórios com o valor do sensor de luz

  for (int i = 0; i < TAMANHO_SEQUENCIA; i++) {
    sequenciaLuzes[i] = sorteiaCor(); //Sorteia uma cor e armazena na sequencia de luzes

  }

}

int sorteiaCor(){
  return random(LED_VERDE, LED_AZUL + 1); // +1 para incluir o ultimo valor

}

void loop(){
  switch (estadoAtual()){
    case PRONTO_PARA_PROXIMA_RODADA:
      Serial.println("Pronto para a proxima rodada");
      preparaNovaRodada();
      break;
    case USUARIO_RESPONDENDO:
      Serial.println("Usuario respondendo");
      processaRespostaUsuario();
      break;
    case JOGO_FINALIZADO_SUCESSO:
      Serial.println("Jogo finalizado com sucesso");
      jogoFinalizadoSucesso();
      break;
    case JOGO_FINALIZADO_FALHA:
      Serial.println("Jogo finalizado com falha");
      jogoFinalizadoFalha();
      break;
  }
  delay(QUARTO_SEGUNDO); // ou MEIO_SEGUNDO

}

void preparaNovaRodada(){ // prepara a proxima rodada
  rodada++; // incrementa a rodada
  ledsRespondidos = 0; // zera o contador de leds respondidos
  if(rodada <= TAMANHO_SEQUENCIA){
    tocaLedsRodada();

  }

}

void processaRespostaUsuario(){ // verifica se o usuario respondeu corretamente
  int resposta = checaRespostaJogador();

  if(resposta == INDEFINIDO){
    return;

  }
  
  if(resposta == sequenciaLuzes[ledsRespondidos]){ // se a resposta for correta
    ledsRespondidos++;

  }
  else{
    Serial.println("Sequencia errada"); // se a resposta for errada
    rodada = TAMANHO_SEQUENCIA + 2;

  }

}

int estadoAtual() { // verifica o estado atual do jogo
  if(rodada <= TAMANHO_SEQUENCIA){
    if (ledsRespondidos == rodada){ // se o usuario respondeu corretamente
      return PRONTO_PARA_PROXIMA_RODADA;  // pronto para a proxima rodada

    }
    else{
      return USUARIO_RESPONDENDO; // usuario esta respondendo

    }
    
  }
  else if(rodada == TAMANHO_SEQUENCIA + 1){ // se o usuario respondeu corretamente todas as vezes
    return JOGO_FINALIZADO_SUCESSO; // jogo finalizado com sucesso

  }
  else{
    return JOGO_FINALIZADO_FALHA;

  }

}

void tocaLedsRodada() { // toca as luzes da rodada
  for (int i = 0; i < rodada; i++){ // para cada led da rodada
    piscaLed(sequenciaLuzes[i]); // pisca o led da sequencia

  }

}

int checaRespostaJogador(){ // verifica se o usuario respondeu corretamente
  if(digitalRead(BOTAO_VERDE) == LOW){ // se o usuario pressionou o botao verde
    return piscaLed(LED_VERDE); // pisca o led verde

  }
  if(digitalRead(BOTAO_AMARELO) == LOW){
    return piscaLed(LED_AMARELO);

  }
  if(digitalRead(BOTAO_VERMELHO) == LOW){
    return piscaLed(LED_VERMELHO);

  }
  if(digitalRead(BOTAO_AZUL) == LOW){
    return piscaLed(LED_AZUL);

  }

  return INDEFINIDO; // se o usuario nao pressionou nenhum botao

}

void jogoFinalizadoSucesso(){ // piscando todos os leds em sequencia
  piscaLed(LED_VERDE); 
  piscaLed(LED_AMARELO);
  piscaLed(LED_VERMELHO);
  piscaLed(LED_AZUL);
  delay(MEIO_SEGUNDO);

}

void jogoFinalizadoFalha(){ // piscando todos os leds de uma vez
  digitalWrite(LED_VERDE,HIGH);
  digitalWrite(LED_AMARELO,HIGH);
  digitalWrite(LED_VERMELHO,HIGH);
  digitalWrite(LED_AZUL,HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(LED_VERDE,LOW);
  digitalWrite(LED_AMARELO,LOW);
  digitalWrite(LED_VERMELHO,LOW);
  digitalWrite(LED_AZUL,LOW);
  delay(MEIO_SEGUNDO);

}

int piscaLed(int portaLed){ // pisca o led da porta passada como parametro

  verificaSomDoLed(portaLed); // verifica o som do led

  digitalWrite(portaLed,HIGH); // pisca o led
  delay(UM_SEGUNDO); // espera um segundo - delay do led aceso
  digitalWrite(portaLed,LOW); // desliga o led
  delay(MEIO_SEGUNDO); // espera um meio segundo - delay do led desligado

  return portaLed; // retorna a porta do led
}
