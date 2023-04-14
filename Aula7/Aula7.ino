#define BOTAO PD7
#define LED PD6

ISR (PCINT2_vect) {
  // if(PIND |= (BOTAO))-> SE QUISER USAR UMA VEZ O BOTAO
  PORTD |= (1 << LED);
  _delay_ms(500);
  PORTD &= ~(1 << LED);
}

int main () {
  DDRD = (1 << LED);
  PORTD = (1 << BOTAO);
  PCICR = 0b00000100;
  PCMSK2 = 0b10000000;
  sei();
  while (1){
    
  }
}
