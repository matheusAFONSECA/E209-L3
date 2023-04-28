#define LED PD7
int contador = 0;
ISR (TIMER0_OVF_vect) {
  if (contador > 9) {    // 1 segundo 
    PORTD ^= (1 << LED);  // inverte o estado do LED
    contador = 0;
  }
  contador ++;
}
int main()
{
  DDRD |= (1 << LED);
  
  TCCR0A = 0;                          // modo normal
  TCCR0B = (1 << CS02) | (1 << CS00);  // 1024
  TIMSK0|= (1 << TOIE0);               // habilita interrupção no fim da contagem
  sei();                               // habilita interrupção global 
  
  while (1) {
   
  }
}
