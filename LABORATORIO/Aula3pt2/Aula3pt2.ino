int main(){
  
 DDRD = DDRD | 0b10100000;      // pino PD7 definido como saída
 PORTD = PORTD | 0b01000000;    // Habilitar resistor de PULL-UP no PD6
 PORTD = PORTD & ~(0b10000000);  // Desliga a saída PD7

 for(;;){
  if((PIND & 0b01000000) == 0)   // verifica se o botão foi precionado
  {
    PORTD = PORTD | 0b00100000;  // Aciona a saida
    _delay_ms(5000);
    PORTD = PORTD &~(0b00100000); // Desliga a saída
  }
 }
 
}
