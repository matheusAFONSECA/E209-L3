# define LED1 PD5
# define LED2 PD4
# define BOTAO PD2
# define BOTAO2 PD1

ISR (INT0_vect) {
  PORTD &= ~(1 << LED1);
  PORTD ^= (1 << LED2);
  _delay_ms(1000);
  PORTD ^= (1 << LED2);
}

ISR (INT1_vect) {
  PORTD ^= (1 << BOTAO);
}

int main(){

  DDRD |= (1 << LED1) | (1 << LED2);
  PORTD &= ~(1 << LED2);
  PORTD |= (1 << BOTAO);
  PORTD |= (1 << BOTAO2);

  EICRA = 0b00000011;
  EIMSK = 0b00000011;
  sei();

  while(1){
    PORTD |= (1 << LED1);
    _delay_ms(500);
    PORTD &= ~(1 << LED1);
    _delay_ms(500);
  }
  
}
