int main(){
  
 DDRD = DDRD | 0b10100000;      // pino PD7 e PD5 definido como saída
 PORTD = PORTD | 0b01010000;    // Habilitar resistor de PULL-UP no PD6 e PD4
 PORTD = PORTD & ~(0b10000000);  // Desliga a saída PD7

 while(1){
    PORTD = PORTD | 0b10000000;  // 
    PORTD = PORTD & ~ (0b00100000);
    _delay_ms(2000);
    PORTD = PORTD & ~ (0b10000000);
    PORTD = PORTD | 0b00100000;  // 
    _delay_ms(2000);
 }
 
}
