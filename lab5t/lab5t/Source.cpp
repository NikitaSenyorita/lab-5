#include "b_r_tree.h"
#include <time.h>
#include <iostream>

const int N = 5; // Мощность множества

b_r_tree* generate();

int main()
{
	srand(time(NULL));
	b_r_tree a, b, c, d, e, result;
	a = *generate();
	b = *generate();
	c = *generate();
	d = *generate();
	e = *generate();


	std::cout << "A:\n" << a << '\n';
	std::cout << "B:\n" << b << '\n';
	std::cout << "C:\n" << c << '\n';
	std::cout << "D:\n" << d << '\n';
	std::cout << "E:\n" << e << '\n';

	std::cout << "-----------------------------------------------------\n";


	std::cout << "RESULT = (A | B) ^ (C & D) / E\n\n";
	result = ((a | b) ^ ((c&d) / e));

	std::cout << "RESULT:\n" << result;

	std::cin.get();

	return 0;
}

b_r_tree* generate()
{
	b_r_tree* result = new b_r_tree();

	size_t size = N + (rand() % 6) - 3;
	int key = 0;
	for (size_t count = 0; count < size;) {
		key = (rand() % 900) + 100;
		if (result->add(key))
			count++;
	}

	return result;
}