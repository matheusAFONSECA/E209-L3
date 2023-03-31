#define LED1 PD5  // led cheio
#define LED2 PD4  // led vazio
#define LED3 PD6  // led da valvula
#define MAX PD3   // sensor de máximo
#define MIN PD2   // sensor de minimo

int main(){
  DDRD |= (1 << LED1) | (1 << LED2);
  PORTD |= (1 << MAX) | (1 << MIN);
  
  while(1){
    if(!(PIND & (1 << MAX)) && (PIND &(1 << MIN))){
      PORTD |= (1 << LED1);
      PORTD &= ~(1 << LED2);
      PORTD &= ~(1 << LED3);
    }
    if((PIND & (1 << MAX)) && !(PIND &(1 << MIN))){
      PORTD |= (1 << LED2);
      PORTD |= (1 << LED3);
      PORTD &= ~(1 << LED1);
    }
  }
  
}
