#define LED PD7 
#define BOTAO PD2
ISR (INT0_vect) {
  PORTD ^= (1 << LED);
}
int main () {
  DDRD |= (1 << LED);
  PORTD |= (1 << BOTAO);
  
  EICRA = 0b00000011;
  EIMSK = 0b00000001;
  sei();
  while (1) {
    
  }
}
