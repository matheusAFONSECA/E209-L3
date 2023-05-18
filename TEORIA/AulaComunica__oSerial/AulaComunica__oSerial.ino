int main()
{
  int i = 0;                                // inicializa a posição da string ' INATEL '
  unsigned char dado[] = " INATEL ";
  UBRR0 = 103;
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);  // TAMANHO DO BIT (8 BITS)
  UCSR0C |= (1 << USBS0);                   // DOIS BITS DE PARADA
  UCSR0B |= (1 << TXEN0);                   // LIGA O TRANSMISSOR
  UCSR0B |= (1 << RXEN0);                   // LIGA O RECEPTOR

  while (1){
      while(dado[i] != 0){
        while(!(UCSR0A & (1 << UDRE0)));    // AGUARDA LIBERAR O BUFFER
          UDR0 = dado[i];                   // CARREGA O DADO DO BUFFER
          i++;                              // PASSA PARA O PRÓXIMO DADO
      }
      i = 0;
  }
}
