# define LED PD7
# define BOTAO PD6

int main(){

  DDRD = 0b1000000;
  PORTD |= (1 << BOTAO);

  while(1){
    if(!(PIND & (1 << BOTAO))) {
      PORTD |= (1 << LED);
    } else {
      PORTD &= ~(1 << LED);
    }

  }
  
}
