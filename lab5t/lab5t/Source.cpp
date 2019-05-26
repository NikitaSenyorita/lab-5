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


	//............................................................ДЕМОНСТРАЦИЯ РАБОТЫ С ПОСЛЕДОВАТЕЛЬНОСТЯМИ
	b_r_tree S1, S2;
	size_t left, right; // Границы для операции erase
	size_t count; // Количество вставок mul


	left = 0;
	right = 3;
	count = 1;
	S1 = *generate();
	S2 = *generate();


	// Вывод последовательностей
	cout << "\n---------------------------------------------------------\n\n\n";
	cout << "S1: \n" << S1;
	cout << "\nS2: \n" << S2;


	// Операции над последовательностями
	// ERASE
	cout << "\n\nErase S2 [" << left << "; " << right << "] : \n";
	cout << "Current S2: \n" << S2;
	S2.erase(left, right);
	cout << "New S2: \n" << S2;


	// MUL
	cout << "\n\nMul(" << count << ") S1: \n";
	cout << "Current S1: \n" << S1;
	S1.mul(count);
	cout << "New S1: \n" << S1;


	// EXCL
	cout << "\n\nExcl S2 from S1: \n";
	cout << "Current S1: \n" << S1;
	cout << "Current S2: \n" << S2;
	S1.excl(S2);
	cout << "New S1: \n" << S1;

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