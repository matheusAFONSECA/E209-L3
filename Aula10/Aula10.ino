#define LED PD4     // led verde
#define LED1 PD3    // led amarelo
#define LED2 PD2    // led vermelho

int contador = 0;

ISR (TIMER0_OVF_vect) {
  if (contador <= 11723) {  // 12 segundos
    PORTD |= (1 << LED);    // liga o led verde
    PORTD &= ~(1 << LED1);  // desliga o led amarelo
    PORTD &= ~(1 << LED2);  // desliga o led vermelho
  }
  
  if (contador == 11724) {  // 3 segundos
    PORTD &= ~(1 << LED);   // desliga o led verde
    PORTD |= (1 << LED1);   // liga o led amarelo
    PORTD &= ~(1 << LED2);  // desliga o led vermelho
  }

  
  if (contador == 14655) {  // 15 segundos
    PORTD &= ~(1 << LED);   // desliga o led verde
    PORTD &= ~(1 << LED1);  // desliga o led amarelo
    PORTD |= (1 << LED2);   // liga o led vermelho
  }
  
  if(contador == 29310){    // quando terminar o ciclo de contagem do semáforo
    contador = 0;           // zerando p contador
  }
  
  contador ++;    // incremento do contador
}

int main()
{
  DDRD |= (1 << LED);     // definindo o led verde como saida
  DDRD |= (1 << LED1);    // definindo o led amarelo como saida
  DDRD |= (1 << LED2);    // definindo o led vermelho como saida
  
  TCCR0A = 0;                          // modo normal
  TCCR0B = (1 << CS01) | (1 << CS00);  // 64 -> preescaler
  TIMSK0|= (1 << TOIE0);               // habilita interrupção no fim da contagem
  sei();                               // habilita interrupção global 
  
  while (1) {
   
  }
}
