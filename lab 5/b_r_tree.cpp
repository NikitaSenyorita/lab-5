#include "b_r_tree.h"

node::node()
{
}

b_r_tree::b_r_tree(const b_r_tree & set)// copy construltor 
{
	root = nullptr;
	seq = nullptr;
	size = 0;
	sizeSeq = 0;
	copy_tree(set.root);
}

b_r_tree::b_r_tree(b_r_tree && set) // bit-for-bit construltor 
{
	root = set.root;
	size = set.size;
	sizeSeq = set.sizeSeq;
	seq = set.seq;
}

b_r_tree::~b_r_tree()
{
	if (root)
		delete root;
	if (seq)
		delete seq;
}

void b_r_tree::copy_tree(const node* n) { // O(n log n)
	if (n) {
		add(n->key);
		copy_tree(n->child[0]);
		copy_tree(n->child[1]);
	}
}

void b_r_tree::add(int key) // O(log n)
{
	node* inserted_node = nullptr;

	++sizeSeq;
	node * newNode = make_node(key);
	node ** tempSeq = new node * [sizeSeq];
	for (size_t i = 0; i < sizeSeq - 1; ++i)
		tempSeq[i] = seq[i];
	tempSeq[sizeSeq - 1] = newNode;

	seq = tempSeq;
	tempSeq = nullptr;

	if (!root) {
		root = newNode;
		size++;
		inserted_node = root;
		root->parent = nullptr;
	}
	else {
		for (node* temp = root; temp;) {
			if (temp->key == key) {
				temp = nullptr;
				return;
			}
			else if (temp->key < key) {
				if (!temp->child[1]) {
					temp->child[1] = newNode;
					size++;
					temp->child[1]->parent = temp;
					inserted_node = temp->child[1];
					temp = nullptr;
				}
				else
					temp = temp->child[1];
			}
			else {
				if (!temp->child[0]) {
					temp->child[0] = newNode;
					size++;
					temp->child[0]->parent = temp;
					inserted_node = temp->child[0];
					temp = nullptr;
				}
				else
					temp = temp->child[0];
			}
		}
	}

	rebalance_add(inserted_node);

}

node* b_r_tree::search(int key, node* temp)const
{
	node* n = temp;
	if (!n)	n = root;
	if (n) {
		if (n->key == key)	 return n;
		else if (!(n->key < key ? n->child[1] : n->child[0]))	return nullptr;
		else search(key, n->key < key ? n->child[1] : n->child[0]);
	}
	else return nullptr;
}

/* ������� ��� ������������ �������� ���� */
void node::rot_one(b_r_tree& tree, bool dir) {
	node* pivot = child[!dir];

	pivot->parent = parent; /* ��� ����, ��������, pivot ���������� ������ ������ */
	if (parent != nullptr) {
		if (parent->child[dir] == this)
			parent->child[dir] = pivot;
		else
			parent->child[!dir] = pivot;
	}

	child[!dir] = pivot->child[dir];
	if (pivot->child[dir] != nullptr)
		pivot->child[dir]->parent = this;

	parent = pivot;
	pivot->child[dir] = this;
	if (this == tree.root)
		tree.root = pivot;
}

node* b_r_tree::make_node(int data)
{
	node *red_node = new node(data);

	if (red_node != nullptr) {
		red_node->red = true; /* �������������� ������� ������ */
	}
	return red_node;
}

void b_r_tree::rebalance_add(node* inserted) {
	if (root == inserted)
		inserted->red = false;
	else if (inserted->parent->red) {
		if (inserted->parent->brother()) {
			if (inserted->parent->brother()->red) {
				inserted->parent->red = false;	inserted->parent->brother()->red = false;
				inserted->parent->parent->red = true;
				rebalance_add(inserted->parent->parent);
			}
			else {
				node* granda = inserted->parent->parent;
				if ((inserted == inserted->parent->child[1]) && (inserted->parent == granda->child[0])) {
					inserted->parent->rot_one(*this, 0);
					rebalance_add(inserted->child[0]);
				}
				else if ((inserted == inserted->parent->child[0]) && (inserted->parent == granda->child[1])) {
					inserted->parent->rot_one(*this, 1);
					rebalance_add(inserted->child[1]);
				}
				else {
					inserted->parent->red = false;
					granda->red = true;
					if ((inserted == inserted->parent->child[0]) && (inserted->parent == granda->child[0])) {
						granda->rot_one(*this, 1);
					}
					else { /* (n == n->parent->right) && (n->parent == g->right) */
						granda->rot_one(*this, 0);
					}
				}
			}
		}
		else {
			inserted->parent->red = false;
			inserted->parent->parent->red = true;
			rebalance_add(inserted->parent->parent);
		}
	}
	root->red = false;/////
}

void b_r_tree::rebalance_delete_3(node* son) {
	node* bro = son->brother();
	if (bro->red == false) {
		if ((son == son->parent->child[0]) &&
			(bro->child[1]->red == false) &&
			(bro->child[0]->red == true)) {
			bro->red = true;
			bro->child[0]->red = false;
			bro->rot_one(*this, 1);
		}
		else if ((son == son->parent->child[1]) &&
			(bro->child[0]->red == false) &&
			(bro->child[1]->red == true)) {
			bro->red = true;
			bro->child[1]->red = false;
			bro->rot_one(*this, 0);
		}
	}
	bro->red = son->parent->red;
	son->parent->red = false;

	if (son == son->parent->child[0]) {
		bro->child[1]->red = false;
		son->parent->rot_one(*this, 0);
	}
	else {
		bro->child[0]->red = false;
		son->parent->rot_one(*this, 1);
	}
}

void b_r_tree::rebalance_delete_2(node* son) {
	node* bro = son->brother();
	if ((son->parent->red == true) &&
		(bro->red == false) &&
		(bro->child[0]->red == false) &&
		(bro->child[1]->red == false)) {
		bro->red = true;
		son->parent->red = false;
	}
	else
		rebalance_delete_3(son);
}

void b_r_tree::rebalance_delete(node* son) {
	if (son) {
		node* bro = son->brother();
		if (bro->red) {
			son->parent->red = true;
			bro->red = false;
			if (son == son->parent->child[0])
				son->parent->rot_one(*this, 0);
			else
				son->parent->rot_one(*this, 1);
		}
		if (!(son->parent->red || bro->red || bro->child[0]->red || bro->child[1]->red)) {
			bro->red = true;
			rebalance_delete(son->parent);
		}
		else {
			rebalance_delete_2(son);
		}
	}
}

void b_r_tree::remove(node* victim) {

	if (victim->child[0] != nullptr && victim->child[1] != nullptr) {
		node* next = next_node(victim);
		victim->key = next->key;
		victim = next;
	}
	node* son = victim->child[0] != nullptr ? victim->child[0] : victim->child[1];
	bool black = !victim->red;

	if (!(victim->child[0] || victim->child[1])) {
		victim == victim->parent->child[0] ? victim->parent->child[0] : victim->parent->child[1] = nullptr;
		if (victim == root)
			root = nullptr;
		delete victim;	size--;
	}
	else if (victim->child[0] != nullptr ^ victim->child[1] != nullptr) {
		if (!victim->red)
			victim->child[0] != nullptr ? victim->child[0]->red : victim->child[1]->red = false;
		victim->child[0] != nullptr ? victim->child[0]->parent : victim->child[1]->parent = victim->parent;
		if (victim->parent)
			victim->parent->child[0] == victim ? victim->parent->child[0] : victim->parent->child[1] = victim->child[0] != nullptr ? victim->child[0] : victim->child[1];
		if (victim == root)
			root = victim->child[0] != nullptr ? victim->child[0] : victim->child[1];
		delete victim;	size--;
	}
	if (black)
		rebalance_delete(son);
}

node* node::brother()
{
	if (this == parent->child[0])
		return parent->child[1];
	else
		return parent->child[0];
}

node* b_r_tree::next_node(node* victim) {
	node* iterator = victim->child[1];
	while (iterator->child[0])	iterator = iterator->child[0];
	return iterator;
}

void b_r_tree::operator=(const b_r_tree& other)
{
	if (&other != this) {
		delete root;
		root = nullptr;
		delete seq;
		seq = nullptr;
		size = 0;
		sizeSeq = 0;
		copy_tree(other.root);
	}
}

b_r_tree b_r_tree::operator=(b_r_tree& other)
{
	if (&other != this) {
		delete root;
		root = other.root;
		delete seq;
		seq = other.seq;
		size = other.size;
		sizeSeq = other.sizeSeq;
	}
	return *this;
}

b_r_tree b_r_tree::operator |(const b_r_tree& other)const
{
	b_r_tree result = b_r_tree(*this);

	result.copy_tree(other.root);

	return b_r_tree(result);
}

b_r_tree b_r_tree::operator /(const b_r_tree& other)const
{
	b_r_tree result = b_r_tree(*this);

	result.difference(other.root);

	return b_r_tree(result);
}

void b_r_tree::difference(const node* n) {
	if (n) {
		node* victim = search(n->key);
		if (victim)	remove(victim);
		difference(n->child[0]);
		difference(n->child[1]);
	}
}

b_r_tree b_r_tree::operator &(const b_r_tree& other)const
{
	b_r_tree result = b_r_tree();
	result.AND(root, other);
	return b_r_tree(result);
}

void b_r_tree::AND(const node* parent, const b_r_tree& other) {
	if (parent) {
		node* victim = other.search(parent->key);
		if (victim)	add(victim->key);
		AND(parent->child[0], other);
		AND(parent->child[1], other);
	}
}

b_r_tree b_r_tree::operator ^(const b_r_tree& other)const // Other way of solving - this/other | other/this
{
	b_r_tree result = b_r_tree(*this);
	result = (result | other) / (result & other);
	return b_r_tree(result);
}

std::ostream& operator<<(std::ostream& os, b_r_tree& tree)
{
	os << "[" << tree.size << "] : \n\t[ ";

	tree.put_all(os, tree.root);

	os << " ]\n";

	return os;
}

void b_r_tree::put_all(std::ostream& os, node* n) {
	if (n) {
		if (n->child[0])
			put_all(os, n->child[0]);
		os << n->key;
		if (n->red)
			os << '*';
		os << " \t ";
		if (n->child[1])
			put_all(os, n->child[1]);
	}
}

void b_r_tree::putSeq_all() {

	for (size_t i = 0; i < sizeSeq; ++i) {

		if (seq[i])
			std::cout << seq[i]->key << " ";
	}

	std::cout << std::endl;
}

void b_r_tree::erase(size_t left, size_t right)
{
	node ** tempSeq = new node *[sizeSeq - right + left - 1];
	size_t j = 0;

	for (size_t i = 0; i < sizeSeq; ++i) {

		if (!(i >= left && i <= right)) {

			tempSeq[j] = seq[i];
			++j;
		}
	}

	sizeSeq -= right - left + 1;
	seq = tempSeq;
	tempSeq = nullptr;
}

void b_r_tree::removeSeq(size_t j) {

	node ** tempSeq = new node *[sizeSeq - 1];
	size_t k = 0;

	for (size_t i = 0; i < sizeSeq; ++i) {

		if (i != j) {
			tempSeq[k] = seq[i];
			++k;
		}
	}

	delete[] seq;
	--sizeSeq;
	seq = tempSeq;
	tempSeq = nullptr;
}

void b_r_tree::excl(b_r_tree & exclSeq) {

	if (sizeSeq >= exclSeq.sizeSeq) {

		int j = 0;

		for (int i = 0; i < sizeSeq; ++i) {

			if (seq[i]->key == exclSeq.seq[j]->key) {
				++j;
			}
			else {
				i -= j;
				j = 0;
			}

			if (j == exclSeq.sizeSeq) {

				for (size_t g = i; g > i - j; --g)
					removeSeq(g);

				i -= j - 1;
				j = 0;

				if (sizeSeq != 0 && i < sizeSeq)
					if (seq[i]->key == exclSeq.seq[j]->key) {
						++j;
					}
					else {
						i -= j;
						j = 0;
					}
			}
		}
	}

	// treeRestart();
}


void b_r_tree::mul(size_t n)
{
	node ** tempSeq = new node *[sizeSeq * (n + 1)];
	size_t k = 0;

	for (size_t i = 0; i < n + 1; ++i) {

		for (size_t j = 0; j < sizeSeq; ++j) {

			tempSeq[k] = seq[j];
			++k;
		}
	}

	sizeSeq *= n + 1;
	seq = tempSeq;
	tempSeq = nullptr;
}