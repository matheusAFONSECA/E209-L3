#define motor PD6


int main(){
  DDRD |= (1 << motor);     // definindo o motor como saida
  // conversor AD:
  ADMUX |= (1 << REFS0);
  ADCSRA |= (1 << ADEN);    // liga o ADC
  ADCSRA |= (1 << ADSC);    // inicializo a conversão
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);   // prescaler = 128
  
  //PWM
  TCCR0A |= (1 << WGM00) |(1 << WGM01);     // MODO PWM RÁPIDO
  TCCR0A |= (1 << COM0A1) |(1 << COM0A0);   // LIGA O PINA PWM NA COMPARAÇÃO DO CONTADOR COM OCR
  TCCR0B |= (1 << CS01);      // PRESCALER = 8
  OCR0A = 200;   // INICIALIZA O VALOR DE OCR
  int conv;
  
  Serial.begin(9600);   // inicia a comunicação serial

  while(1){
    while(ADCSRA & (1 << ADSC));   // aguarda o fim da conversão
    Serial.println(ADC);
    conv = map(ADC, 0, 1023, 0, 225);   // coloca a leitura do POT entre 0 e 255
    OCR0A = 200;

    ADCSRA |= (1 << ADSC);      // inicia uma nova conversão
  }
  
}
