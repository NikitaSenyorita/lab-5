#include "b_r_tree.h"
#include <time.h>
#include <iostream>
#include<conio.h>

const int N = 6; // Мощность множества
const size_t K = 12; // Мощность множества размещаемых элементов

b_r_tree* generate();

using namespace std;

int main()
{

	b_r_tree a, b, c, d, e, result;
	a = *generate();
	b = *generate();
	c = *generate();
	d = *generate();
	e = *generate();


	std::cout << "A:\n" << a << '\n' << "Seq: ";
	a.putSeq_all();
	std::cout << "B:\n" << b << '\n' << "Seq: ";
	b.putSeq_all();
	std::cout << "C:\n" << c << '\n' << "Seq: ";
	c.putSeq_all();
	std::cout << "D:\n" << d << '\n' << "Seq: ";
	d.putSeq_all();
	std::cout << "E:\n" << e << '\n' << "Seq: ";
	e.putSeq_all();

	std::cout << "-----------------------------------------------------\n";

	std::cout << "RESULT = (A | B) ^ (C & D) / E\n\n";
	result = ((a | b) ^ ((c&d) / e));

	std::cout << "RESULT:\n" << result;
	std::cout << std::endl << "Seq: ";
	result.putSeq_all();

	size_t left, right;
	left = 2;
	right = 5;

	cout << endl << endl;
	cout << "S1: ";
	a.putSeq_all();
	cout << "S2: ";
	b.putSeq_all();
	
	cout << endl << endl << "erase S2 from " << left << " to " << right << ": " << endl;
	b.putSeq_all();
	b.erase(left, right);
	b.putSeq_all();

	cout << endl << endl << "mul x2 S1: " << endl;
	a.putSeq_all();
	a.mul(2);
	a.putSeq_all();

	cout << "excl S2 from S1: " << endl;
	a.putSeq_all();
	a.excl(b);
	a.putSeq_all();

	_getch();

	return 0;
}

b_r_tree* generate()
{
	b_r_tree* result = new b_r_tree();

	size_t size = N + (rand() % 6) - 3;
	int key = 0;
	for (size_t count = 0; count < size;) {
		key = (rand() % K) + 1;
		result->add(key);
		count++;
	}

	return result;
}