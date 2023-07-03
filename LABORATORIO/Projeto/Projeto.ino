// configurações para usar a UART
#define FOSC 16000000U               // Frequência do oscilador do microcontrolador
#define BAUD 9600                    // Taxa de transmissão para comunicação UART
#define MYUBRR FOSC / 16 / BAUD - 1  // Cálculo do valor do registrador UBRR para configurar a comunicação UART

// configurações de pino de sensor de gotas
#define SensorGotas PD2
#define Motor (1 << PD6)
//#define Trimpot (1 << PC0)
#define Buzzer (1 << PD4)

// bibliotecas para manipular strings e booleanos
#include <stdio.h>        // -> manipulação de strings
#include <stdbool.h>      // -> utilização do tipo booleano
#include <avr/io.h>

// VAR AUX
int volume;                   // volume em ml
int tempo;                    // tempo em min
char volumeAux[4];            // var aux para pegar a string do volume do buffer
char tempoAux[4];             // var aux para pegar a string do tempo do buffer
bool controle;                // booleano para controle de entrada de dados
char opcao[4];                // char que pega a opção de mudar os valores, volume e tempo, ou não
float fluxo;                  // fluxo que sera gerado conforme o tempo de infusão e o volume
char fluxoNecessarioAux[10];  // string aux para retornar um valor String para o monitor serial
float potencia;               // define a potência que o motor irá trabalhar
char potenciaAux[10];         // string aux para retornar um valor String para o monitor serial
char resposta[100];           // char que aux para mostrar os valores que foram atribuidos para o tempo e volume
int numeroGotasDetectadas;    // numero de gotas detectadas por pelo sensor de gotas
int controlenumerodegotas;    // controle da quantidade de gotas
int pot;                      // potência que o motor irá funcionar
double tensao;                // tensão que está sendo aplicada pelo trimpot
int distancia;                // distancia que esta sendo detectada
int contador;                 // contador aux para calculo do tempo -> tempo no em segundos desde que a primeira gota caiu
float erro;                   // var para calculo do erro
float fluxoReal;              // var para o calculo do fluxo real
char erroAux[10];             // string aux para retornar um valor String para o monitor serial

// FUNÇÕES
// funções para usar a UART
void UART_Init(unsigned int ubrr) {      // inicializa a comunicação com o monitor Serial
  // Configura a baud rate
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;

  // Habilita a recepção, transmissão e interrupção na recepção
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);

  // Configura o formato da mensagem: 8 bits de dados e 1 bit de stop
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_Transmit(char *dados) {       // faz a tranmissão de dados(string) para o monitor serial
  while (*dados != 0) {
    while (!(UCSR0A & (1 << UDRE0)))    // cria um loop que aguarda até que o registrador de buffer de transmissão esteja vazio
      ;
    UDR0 = *dados;
    dados++;
  }
}

void UART_Receive(char *dados, int tamanho) {     // recebe os dados pelo monitor serial

  int i;
  for (i = 0; i < tamanho - 1; i++) {
    while (!(UCSR0A & (1 << RXC0)))
      ;
    char caractere = UDR0;

    if (caractere == '\n') {
      i--;  // Descarta o caractere '\n' e lê o próximo caractere
      continue;
    }

    dados[i] = caractere;
    UART_Transmit(&dados[i]);  // Ecoa o caractere recebido de volta
  }
  dados[i] = '\0';
  UART_Transmit("\n");
}

float fluxoNecessario(int tempoFluxo, int volumeFluxo) {    // função que faz o calculo do fluxo necessario do motor

  return ((float)volumeFluxo) / ((float)tempoFluxo / 60);

  /*
    -> coloquei (float)volume dessa forma para calcular porque precisava de rotornar um valor em float,
    mas o tipo da var é int
  */
}

ISR(INT0_vect) {        // Rotina de tratamento da interrupção INT0 para o SensorDeGotas
  numeroGotasDetectadas++;
  controlenumerodegotas = 1;
}

void motorFunciona(int potenciaMotor) {     // faz o motor funcionar

  pot = map(potenciaMotor, 0, 100, 0, 255);   // coloca a leitura do POT entre 0 e 255
  OCR0A = pot;                                // define a potência do motor

}

int ADC_read(u8 ch) {     // faz a leitura ADC do pino
  char i;
  int ADC_temp = 0;                     // ADC temporário, para manipular leitura
  int ADC_read = 0;                     // ADC_read
  ch &= 0x07;
  ADMUX = (ADMUX & 0xF8) | ch;          // Configura o canal de leitura do ADC
  ADCSRA |= (1 << ADSC);                // Inicia uma conversão
  while (!(ADCSRA & (1 << ADIF)))
    ;                                   // Aguarda a conclusão da conversão
  for (i = 0; i < 8; i++) {             // Realiza a conversão 8 vezes para maior precisão
    ADCSRA |= (1 << ADSC);              // Inicia uma conversão
    while (!(ADCSRA & (1 << ADIF)))
      ;                                 // Aguarda a conclusão da conversão
    ADC_temp = ADCL;                    // Lê o registro ADCL
    ADC_temp += (ADCH << 8);            // Lê o registro ADCH
    ADC_read += ADC_temp;               // Acumula o resultado (8 amostras) para média
  }
  ADC_read = (ADC_read / 8);            // Calcula a média das amostras

  return ADC_read;                      // retorna a leitua feita no pino PC0
}

// Rotina de interrupção do Timer2 (ocorre quando o contador do Timer0 estoura)
ISR(TIMER2_OVF_vect) {
  if (controlenumerodegotas == 1) {           //quando detectar a primeira gota inicia a contagem
    if (contador > (tempo * 60 * 61)) {       // quando o contador atingir o tempo(min) ele desliga o motor
      contador = 0;                           // zera a contagem de tempo
      controlenumerodegotas = 0;
      motorFunciona(0);

      /*
        -> tempo * 60 * 61: converte o tempo de min para segundos e depois converte para a numeração que o nosso timer está
        trabalhando
        -> 1 segundo = 61
      */
    }
    contador++;                               // incrementa o contador(seg)
  }
}

void configuracoes() {    // define as configurações necessárias para o código rodar
  // inicia a comunicacao UART
  UART_Init(MYUBRR);

  // Configurando o pino do sensor de gotas como entrada digital
  // Habilita o resistor interno de pull-up no pino 2 (PD2)
  PORTD = 0b00000100;
  // Configura a interrupção externa 0 para transição de descida
  EICRA = 0b00000010;
  // Habilita a interrupção externa 0
  EIMSK = 0b00000001;

  // configuração do PWM -> Motor
  DDRD |= Motor; // configura saída para o PWM
  PORTD &= ~Motor; // PWM inicia desligado
  // Configura modo FAST PWM e modo do comparador A
  TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1);
  TCCR0B = 1; // Seleciona opção para frequência
  OCR0A = 0;

  // configuração do PWM -> trimpot para acionar o buzzer
  // Configura o Timer1 para operar em modo Fast PWM de 8 bits
  TCCR1A |= (1 << WGM10) | (1 << COM1A1);
  TCCR1B |= (1 << WGM12) | (1 << CS10);
  // Define o valor inicial do comparador A (OCR1A)
  OCR1A = 0;

  // inicia o buzzer desligado
  PORTD &= ~Buzzer;

  // configuração ADC -> ajudar na leitura do PC0
  // Configura a referência de tensão do ADC
  ADMUX = (1 << REFS0);
  // Habilita o ADC e configura o prescaler
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  // Configura o Timer2 para operar em modo normal
  TCCR2A = 0;
  TCCR2B = 0;
  // Define o prescaler para 256
  TCCR2B |= (1 << CS22);
  // Define o valor inicial do contador do Timer2
  TCNT2 = 0;
  // Habilita a interrupção do overflow do Timer2
  TIMSK2 |= (1 << TOIE2);

  // Habilita a interrupção global
  sei();

  // variáveis para o sensor de gotas
  controlenumerodegotas = 0;    // controla o numero de gotas
  numeroGotasDetectadas = 0;    // quantidade de gotas detectadas

  // inicializando o contador como zero
  contador = 0;

}

void executa() {    // função que vai rodar em loop de execução do sistema
  // setando o volume e tempo como zero porque reiniciou a entrada de dados
  volume = 0;     // volume medido em ml
  tempo = 0;      // tempo medido em min

  // booleano que faz o controle da entrada de dados
  controle = false;

  // Limpa o buffer volumeAux -> necessário para entrada de dados posterior
  for (int i = 0; i < sizeof(volumeAux); i++) {
    volumeAux[i] = '\0';
  }

  // Limpa o buffer tempoAux -> necessário para entrada de dados posterior
  for (int i = 0; i < sizeof(tempoAux); i++) {
    tempoAux[i] = '\0';
  }

  // Entrada de dados
  UART_Transmit("Entrando com o dados:\n");

  // Entrada do volume
  UART_Transmit("Volume(ml): ");
  UART_Receive(volumeAux, 4);     // faz a entrada do volume -> ml
  volume = atoi(volumeAux);       // atribui o volume que foi pego pelo monitor serial

  // Entrada do tempo de infusão
  UART_Transmit("Tempo de infusao(min): ");
  UART_Receive(tempoAux, 4);      // faz a entrada do tempo -> min
  tempo = atoi(tempoAux);         // atribui o tempo que foi pego pelo monitor serial

  // Cálculo do fluxo necessário
  fluxo = fluxoNecessario(tempo, volume);

  // calculo da potência do motor
  potencia = (fluxo / 450) * 100;

  // Saindo com os valores inseridos pelo monitor serial
  UART_Transmit("Dados inseridos pelo monitor, fluxo e potencia:\n");
  sprintf(resposta, "Volume: %d ml\nTempo de infusao: %d h\n", volume, tempo / 60);
  UART_Transmit(resposta);

  dtostrf(fluxo, 4, 2, fluxoNecessarioAux);     // transformando o fluxo em string para saída no monitor serial
  UART_Transmit("Fluxo necessario: ");
  UART_Transmit(fluxoNecessarioAux);
  UART_Transmit(" ml/h \n");

  dtostrf(potencia, 4, 2, potenciaAux);        // transformando a potencia em string para saída no monitor serial
  UART_Transmit("Potencia: ");
  UART_Transmit(potenciaAux);
  UART_Transmit(" %\n");

  // outro loop que faz o sistema rodar o motor -> buzzer desligado
  do {
    // Limpa o buffer da opcao -> necessário para entrada de dados posterior
    for (int i = 0; i < sizeof(opcao); i++) {
      opcao[i] = '\0';
    }

    // acionamento ou não do buzzer
    u16 leitura = ADC_read(ADC0D);          // pega a leitura do pino analógico PC0
    tensao = leitura * 5.0 / 1023;          // faz o calculo da tensao que está sendo aplicada pelo pino
    distancia = (20 - tensao * 4);          // faz a corelação entre a tensão e a distancia

    if (distancia < 1.25) {  // caso algo seja detectado a menos de 5cm

      PORTD |= Buzzer;   // liga o buzzer
      tone(4, 1000);     // Frequência de 1000 Hz
      motorFunciona(0);  // desliga o motor
    } else {             // Desligar o buzzer
      noTone(4);
    }

    // analisa qual ação o usuário deseja fazer
    UART_Transmit("Deseja alterar algum valor? ");
    UART_Receive(opcao, 4);   // recebimento da opcao escolhida

    if (opcao[0] == 's' && opcao[1] == 'i' && opcao[2] == 'm') {
      controle = true;        // volta pra colocar as informações

      PORTD &= ~Buzzer;       // desliga o buzzer

      OCR0A = 0;              // desliga o motor

      numeroGotasDetectadas = 0;    // zera a contagem do numero de gotas

    }
    else if (opcao[0] == 'n' && opcao[1] == 'a' && opcao[2] == 'o') {
      // mostra os valores calculados do erro e deixa o motor funcionando

      // chamando a função que faz o motor funcionar
      int op = int(potencia);   // var aux para mandar a potencia que o motor ira trabalhar
      motorFunciona(op);

      // calculo do erro, fluxo real e saida vo valor de erro no monitor serial
      fluxoReal = numeroGotasDetectadas / ((contador / 61) * 60) * 0.05;               // primeiro eu converti o contador para min
      erro = ((fluxoReal - fluxo) / fluxo) * 100.0;
      dtostrf(erro, 4, 2, erroAux);                                               // transformando o erro em string para saída no monitor serial
      UART_Transmit("Erro: ");
      UART_Transmit(erroAux);
      UART_Transmit(" %\n");


      // saida da quantidade de gotas
//      sprintf(resposta, "Numero de gotas: %d \n", numeroGotasDetectadas);
//      UART_Transmit(resposta);

    }
    // entrada inválida na opção de escolha
    else {
      UART_Transmit("Opcao invalida!!\n");
      UART_Transmit("Coloque sim ou nao\n");
      controle = false;
    }

  } while (controle == false);
}

int main() {  // funcao principal

  //chama função que faz as configurações
  configuracoes();

  while (1) {
    executa();    // chama a função que realiza a execução do sistema -> fica em loop
  }

  /*
    -> separamos a int main dessa forma para facilitar a programação do código, uma
    vez que cada um faz uma função do código, daí é só adicionar a parte que foi feita
    e chamar a função que a realiza
  */

}
