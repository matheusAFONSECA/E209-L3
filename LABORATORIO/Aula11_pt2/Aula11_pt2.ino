#define PINO 2 //PC2
#define Led (1 << PD6)// LED ATIVADO COMO SAIDA
#define AMOSTRAS 50

unsigned int Leitura_AD; //leitura da temperatura
float tensao;

int main(){

  Serial.begin(115200);

  // configurações PWM
  DDRD |= Led; // configura saída para o PWM
  PORTD &= ~Led; // PWM inicia desligado
  // Configura modo FAST PWM e modo do comparador A
  TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1);
  TCCR0B = 1; // Seleciona opção para frequência
  OCR0A = 0;
  
  ADMUX = (0 << REFS1) + (1 << REFS0); //Utiliza 5V como referência (1023)
  
  ADCSRA = (1 << ADEN) + (1 << ADPS2) + (1 << ADPS1) + (1 << ADPS0); //Habilita ADC e PS 128 (10 bits)
  
  ADCSRB = 0; //Conversão Única
  
  DIDR0 = (1 << PINO); //Desabilita o PC2 como pino digital - Não obrigatório
  
  for(;;){
  
    //Determinar o pino de leitura
    ADMUX = (ADMUX & 0xF8) | PINO;
  
    //Leitura do ADC (Sem média)
    ADCSRA |= (1 << ADSC); //Inicia a conversão
    
    while((ADCSRA & (1<<ADSC)) == (1<<ADSC)); //Esperar a conversão
    
    Leitura_AD = ADC; //Armazenamento da leitura
    
    tensao = (Leitura_AD * 5) / 1023.0; //Cálculo da Tensão
    
    //Serial.println(tensao);
   
    //Leitura do ADC (Com média)
    unsigned int SomaLeitura = 0, MediaLeitura;
    for(int i=0; i < AMOSTRAS; i++){
    
      ADCSRA |= (1 << ADSC); //Inicia a conversão
    
      while((ADCSRA & (1<<ADSC)) == (1<<ADSC)); //Esperar a conversão
    
      Leitura_AD = ADC;
      
      SomaLeitura += Leitura_AD;
    }
    
    MediaLeitura = SomaLeitura / AMOSTRAS;
    
    tensao = ((MediaLeitura * 255.0) / (1023.0));

    OCR0A = tensao;

    Serial.print(tensao);
    
  }
  
}
