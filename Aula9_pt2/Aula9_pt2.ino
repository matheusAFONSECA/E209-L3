#define pwm_out (1 << PD6)
#define BOTAO PD5

int brilho = 0;

int main(void)
{
  DDRD |= pwm_out; // configura saída para o PWM
  PORTD |= (1 << BOTAO); // CONFIGURA O BOTAO
  PORTD &= ~pwm_out; // PWM inicia desligado
  // Configura modo FAST PWM e modo do comparador A
  TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1);
  TCCR0B = 1; // Seleciona opção para frequência
  OCR0A = 0;

  for (;;)
  {
    OCR0A = brilho;
    if(!(PIND & (1 << BOTAO))){
      brilho += 38;
    }
    if (brilho > 254){
      brilho = 0;
    }
  }
}
