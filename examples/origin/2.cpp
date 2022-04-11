#include<iostream>
using namespace std;
int max(int a, int b) {
	if (a > b)return a;
	else return b;
}
class node {
public:
	int value;
	node* left;
	node* right;
	int height;
	int distance;
	node(int data=0,int h=0, node* leftchild = NULL, node* rightchild = NULL) {
		this->value = data;
		this->left = leftchild;
		this->right = rightchild;
		this->height = h;
		distance = 0;
	}
};
class avltree {
public:
	node* root;
	int mindis;
	avltree() { root = NULL; mindis = 100; }
	void insert_n(int number,node*&t) {
		if (t == NULL) {//根节点为空
			t = new node(number);
		}
		else {
			if (number > t->value) {//往右
				insert_n(number, t->right);
				if (get_height(t->right) - get_height(t->left) == 2) {
					if (get_height(t->right->left) > get_height(t->right->right)) {//RL
						t=rotate_rl(t);
					}
					else { t=rotate_l(t); }//RR
				}
			}
			else if (number == t->value)return;
			else {//往左
				insert_n(number, t->left);
				if (get_height(t->left) - get_height(t->right) == 2) {
					if (get_height(t->left->left) > get_height(t->left->right)) {//ll
						t=rotate_r(t);
					}
					else { t=rotate_lr(t); }//LR
				}
			}
		}
		t->height = get_height(t);
	}

	void insert(int number) {
		insert_n(number, root);
	}

	void remove_n(node*& x, int number) {
		if (x == NULL) { cout << "empty"; return; }
		else {
			if (number > x->value)remove_n(x->right, number);
			else if (number < x->value)remove_n(x->left, number);
			else {
				if (x->left != NULL && x->right != NULL) {
					node* p = x->left;
					while (p->right != NULL)p = p->right;
					x->value = p->value;
					remove_n(x->left, p->value);
				}

				else {
					node* p;
					if (x->left == NULL)p = x->right;
					else { p = x->left; }
					delete x;
					x = p;
					return;
				}
			}
				if ((get_height(x->left)) - (get_height(x->right)) == 2) {
					if (get_height(x->left->left) > get_height(x->left->right))
						x = rotate_r(x);
					else x = rotate_lr(x);
				}
				else if (get_height(x->right) - get_height(x->left) == 2) {
					if (get_height(x->right->right) > get_height(x->right->left))
						x = rotate_l(x);
					else x = rotate_rl(x);
				}
			}
		x->height = get_height(x);
	}
	void remove(int number) {
		remove_n(root, number);
	}

	node* rotate_r(node*& x) {//右旋
		node* temp = x->left;
		x->left = temp->right;
		temp->right = x;
		x->height = get_height(x);
		temp->height = get_height(temp);
		return temp;
	}

	node* rotate_l(node*& x) {//左旋
		node* temp = x->right;
		x->right = temp->left;
		temp->left = x;
		x->height = get_height(x);
		temp->height = get_height(temp);
		return temp;
	}

	node* rotate_lr(node*&x){//左节点左旋，节点右旋
		node* temp = x->left->right;
		x->left=rotate_l(x->left);
		x=rotate_r(x);
		return temp;
	}

	node* rotate_rl(node*& x) {//右节点右旋，节点左旋
		node* temp = x->right->left;
		x->right=rotate_r(x->right);
		x=rotate_l(x);
		return temp;
	}

	int get_height(node* x) {//获取节点高度
			if (x == NULL)return 0;
			int lh = 0, rh = 0;
			lh = get_height(x->left) + 1;
			rh = get_height(x->right) + 1;
			if (lh > rh)return lh;
			else return rh;
		}

	void dis(node* x,int number) {
		if(x!=NULL)x->distance = number;
		if (x ->left!= NULL) {
			dis(x->left, number + 1);
		}
		if (x->right != NULL) {
			dis(x->right, number + 1);
		}
	}
	void min(node*x) {//找出最近距离
		if (x != NULL) {
			if (x->left == NULL && x->right == NULL) {
				if (x->distance < mindis)mindis = x->distance;
			}
			min(x->left);
			min(x->right);
		}
	}

	void nearest_0(node* x) {
		if (x != NULL) {
			if (x->left == NULL && x->right == NULL) {
				if (x->distance == mindis)cout << x->value << " ";
			}
			nearest_0(x->left);
			nearest_0(x->right);
		}
	}

	void nearest() {
		//cout << "最近叶节点:";
		dis(root, 0);
		min(root);
		nearest_0(root);
	}

};

void show(node * tNode, int depth) {//通过depth控制缩进
	if (tNode != NULL) {
		show(tNode->right, ++depth);
		//cout << setfill('\t') << setw(depth) << tNode->value << endl;
		for (int i = 1; i < depth; i++) cout << '\t';
		//cout << tNode->value << endl;
		show(tNode->left, depth++);
	}
}



int main() {
	avltree avl;
	avl.insert(5);
	avl.insert(3);
	avl.insert(20);
	avl.insert(2);
	avl.insert(6);
	avl.insert(40);
	avl.insert(60);
	avl.insert(80);
	show(avl.root,0);
	avl.get_height(avl.root);
	avl.nearest();
	avl.remove(6);
	show(avl.root,0);
	avl.get_height(avl.root);
	avl.nearest();
	cout << "over"<<endl;
}

