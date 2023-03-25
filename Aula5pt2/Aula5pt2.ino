# define LED1 PD5
# define LED2 PD4
# define BOTAO PD2

ISR (INT0_vect) {
  PORTD &= ~(1 << LED1);
  PORTD ^= (1 << LED2);
  _delay_ms(1000);
  PORTD ^= (1 << LED2);
}

int main(){

  DDRD |= (1 << LED1) | (1 << LED2);
  PORTD &= ~(1 << LED2);
  PORTD |= (1 << BOTAO);

  EICRA = 0b00000011;
  EIMSK = 0b00000001;
  sei();

  while(1){
    PORTD |= (1 << LED1);
    _delay_ms(500);
    PORTD &= ~(1 << LED1);
    _delay_ms(500);
  }
  
}
