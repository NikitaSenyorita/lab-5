#include "b_r_tree.h"
#include <time.h>
#include <iostream>
#include <conio.h>

b_r_tree* generate();

int main()
{
	srand(time(NULL));


	b_r_tree A, B, C, D, E, Temp1, Temp2, Temp3, result;


	// Генерация множеств
	A = *generate();
	B = *generate();
	C = *generate();
	D = *generate();
	E = *generate();


	// Вывод множеств
	cout << "A:\n" << A << '\n';
	cout << "B:\n" << B << '\n';
	cout << "C:\n" << C << '\n';
	cout << "D:\n" << D << '\n';
	cout << "E:\n" << E << '\n';


	// Цепочка операций
	Temp1 = A | B;
	cout << "A | B:\n" << Temp1 << '\n';

	Temp2 = C & D;
	cout << "C & D:\n" << Temp2 << '\n';

	Temp3 = Temp2 / E;
	cout << "C & D / E:\n" << Temp3 << '\n';

	result = Temp1 ^ Temp3;
	cout << "(A | B) ^ ((C & D) / E):\n" << result << '\n';

	cout << "-----------------------------------------------------\n";


	_getch();

	return 0;
}

b_r_tree* generate()
{
	b_r_tree* result = new b_r_tree();

	for (size_t i = 0; i < N; ++i)
		result->add(rand() % POWER);

	return result;
}