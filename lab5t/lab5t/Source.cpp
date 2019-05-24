#include "b_r_tree.h"
#include <time.h>
#include <conio.h>

const size_t N = 6; // Максимальная мощность множеств
const size_t POWER = 100; // Мощность множества размещаемых элементов

b_r_tree* generate();

int main()
{
	srand(time(NULL));

	b_r_tree A, B, C, D, E, temp1, temp2, temp3, result;

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
	cout << "-----------------------------------------------------\n\n";

	// Вычисление цепочки операций
	temp1 = A | B;
	temp2 = C & D;
	temp3 = temp2 / E;
	result = temp1 ^ temp3;

	// Вывод цепочки операций
	cout << "A | B\n" << temp1;
	cout << "\nC & D\n" << temp2;
	cout << "\n(C & D) / E\n" << temp3;
	cout << "\nRESULT = (A | B) ^ ((C & D) / E)\n" << result;

	_getch();

	return 0;
}

b_r_tree* generate()
{
	b_r_tree* result = new b_r_tree();

	int key = 0;
	for (size_t i = 0; i < N; ++i)
		result->add(rand() % POWER);

	return result;
}