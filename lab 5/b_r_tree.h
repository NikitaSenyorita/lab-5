#pragma once
#include <iostream>

class b_r_tree;

struct node {
	node* child[2];
	node* parent;
	int key;
	bool red = false;
	node(int key) :key(key) { child[0] = nullptr;  child[1] = nullptr; parent = nullptr;};
	node();
	node* brother();
	void rot_one(b_r_tree& tree, bool dir);
	~node()
	{
		if (child[0])
			delete child[0];
		if (child[1])
			delete child[1];

		child[0] = nullptr;
		child[1] = nullptr;
	};
};


class b_r_tree {	// black-red tree
private:
	node *root;
	int size;
	node ** seq;
	size_t sizeSeq;

public:
	b_r_tree() : root(nullptr), size(0) {}; // check size
	b_r_tree(b_r_tree &&);
	b_r_tree(const b_r_tree &);
	~b_r_tree();

	friend struct node;
	friend std::ostream & operator<<(std::ostream& os, b_r_tree& set);
	// check empty tree /////////
	void add(int key);
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
	void operator=(const b_r_tree&);
	b_r_tree operator=(b_r_tree &);
	b_r_tree operator |(const b_r_tree&)const;
	b_r_tree operator &(const b_r_tree&)const;
	b_r_tree operator ^(const b_r_tree&)const;
	b_r_tree operator /(const b_r_tree&)const;
	void put_all(std::ostream& os, node* temp);
	void putSeq_all();

	void removeSeq(size_t);

	void erase(size_t, size_t);
	void excl(b_r_tree &);
	void mul(size_t);
};