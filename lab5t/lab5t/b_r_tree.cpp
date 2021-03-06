#include "b_r_tree.h"

b_r_tree::b_r_tree(const b_r_tree & set)
{
	root = nullptr;
	size = 0;
	copy_tree(set.root);
}

b_r_tree::b_r_tree(b_r_tree && set)
{
	root = set.root;
	size = set.size;
}

b_r_tree::~b_r_tree()
{
	delete root;
}

void b_r_tree::copy_tree(const node* n) { // O(n log n)
	if (n) {
		add(n->key);
		copy_tree(n->child[0]);
		copy_tree(n->child[1]);
	}
}

bool b_r_tree::add(int key) // O(log n)
{
	node* inserted_node = nullptr;

	if (!root) {
		root = make_node(key);
		size++;
		inserted_node = root;
		root->parent = nullptr;
	}
	else {
		for (node* temp = root; temp;) {
			if (temp->key == key) {
				seq.push_back(temp);
				temp = nullptr;
				return false;
			}
			else if (temp->key < key) {
				if (!temp->child[1]) {
					temp->child[1] = make_node(key);
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
					temp->child[0] = make_node(key);
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
	seq.push_back(inserted_node);
	rebalance_add(inserted_node);
	return true;
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

// ������� ��� ������������ �������� ����
void node::rot_one(b_r_tree& tree, bool dir) {
	node* pivot = child[!dir];

	pivot->parent = parent;
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
		red_node->red = true;
	}
	return red_node;
}

void b_r_tree::rebalance_add(node* inserted) {
	if (root == inserted)
		inserted->red = false;
	else {
		if (inserted->parent->red) {
			if (inserted->parent->brother() && inserted->parent->brother()->red) {
				inserted->parent->red = false;	inserted->parent->brother()->red = false;
				inserted->parent->parent->red = true;
				rebalance_add(inserted->parent->parent);
			}
			else {
				node* granda = inserted->parent->parent;
				if ((inserted == inserted->parent->child[1]) && (inserted->parent == granda->child[0])) {
					inserted->parent->rot_one(*this, 0);
					inserted = inserted->child[0];
				}
				else if ((inserted == inserted->parent->child[0]) && (inserted->parent == granda->child[1])) {
					inserted->parent->rot_one(*this, 1);
					inserted = inserted->child[1];
				}
				inserted->parent->red = false;
				granda->red = true;
				if ((inserted == inserted->parent->child[0]) && (inserted->parent == granda->child[0])) {
					granda->rot_one(*this, 1);
				}
				else {
					granda->rot_one(*this, 0);
				}
			}
		}
	}
	root->red = false;
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
		else {
			if ((son == son->parent->child[1]) &&
				(bro->child[0]->red == false) &&
				(bro->child[1]->red == true)) {
				bro->red = true;
				bro->child[1]->red = false;
				bro->rot_one(*this, 0);
			}
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
	bool bro_left_child_black, bro_rigth_child_black;
													 
	if (bro->child[0]) bro_left_child_black = bro->child[0]->red == false;
	else bro_left_child_black = true;

	if (bro->child[1]) bro_rigth_child_black = bro->child[1]->red == false;
	else bro_rigth_child_black = true;

	if ((son->parent->red == true) &&
		(bro->red == false) &&
		(bro_left_child_black) &&
		(bro_rigth_child_black)) {
		bro->red = true;
		son->parent->red = false;
	}
	else
		rebalance_delete_3(son);
}

void b_r_tree::rebalance_delete(node* son) {
	if (son && son != root) {
		node* bro = son->brother();
		if (!bro)	son->red = true;
		else {
			if (bro->red) {
				son->parent->red = true;
				bro->red = false;
				if (son == son->parent->child[0])
					son->parent->rot_one(*this, 0);
				else
					son->parent->rot_one(*this, 1);
			}
			bool bro_left_child_black, bro_rigth_child_black;

			if (bro->child[0]) bro_left_child_black = bro->child[0]->red == false;
			else bro_left_child_black = true;

			if (bro->child[1]) bro_rigth_child_black = bro->child[1]->red == false;
			else bro_rigth_child_black = true;

			if (!son->parent->red && !bro->red && bro_left_child_black && bro_rigth_child_black) {
				bro->red = true;
				rebalance_delete(son->parent);
			}
			else {
				rebalance_delete_2(son);
			}
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
		if (victim == root)
			root = nullptr;
		else{
			if (victim == (victim->parent->child[0]))
				victim->parent->child[0] = nullptr;
			else
				victim->parent->child[1] = nullptr;
			}
		delete victim;	size--;
	}
	else if (victim->child[0] != nullptr ^ victim->child[1] != nullptr) {
		if (!victim->red) {
			if (victim->child[0] != nullptr)
				victim->child[0]->red = false;
			else
				victim->child[1]->red = false;
		}
		if (victim->child[0] != nullptr)
			victim->child[0]->parent = victim->parent;
		else
			victim->child[1]->parent = victim->parent;
		if (victim->parent) {
			if (victim->parent->child[0] == victim)
				victim->parent->child[0] = victim->child[0] != nullptr ? victim->child[0] : victim->child[1];
			else
				victim->parent->child[1] = victim->child[0] != nullptr ? victim->child[0] : victim->child[1];
		}
		if (victim == root) {
			root = victim->child[0] != nullptr ? victim->child[0] : victim->child[1];
		}
		victim->child[0] = victim->child[1] = nullptr;
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
 
b_r_tree & b_r_tree::operator=(const b_r_tree& other)
{
	if (&other != this) {
		delete root;
		seq.clear();
		root = nullptr;
		size = 0;
		copy_tree(other.root);

		seq.clear();

		for (size_t i = 0; i < other.seq.size(); ++i)
			seq.push_back(search(other.seq.at(i)->key));
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

b_r_tree b_r_tree::operator ^(const b_r_tree& other) const
{
	b_r_tree result = b_r_tree(*this);
	result = (result | other) / (result & other);
	return b_r_tree(result);
}

ostream& operator<<(ostream& os, b_r_tree& tree)
{
	os << "Tree [" << tree.size << "] : \t[ ";

	tree.put_all(os, tree.root);

	os << " ]\n";

	tree.put_Seq(os);

	os << " ]\n";

	return os;
}

void b_r_tree::put_all(ostream& os, node* n) {
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

void b_r_tree::put_Seq(ostream & os) {

	os << "Seq [" << seq.size() << "] : \t[ ";

	for (size_t i = 0; i < seq.size(); ++i)
		os << seq.at(i)->key << " \t";
}

void b_r_tree::erase(size_t left, size_t right)
{
	if (right >= seq.size())
		right = seq.size() - 1;

	if (left <= right) {

		vector<int> tempSeq;

		for (size_t i = 0; i < seq.size(); ++i)
			tempSeq.push_back(seq.at(i)->key);

		seq.clear();
		delete root;
		root = nullptr;
		size = 0;

		for (size_t i = 0; i < tempSeq.size(); ++i)
			if (!(i >= left && i <= right))
				add(tempSeq.at(i));

	}
}

void b_r_tree::excl(b_r_tree & exclSeq) {

	if (seq.size() >= exclSeq.seq.size() && exclSeq.seq.size() != 0) {

		vector<int> tempSeq;

		for (size_t i = 0; i < seq.size(); ++i)
			tempSeq.push_back(seq.at(i)->key);

		seq.clear();
		delete root;
		root = nullptr;
		size = 0;

		int j = 0;

		for (int i = 0; i < tempSeq.size(); ++i) {

			if (tempSeq.at(i) == exclSeq.seq.at(j)->key) {
				++j;
			}
			else {
				i -= j;
				j = 0;
			}

			if (j == exclSeq.seq.size()) {

				for (int g = i; g > i - j; --g)
					tempSeq.erase(tempSeq.begin() + g);

				i -= j;
				j = 0;
			}
		}

		for (size_t i = 0; i < tempSeq.size(); ++i)
			add(tempSeq.at(i));
	}
}


void b_r_tree::mul(size_t n)
{
	size_t tempSize = seq.size();

	for (size_t i = 0; i < n; ++i) {

		for (size_t j = 0; j < tempSize; ++j) {

			seq.push_back(seq[j]);
		}
	}
}