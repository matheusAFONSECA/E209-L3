int main(void) {
  DDRD |= (1 << PD5); // Define o pino 5 do PORTD como saída
  // Configuração do modo PWM rápido do timer 0
  TCCR0A = (1 << WGM01) | (1 << WGM00); // Modo PWM rápido, TOP = 0xFF
  TCCR0B = (1 << CS01); // Prescaler 8
  OCR0A = 0x00; // Duty cycle inicial de 0%
  
  while (1) {
    for (int duty_cycle = 0; duty_cycle <= 250; duty_cycle=duty_cycle+50) {
      OCR0A = duty_cycle;
      _delay_ms(400); // Intervalo de 1 segundo / 256
    }
    
  }
}
