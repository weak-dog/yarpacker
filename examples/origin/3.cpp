#include<iostream>
using namespace std;
class node {
public:
	int value;
	node* left;
	node* right;
	node* parent;
	int color;//0 is black,1 is red
	node(int value = 0, int color = 1,node*left=NULL,node*right=NULL,node*parent=NULL)
	{
		this->value = value;
		this->left = left;
		this->right = right;
		this->parent = parent;
		this->color = color;
	}
};

class brtree {
public:
	node* root;
	brtree(node* root = NULL) { this->root = root; }
	int get_color(node* x);
	node* rotate_r(node* x);
	node* rotate_l(node* x);
	void insert_adjust(node*& x);
	void insert(int number);
	node* search(int number);
	node* heir(node* x);
	node* front(node* x);
	void adjust(node*y);
	void delete_n(int number);
	void delete_n(node*x);
};

int brtree::get_color(node*x){
	if (x == NULL)return 0;
	else {
		if (x->color == 0)return 0;
		else return 1;
	}
}

node* brtree::rotate_r(node* x) {//右旋
	if (x == NULL)return NULL;
	node* temp = x->left;
	x->left = temp->right;
	if (temp->right != NULL)(temp->right)->parent = x;
	temp->parent = x->parent;
	if (x->parent == NULL)root = temp;
	else {
		if (x->parent->right == x)x->parent->right = temp;
		else { x->parent->left = temp; }
	}
	temp->right = x;
	x->parent = temp;
	return temp;
}
node* brtree::rotate_l(node* x) {//左旋
	if (x == NULL)return NULL;
	node* temp = x->right;
	x->right = temp->left;
	if (temp->left != NULL)(temp->left)->parent = x;
	temp->parent = x->parent;
	if (x->parent == NULL)root = temp;
	else {
		if (x->parent->right == x)x->parent->right = temp;
		else { x->parent->left = temp; }
	}
	temp->left = x;
	x->parent = temp;
	return temp;
}

void brtree::insert(int number) {
	if (root == NULL) {
		root = new node(number, 0);
		return;
	}
	node* temp = NULL;
	node* r = root;
	while (r != NULL) {//temp指向将插入的位置的父节点
		temp = r;
		if (number < r->value)r = r->left;
		else if (number > r->value)r = r->right;
		else { return; }
	}

	node* m = new node(number, 1);
	m->parent = temp;
	if (number < temp->value)temp->left = m;
	else temp->right = m;
	insert_adjust(m);
}
void brtree::insert_adjust(node*& x) {
	while ((x != NULL && x != root && get_color(x->parent) == 1)) {
		if (x == x->parent->left && (x->parent == x->parent->parent->left)) {//LL
			if (x->parent->parent->right == NULL) {//LLb
				x->parent->color = 0;
				x->parent->parent->color = 1;
				rotate_r(x->parent->parent);
			}
			else if (x->parent->parent->right->color == 1) {//LLr
				x->parent->color = 0;
				x->parent->parent->right->color = 0;
				x->parent->parent->color = 1;
				x = x->parent->parent;
			}
			else {//LLb
				x->parent->color = 0;
				x->parent->parent->color = 1;
				rotate_r(x->parent->parent);
			}
		}
		else if (x == x->parent->right && (x->parent == x->parent->parent->left)) {//LR
			if (x->parent->parent->right == NULL) {//LRb
				x = x->parent;
				rotate_l(x);
				x->parent->color = 0;
				x->parent->parent->color = 1;
				rotate_r(x->parent->parent);
			}
			else if (x->parent->parent->right->color == 1) {//LRr
				x->parent->color = 0;
				x->parent->parent->right->color = 0;
				x->parent->parent->color = 1;
				x = x->parent->parent;
			}
			else {//LRb
				x = x->parent;
				rotate_l(x);
				x->parent->color = 0;
				x->parent->parent->color = 1;
				rotate_r(x->parent->parent);
			}
		}
		else if (x == x->parent->right && (x->parent == x->parent->parent->right)) {//RR

			if (x->parent->parent->left == NULL) {
				x->parent->color = 0;
				x->parent->parent->color = 1;
				rotate_l(x->parent->parent);
			}
			else if (x->parent->parent->left->color == 1) {//RRr
				x->parent->color = 0;
				x->parent->parent->left->color = 0;
				x->parent->parent->color = 1;
				x = x->parent->parent;
			}
			else {//RRb
				x->parent->color = 0;
				x->parent->parent->color = 1;
				rotate_l(x->parent->parent);
			}
		}

		else if (x == x->parent->left && (x->parent == x->parent->parent->right)) {//RL
			if (x->parent->parent->left == NULL) {//RLb
				x = x->parent;
				rotate_r(x);
				x->parent->color = 0;
				x->parent->parent->color = 1;
				rotate_l(x->parent->parent);
			}
			else if (x->parent->parent->left->color == 1) {//RLr
				x->parent->color = 0;
				x->parent->parent->left->color = 0;
				x->parent->parent->color = 1;
				x = x->parent->parent;
			}
			else {//RLb
				x = x->parent;
				rotate_r(x);
				x->parent->color = 0;
				x->parent->parent->color = 1;
				rotate_l(x->parent->parent);
			}
		}

	}
	root->color = 0;
}

node* brtree::heir(node* x) {//后继节点，L型
	node* p = x->right;
	if (p == NULL)return p;
	while (p->left != NULL) {
		p = p->left;
	}
	return p;
}
node* brtree::front(node* x) {//前节点,R型
	node* p = x->left;
	if (p == NULL)return p;
	while (p->right != NULL) {
		p = p->right;
	}
	return p;
}

node* brtree::search(int number) {
	node* x = root;
	while (x != NULL) {
		if (number < x->value)x = x->left;
		else if (number > x->value)x = x->right;
		else return x;
	}
	return NULL;
}

void brtree::delete_n(int number) {
	node* p = search(number);
	if (p != NULL)delete_n(p);
}
void brtree::delete_n(node* x) {
	if (x->left != NULL && x->right != NULL) {//两个孩子,转化为一个孩子
		node* f = heir(x);
		x->value = f->value;
		x = f;
	}
	//x为待删除
	node* temp;
	if (x->left != NULL)temp = x->left;
	else temp = x->right;
	if (temp != NULL) {
		temp->parent = x->parent;
		if (x->parent == NULL) {//根节点
			root = temp;
		}
		else if (x == x->parent->left)x->parent->left = temp;
		else x->parent->right = temp;

		x->left = x->right = x->parent = NULL;//删除x
		if (get_color(x) == 0)adjust(temp);
	}
	else if (x->parent == NULL) { root = NULL; }//只有根节点
	else {//没孩子
		if (get_color(x) == 0)adjust(x);
		if (x->parent != NULL) {
			if (x == x->parent->left)x->parent->left = NULL;
			else x->parent->right = NULL;
			x->parent = NULL;//删除x
		}
	}
}
void brtree::adjust(node* x) {
	while (x != root && get_color(x) == 0) {//不为根节点且为黑是要调整
		if (x == x->parent->left) {//L
			node* v = x->parent->right;
			if (get_color(v) == 1) {//Lr
				v->color = 0;
				x->parent->color = 1;
				rotate_l(x->parent);
				v = x->parent->right;
			}
			if (get_color(v->left) == 0 && get_color(v->right) == 0) {
				v->color = 1;
				x = x->parent;
			}
			else {
				if (get_color(v->right) == 0) {
					v->left->color = 0;
					v->color = 1;
					rotate_r(v);
					v = x->parent->right;
				}
				v->color = get_color(x->parent);
				x->parent->color = 0;
				v->right->color = 0;
				rotate_l(x->parent);
				x = root;
			}
		}
		else {//R
			node* v = x->parent->left;
			if (get_color(v) == 1) {//Rr
				v->color = 0;
				x->parent->color = 1;
				rotate_r(x->parent);
				v = x->parent->left;
			}
			if (get_color(v->left) == 0 && get_color(v->right) == 0) {
				v->color = 1;
				x = x->parent;
			}
			else {
				if (get_color(v->left) == 0) {
					v->right->color = 0;
					v->color = 1;
					rotate_l(v);
					v = x->parent->left;
				}
				v->color = get_color(x->parent);
				x->parent->color = 0;
				v->left->color = 0;
				rotate_r(x->parent);
				x = root;
			}
		}
		x->color = 0;
	}
}

void show(node* tNode, int depth) {
	if (tNode != NULL) {
		show(tNode->right, ++depth);
		for (int i = 1; i < depth; i++) cout << '\t';
		cout << tNode->value << " ";
		if (tNode->color == 0)cout << "b";
		else cout << "r";
		cout << endl;
		show(tNode->left, depth++);
	}
}

void ascent(node* x) {
	if (x != NULL) {
		ascent(x->left);
		cout << x->value << " ";
		ascent(x->right);
	}
}


int main() {
	brtree a;
	a.insert(1);
	a.insert(4);
	a.insert(5);
	a.insert(-1);
	a.insert(4);
	a.insert(7);
	a.insert(6);
	a.insert(8);
	a.insert(18);
	a.insert(28);
	a.insert(38);
	a.insert(58);
	a.insert(88);
	a.insert(9);
	a.insert(18);
	a.delete_n(6);
	a.delete_n(4);
	cout<<"over"<<endl;
}

