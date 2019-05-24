#pragma once
#include <iostream>

using namespace std;

struct node {

	node* child[2];
	node* parent;
	int key;
	bool red = false;

	node(int key) :key(key) { child[0] = nullptr;  child[1] = nullptr; parent = nullptr; };
	~node(){ delete child[0];	delete child[1]; };

	node* brother();

	void rot_one(b_r_tree& tree, bool dir);
};


class b_r_tree {

	node *root;
	int size;

public:

	b_r_tree() : root(nullptr), size(0) {};
	b_r_tree(b_r_tree &&);
	b_r_tree(const b_r_tree &);
	~b_r_tree();

	friend struct node;
	friend ostream & operator<<(ostream& os, b_r_tree& set);

	bool add(int key);

	node* make_node(int data);
	node* search(int key, node* temp = nullptr)const;
	node* next_node(node* victim);

	void rebalance_add(node* inserted);
	void rebalance_delete(node* inserted);
	void rebalance_delete_2(node* inserted);
	void rebalance_delete_3(node* inserted);
	void remove(node* victim);
	void copy_tree(const node* n);
	void difference(const node* n);
	void AND(const node* parent, const b_r_tree& other);
	void put_all(ostream& os, node* temp);

	b_r_tree& operator=(const b_r_tree &);
	b_r_tree operator |(const b_r_tree&)const;
	b_r_tree operator &(const b_r_tree&)const;
	b_r_tree operator ^(const b_r_tree&)const;
	b_r_tree operator /(const b_r_tree&)const;
};