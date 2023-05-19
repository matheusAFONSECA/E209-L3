void ADC_init(void)
{
  // Configurando Vref para VCC = 5V
  ADMUX = (1 << REFS0);
  /*
    ADC ativado e preescaler de 128
    16MHz / 128 = 125kHz
    ADEN = ADC Enable, ativa o ADC
    ADPSx = ADC Prescaler Select Bits
    1 1 1 = clock / 128
  */
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}
int ADC_read(u8 ch)
{
  char i;
  int ADC_temp = 0; // ADC temporário, para manipular leitura
  int ADC_read = 0; // ADC_read
  ch &= 0x07;
  // Zerar os 3 primeiros bits e manter o resto
  ADMUX = (ADMUX & 0xF8) | ch;
  // ADSC (ADC Start Conversion)
  ADCSRA |= (1 << ADSC); // Faça uma conversão
  // ADIF (ADC Interrupt Flag) é setada quando o ADC pede interrupção
  // e resetada quando o vetor de interrupção
  // é tratado.
  while (!(ADCSRA & (1 << ADIF))); // Aguarde a conversão do sinal
  for (i = 0; i < 8; i++) // Fazendo a conversão 8 vezes para maior precisão
  {
    ADCSRA |= (1 << ADSC); // Faça uma conversão
    while (!(ADCSRA & (1 << ADIF))); // Aguarde a conversão do sinal
    ADC_temp = ADCL; // lê o registro ADCL
    ADC_temp += (ADCH << 8); // lê o registro ADCH
    ADC_read += ADC_temp; // Acumula o resultado (8 amostras) para média
  }
  ADC_read = ADC_read >> 3; // média das 8 amostras ( >> 3 é o mesmo que /8)
  return ADC_read;
}
int main()
{
  u16 adc_result0;
  unsigned long int aux;
  float tensao;
  
  Serial.begin(9600);
  ADC_init(); // Inicializa ADC
  
  while (1)
  {
    adc_result0 = ADC_read(ADC0D); // lê o valor do ADC0 = PC0
    
    // Mostra o valor na serial
    Serial.print("ADC0: ");
    Serial.println(adc_result0);
    _delay_ms(50);
    tensao = (adc_result0 * 5.0) / 1023.0;
    Serial.print("tensao: ");
    Serial.println(tensao);
  }
}
