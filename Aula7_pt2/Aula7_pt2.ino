#define BOTAO1 PB0
#define BOTAO2 PB2
#define BOTAO3 PD4
#define LED1 PD5
#define LED2 PD7
#define LED3 PD6
#define LED4 PB1

ISR (PCINT2_vect) {
  // if(PIND |= (BOTAO))-> SE QUISER USAR UMA VEZ O BOTAO
  PORTD |= (1 << LED3);
  _delay_ms(500);
  PORTD &= ~(1 << LED3);
}

ISR (PCINT0_vect) {
  if(PIND |= (1 << BOTAO1)){
    PORTD |= (1 << LED2);
    _delay_ms(1000);
    PORTD &= ~(1 << LED2);
  }
}

ISR (PCINT20_vect) {
  if(PINB |= (1 << BOTAO3)){
    PORTB |= (1 << LED4);
    _delay_ms(1000);
    PORTB &= ~(1 << LED4);
  }
}

int main () {
  DDRD = (1 << LED1) | (1 << LED2) | (1 << LED3);
  DDRB = (1 << LED4);
  PORTD = (1 << BOTAO3);
  PORTB = (1 << BOTAO1) | (1 << BOTAO2);
  
  PCICR = 0b00000101;
  PCMSK2 = 0b00010000;
  PCMSK0 = 0b10000101;
  sei();
  while (1){

    PORTD |= (1 << LED1);
    _delay_ms(250);
    PORTD &= ~(1 << LED1);
    
  }
}
