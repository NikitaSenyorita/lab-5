#include "b_r_tree.h"

b_r_tree::b_r_tree(const b_r_tree & set)// copy construltor 
{
	root = nullptr;
	size = 0;
	copy_tree(set.root);
}

b_r_tree::b_r_tree(b_r_tree && set) // bit-for-bit construltor 
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

/* функция для однократного поворота узла */
void node::rot_one(b_r_tree& tree, bool dir) {
	node* pivot = child[!dir];

	pivot->parent = parent; /* при этом, возможно, pivot становится корнем дерева */
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
		red_node->red = true; /* –инициализация красным цветом */
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
			else { // insert_case4(n);
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
				else { /* (n == n->parent->right) && (n->parent == g->right) */
					granda->rot_one(*this, 0);
				}
			}
			/*else {
				inserted->parent->red = false;
				inserted->parent->parent->red = true;
				rebalance_add(inserted->parent->parent);
			}*/
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
			//(victim == (victim->parent->child[0])) ? victim->parent->child[0] : victim->parent->child[1] = nullptr;
			}
		delete victim;	size--;
	}
	else if (victim->child[0] != nullptr ^ victim->child[1] != nullptr) {
		if (!victim->red) {
			if (victim->child[0] != nullptr)
				victim->child[0]->red = false;
			else
				victim->child[1]->red = false;
			//victim->child[0] != nullptr ? victim->child[0]->red : victim->child[1]->red = false;

		}
		if (victim->child[0] != nullptr)
			victim->child[0]->parent = victim->parent;
		else
			victim->child[1]->parent = victim->parent;
		//victim->child[0] != nullptr ? victim->child[0]->parent : victim->child[1]->parent = victim->parent;
		if (victim->parent) {
			if (victim->parent->child[0] == victim)
				victim->parent->child[0] = victim->child[0] != nullptr ? victim->child[0] : victim->child[1];// DANGEROUS
			else
				victim->parent->child[1] = victim->child[0] != nullptr ? victim->child[0] : victim->child[1];
			//victim->parent->child[0] == victim ? victim->parent->child[0] : victim->parent->child[1] = victim->child[0] != nullptr ? victim->child[0] : victim->child[1];
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

void b_r_tree::operator=(const b_r_tree& other)
{
	if (&other != this) {
		delete root;
		root = nullptr;
		size = 0;
		copy_tree(other.root);
	}
}
 
b_r_tree b_r_tree::operator=(b_r_tree& other)
{
	if (&other != this) {
		delete root;
		root = other.root;
		other.root = nullptr;/////////////
		size = other.size;
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