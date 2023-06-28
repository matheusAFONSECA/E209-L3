#include <iostream>
#include <iomanip>  // biblioteca para casa decimais

using namespace std;

int main()
{
	// declaracao de var
	float lar;  	// quantidade de laranjas
	float preco;    // preco das laranjas
	
	// entra com os dados
	cin >> lar;
	
	// saida de dados
	if(lar < 12){
		preco = lar * 0.8;
		cout << "Preco da unidade: R$ 0.80" << endl;
		cout << fixed << setprecision(2);
		cout << "Preco total: R$ " << preco << endl;
	}
	else{
		preco = lar * 0.65;
		cout << "Preco da unidade: R$ 0.65" << endl;
		cout << fixed << setprecision(2);
		cout << "Preco total: R$ " << preco << endl;
	}
	
	return 0;
}