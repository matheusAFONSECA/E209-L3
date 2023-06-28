#define pwm_out (1 << PD6)
int brilho = 0;
int main(void)
{
 DDRD |= pwm_out; // configura saída para o PWM
 PORTD &= ~pwm_out; // PWM inicia desligado
 // Configura modo FAST PWM e modo do comparador A
 TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1);
 TCCR0B = 1; // Seleciona opção para frequência
 OCR0A = 0;
  
 for (;;)
 {
 OCR0A = brilho;
 brilho += 10;
 if (brilho > 254)
 brilho = 0;
 _delay_ms(100);
 }
}
