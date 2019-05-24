#pragma once
#include <iostream>

using namespace std;

class b_r_tree;

struct node {
	node* child[2];
	node* parent;
	int key;
	bool red = false;
	node(int key) :key(key) { child[0] = nullptr;  child[1] = nullptr; parent = nullptr; };
	node* brother();
	void rot_one(b_r_tree& tree, bool dir);
	~node()
	{
		delete child[0];	delete child[1];
	};
};


class b_r_tree {	// black-red tree
private:
	node *root;
	int size;
public:
	b_r_tree() : root(nullptr), size(0) {}; // check size
	b_r_tree(b_r_tree &&);
	b_r_tree(const b_r_tree &);
	~b_r_tree();

	friend struct node;
	friend ostream & operator<<(ostream& os, b_r_tree& set);
	// check empty tree /////////
	bool add(int key);
	node* make_node(int data);
	node* search(int key, node* temp = nullptr)const;
	void rebalance_add(node* inserted);
	void rebalance_delete(node* inserted);
	void rebalance_delete_2(node* inserted);
	void rebalance_delete_3(node* inserted);
	void remove(node* victim);
	node* next_node(node* victim);
	void copy_tree(const node* n);
	void difference(const node* n);
	void AND(const node* parent, const b_r_tree& other);
	b_r_tree& operator=(const b_r_tree &);
	b_r_tree operator |(const b_r_tree&)const;
	b_r_tree operator &(const b_r_tree&)const;
	b_r_tree operator ^(const b_r_tree&)const;
	b_r_tree operator /(const b_r_tree&)const;
	void put_all(ostream& os, node* temp);

};