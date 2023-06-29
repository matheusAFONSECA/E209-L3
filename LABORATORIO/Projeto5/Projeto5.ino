// configurações para usar a UART
#define FOSC 16000000U               // Frequência do oscilador do microcontrolador
#define BAUD 9600                    // Taxa de transmissão para comunicação UART
#define MYUBRR FOSC / 16 / BAUD - 1  // Cálculo do valor do registrador UBRR para configurar a comunicação UART

// bibliotecas para manipular strings e booleanos
#include <stdio.h>        // -> manipulação de strings
#include <stdbool.h>      // -> utilização do tipo booleano

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

float fluxoNecessario(int tempoFluxo, int volumeFluxo){     // função que faz o calculo do fluxo necessario do motor 
  return ((float)volumeFluxo) / ((float)tempoFluxo / 60);

  /*
  -> coloquei (float)volume dessa forma para calcular porque precisava de rotornar um valor em float,
  mas o tipo da var é int
  */
}

void configuracoes() {    // define as configurações necessárias para o código rodar
  // inicia a comunicacao UART
  UART_Init(MYUBRR);

  sei();
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
  sprintf(resposta, "Volume: %d ml\nTempo de infusao: %d min\n", volume, tempo);
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
    UART_Transmit("Deseja alterar algum valor? ");
    UART_Receive(opcao, 4);   // recebimento da opcao escolhida

    if (opcao[0] == 's' && opcao[1] == 'i' && opcao[2] == 'm') {
      controle = true;        // volta pra colocar as informações
    }
    else if (opcao[0] == 'n'&& opcao[1] == 'a' && opcao[2] == 'o') {
      // mostra os valores calculados do erro e deixa o motor funcionando
    }
    //Entrada inválida
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
    executa();    // chama a função que realiza a execução do sistema
  }

  /*
  -> separamos a int main dessa forma para facilitar a programação do código, uma
  vez que cada um faz uma função do código, daí é só adicionar a parte que foi feita
  e chamar a função que a realiza
  */

}
