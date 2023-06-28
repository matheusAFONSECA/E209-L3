int main(void)
{
  /* Configurando os pinos PD7 e PDS como saída (sem mascaramento).
    Todos os bits, representantes dos pinos, que tiverem nível lógico:
    1 (HIGH) - serão saída e O (LOW) - serão entrada
    Exemplo: PINOS Px. =7 6 543210
    BINARIO = 10100000
    HEX = Ox À | 0 => OXAO
  */
  DDRD = 0b11000000;  // PD7 e PD6

  while (1) // loop infinito
  {
    /*
      No exemplo abaixo ficará piscando os leds VERDE e VERMELHO, de acordo
      com a montagem.
    */
    PORTD = 0b00000000;
    _delay_ms(500); // delay ms contabiliza em ms
    PORTD = 0b01000000;
    _delay_ms(500); // delay de 0,5s.
    PORTD = 0b10000000;
    _delay_ms(500); // delay de 0,5s.
    PORTD = 0b11000000;
    _delay_ms(500); // delay de 0,5s.

  }
}
