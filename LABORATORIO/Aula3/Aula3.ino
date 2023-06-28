int main(){

  // esses comandos são exucutados somente uma vez
  DDRD = 0b10000000;
  PORTD |= 0b0100000;
  
  while(1){

    if((PIND & (0b0100000) == 0)){
      PORTD |= 0b1000000; // LIGA O LED QUANDO O BOTÃO ESTEJA PRECIONADO
    } else {
      PORTD &= ~(0b10000000); // DELSIGA O LED QUANDO O BOTÃO FOR PRECIONADO
    }
    
  }
  
}
