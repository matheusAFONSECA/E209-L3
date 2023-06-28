#include <stdio.h>
#include <stdlib.h>
#define FOSC 16000000U // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC / 16 / BAUD - 1
#define LED PD6
#define BotaoInfo PD2

// var aux
int volume;         // volume que sera inserido pelo monitor serial
int tempo;          // tempo que sera inserido pelo monitor serial
int flagVolume = 1;     // flag de controle de entrada de volume
int flagVolume2 = 1;     // flag de controle de entrada de volume
int controle = 0;

char msg_tx[20];
char msg_rx[32];
int pos_msg_rx = 0;
int tamanho_msg_rx = 3;
int brilho;
unsigned int x = 0, valor = 0;

//Prototipos das funcoes
void UART_Init(unsigned int ubrr);  // essa função pega a informação do monitor serial

void UART_Transmit(char *dados);    // essa função transmite a mensagem para o monitor serial

void PegarVolume() {    // função para pegar o volume de tempo que foram colocados atraves do UART

  UART_Transmit("ENTRE COM O VOLUME: ");

  if (flagVolume == 1) {
    while(controle == 0 && volume < 0){
      volume = (msg_rx[0] - 48) * 100 + (msg_rx[1] - 48) * 10 + (msg_rx[2] - 48) * 1;
      controle++;
    }
    itoa(volume, msg_tx, 10);
    UART_Transmit(msg_tx);
    UART_Transmit("\n");

    if (volume > 99 && volume < 1000) {
      UART_Transmit("\nENTRE COM O TEMPO DE INFUSAO EM MIN: ");
      tempo = (msg_rx[0] - 48) * 100 + (msg_rx[1] - 48) * 10 + (msg_rx[2] - 48) * 1;

      if (tempo > 0 && tempo < 60) {
        itoa(volume, msg_tx, 10);
        UART_Transmit(msg_tx);
        UART_Transmit("\n");

        itoa(tempo, msg_tx, 10);
        UART_Transmit(msg_tx);
        UART_Transmit("\n");

        controle = 0;                 // controle de entrada do volume
        flagVolume = 0;               // flag de controle setada para ZERO

      }
    }

  }
}

ISR (INT0_vect) {   // função que vai setar a flag de entrada de dados como UM
  flagVolume = 1;   // para a entrada de dados pela função PegarVolume -> 1
  PegarVolume();
}

int main(void)
{
  // configuraçoes PWM
  DDRD |= (1 << LED); // configura saída para o PWM

  // Configura modo FAST PWM e modo do comparador A
  TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1);
  TCCR0B = 1; // Seleciona opção para frequência
  OCR0A = 0;

  // configurações de botão que controla a entrada de dados
  PORTD |= (1 << BotaoInfo);
  EICRA = 0b00000011;
  EIMSK = 0b00000001;

  UART_Init(MYUBRR);
  sei();

  while (1) {

  }

}

ISR(USART_RX_vect)
{
  // Escreve o valor recebido pela UART na posição pos_msg_rx do buffer msg_rx
  msg_rx[pos_msg_rx++] = UDR0;
  if (pos_msg_rx == tamanho_msg_rx)
    pos_msg_rx = 0;
}

void UART_Transmit(char *dados)
{
  // Envia todos os caracteres do buffer dados ate chegar um final de linha
  while (*dados != 0)
  {
    while (!(UCSR0A & (1 << UDRE0))); // Aguarda a transmissão acabar
    // Escreve o caractere no registro de tranmissão
    UDR0 = *dados;
    // Passa para o próximo caractere do buffer dados
    dados++;
  }
}

void UART_Init(unsigned int ubrr)
{
  // Configura a baud rate */
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  // Habilita a recepcao, tranmissao e interrupcao na recepcao */
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  // Configura o formato da mensagem: 8 bits de dados e 1 bits de stop */
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}
