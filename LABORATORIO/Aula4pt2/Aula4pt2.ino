# define SENSOR PD0
# define MOTOR PD1
# define ALARME PD2
# define BLIGA PD3
# define BDES PD4

int main() {

  DDRD = 0b00000110;
  PORTD |= (1 << BLIGA);
  PORTD |= (1 << BDES);
  PORTD |= (1 << SENSOR);

  while (1) {

    if (!(PIND & (1 << SENSOR))) {  // sensor ligado -> motor desliga -> alarme liga
      PORTD &= ~(1 << MOTOR);
      PORTD |= (1 << ALARME);
    }

    else {
      if (!(PIND & (1 << BLIGA)) && (PIND & (1 << SENSOR))) { // ao apertar o botão liga -> motor liga sensor desligado
        PORTD |= (1 << MOTOR);
      }
      if (!(PIND & (1 << BDES))) {  // ao apertar o botão desliga -> motor desliga
        PORTD &= ~(1 << MOTOR);
      }

      if ((PIND & (1 << SENSOR)) && !(PIND & (1 << BLIGA))) {  // sensor desligado -> botão liga e alarme desliga
        PORTD &= ~(1 << ALARME);
      }
    }
  }
}
