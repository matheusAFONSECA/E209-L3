#include <iostream>

using namespace std;

int main()
{
	// declaracao de var
	int num;	// numero entre 0-50
	int i;		//contador
	
	// entrada do divisor
	cin >> num;
	
	//saida de dados
	for(i = num - 1; i > 0; i--){
		
		cout << "Resto da divisao de " << num << " por " << i << ": " << num % i << endl; 
		
	}
	
	return 0;
}