#include "b_r_tree.h"
#include <time.h>
#include <iostream>
#include<conio.h>

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


	//............................................................ÄÅÌÎÍÑÒĞÀÖÈß ĞÀÁÎÒÛ Ñ ÏÎÑËÅÄÎÂÀÒÅËÜÍÎÑÒßÌÈ
	b_r_tree S1, S2;
	size_t left, right; // Ãğàíèöû äëÿ îïåğàöèè erase
	size_t count;


	S1 = *generate();
	S2 = *generate();
	left = 0;
	right = 3;
	count = 1;


	// Âûâîä ïîñëåäîâàòåëüíîñòåé
	cout << endl << endl << endl << "S1: ";
	S1.putSeq_all();
	cout << "S2: ";
	S2.putSeq_all();


	// Îïåğàöèè íàä ïîñëåäîâàòåëüíîñòÿìè
	// ERASE
	cout << endl << endl << "erase S2 from " << left << " to " << right << ": " << endl;
	cout << "S2: ";
	S2.putSeq_all();
	S2.erase(left, right);
	cout << "new S2: ";
	S2.putSeq_all();


	left = 2;
	right = 3;
	// Îïåğàöèè íàä ïîñëåäîâàòåëüíîñòÿìè
	// ERASE
	cout << endl << endl << "erase S2 from " << left << " to " << right << ": " << endl;
	cout << "S2: ";
	S2.putSeq_all();
	S2.erase(left, right);
	cout << "new S2: ";
	S2.putSeq_all();


	// MUL
	cout << endl << endl << "mul x" << count << " S1: " << endl;
	cout << "S1: ";
	S1.putSeq_all();
	S1.mul(count);
	cout << "new S1: ";
	S1.putSeq_all();


	// EXCL
	cout << endl << endl << "excl S2 from S1: " << endl;
	cout << "S1: ";
	S1.putSeq_all();
	cout << "S2: ";
	S2.putSeq_all();
	S1.excl(S2);
	cout << "new S1: ";
	S1.putSeq_all();

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