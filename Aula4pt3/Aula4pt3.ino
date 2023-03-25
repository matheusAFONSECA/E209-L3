# define MOTOR1 PD0
# define MOTOR2 PD1
# define MOTOR3 PD2
# define B1 PD3
# define B2 PD4
# define B3 PD5

int main(){
  int var = 0;    // var de controle dos motores

  DDRD = 0b00000111;
  PORTD |= (1 << B1);
  PORTD |= (1 << B2);
  PORTD |= (1 << B3);
  
  while(1){

     if(!(PIND & (1 << B3)) && !(PIND & (1 << B2)) && !(PIND & (1 << B1))) {
      PORTD &= ~(1 << MOTOR1);
      PORTD &= ~(1 << MOTOR2);
      PORTD |= (1 << MOTOR3);
    }
    
    else if(!(PIND & (1 << B1))){
      PORTD |= (1 << MOTOR1);
      PORTD &= ~(1 << MOTOR3);
      PORTD &= ~(1 << MOTOR2);
    }
    else if(!(PIND & (1 << B2))){
      PORTD |= (1 << MOTOR2);
      PORTD &= ~(1 << MOTOR3);
      PORTD &= ~(1 << MOTOR1);
    }
    else if(!(PIND & (1 << B3))){
      PORTD |= (1 << MOTOR3);
      PORTD &= ~(1 << MOTOR1);
      PORTD &= ~(1 << MOTOR2);
    }
   
  }
  
}
