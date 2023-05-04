#define LED PD4
#define LED1 PD3
#define LED2 PD2

int contador = 0;
int contador1 = 0;
int contador2 = 0;

ISR (TIMER0_OVF_vect) {
  if (contador == 977) {    // 1 segundo 
    PORTD ^= (1 << LED);  // inverte o estado do LED
    contador = 0;
  }
  contador ++;
  
  if (contador1 == 1466) {    // 1.5 segundo 
    PORTD ^= (1 << LED1);  // inverte o estado do LED1
    contador1 = 0;
  }
  contador1 ++;
  
  if (contador2 == 2443) {    // 2.5 segundo 
    PORTD ^= (1 << LED2);  // inverte o estado do LED2
    contador2 = 0;
  }
  contador2++;
}

int main()
{
  DDRD |= (1 << LED);
  DDRD |= (1 << LED1);
  DDRD |= (1 << LED2);
  
  TCCR0A = 0;                          // modo normal
  TCCR0B = (1 << CS01) | (1 << CS00);  // 64 -> preescaler
  TIMSK0|= (1 << TOIE0);               // habilita interrupção no fim da contagem
  sei();                               // habilita interrupção global 
  
  while (1) {
   
  }
}
