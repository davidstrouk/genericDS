#ifndef tree_h
#define tree_h

#include <iostream>

template <class T>
class Node {
public:
    T _value;
    int _height;

    Node* _father;
    Node* _left_son;
    Node* _right_son;

    Node(T& value, Node<T>* father);
    Node(Node<T>* father);
};

template <class T>
Node<T>::Node(T& value, Node<T>* father): _value(value), _height(1), _father(father),
_left_son(NULL), _right_son(NULL) {}

template <class T>
Node<T>::Node(Node<T>* father): _value(), _height(1), _father(father), _left_son(NULL), _right_son(NULL) {}

template <class T, class Comp>
class Tree {

public:

    Node<T> * _root;
    int _node_number;
    Comp _compare;

    Tree();
    Tree(Node<T> * root);
    ~Tree();
    void clean();

    Node<T> * find(T& value);

    int getNodeNumber();
    int getHeight();

    void insert(T& value);
    void remove(T& value);

private:

    void insertAux(T& value, Node<T>* node);
    void removeAux(T& value, Node<T>* node);
    void cleanAux(Node<T>* node);

    Node<T>* findAux(T& value, Node<T>* node);

    T getMinAux(Node<T>* node);
    int getHeightAux(Node<T>* node) ;

	void rightRotation(Node<T>* node);
	void leftRotation(Node<T>* node);
	void balanceTree(Node<T>* node);
	void balanceTreeAux(Node<T>* node);

	int getNodeHeight(Node<T>* node);
	void updateHeight(Node<T>* node);
};

template <class T, class Comp>
Tree<T, Comp>::Tree() {
	_root=NULL;
	_node_number = 0;
	Comp compare;
	_compare = compare;
}

template <class T, class Comp>
Tree<T, Comp>::Tree(Node<T>* root) : _root(root) {
	if(!_root) {
		_node_number = 0;
	}
	else _node_number = 1;
	Comp compare;
	_compare = compare;
}

template <class T, class Comp>
Tree<T, Comp>::~Tree(){
	clean();
}

template <class T, class Comp>
void Tree<T, Comp>::cleanAux(Node<T> * p) {
	if(!p) return;
	cleanAux(p->_left_son);
	cleanAux(p->_right_son);
	delete p;
	return;
}

template <class T, class Comp>
void Tree<T, Comp>::clean() {
	cleanAux(_root);
	_node_number = 0;
	_root = NULL;
	return;
}

template <class T, class Comp>
Node<T> * Tree<T, Comp>::findAux(  T& _value, Node<T> * p){
    if(!p) return NULL;
    if (_compare(_value,p->_value) == 0) return p;

    if(_compare(_value,p->_value) > 0) {
        return findAux(_value, p->_right_son);
    }
    if(_compare(_value,p->_value) < 0){
        return findAux(_value, p->_left_son);
    }
    return NULL;
}

template <class T, class Comp>
Node<T> * Tree<T, Comp>::find(T& value) {
    return findAux(value, _root);
}

template <class T, class Comp>
void Tree<T, Comp>::insertAux(T& value, Node<T>* node) {
    if(_compare(value,node->_value) > 0) {
        if(!node->_right_son) {
            node->_right_son = new Node<T>(value, node);
            _node_number++;
            balanceTree(node);
            return;
        }
        insertAux(value, node->_right_son);
    }
    if(_compare(value, node->_value) < 0){
        if(!node->_left_son) {
            node->_left_son = new Node<T>(value, node);
            _node_number++;
            balanceTree(node);
            return;
        }
        insertAux(value, node->_left_son);
    }
    return;

}

template <class T, class Comp>
void Tree<T, Comp>::insert(T& value) {
    if(!_root) {
        _root=new Node<T>(value, NULL);
        _node_number++;
        return;
    }
    insertAux(value, _root);
    return;
}

template <class T, class Comp>
void Tree<T, Comp>::remove(T& _value) {
	if(_node_number == 1) {
		Node<T> * tmp= _root;
		_root= NULL;
		delete tmp;
		_node_number = 0;
		return;
	}
	removeAux(_value, _root);
	return;
}

template <class T, class Comp>
void Tree<T, Comp>::removeAux(  T& _value, Node<T> * node) {
	if(!node) return;
	if(_compare(_value, node->_value) > 0) {
		removeAux(_value, node->_right_son);
	}
	if(_compare(_value, node->_value) < 0) {
		removeAux(_value, node->_left_son);
	}

	if(_compare(_value, node->_value) == 0) {
		if(!node->_left_son && !node->_right_son) {
			Node<T> * father = node->_father;
			(father->_left_son == node)? father->_left_son = NULL : father->_right_son = NULL;
			delete node;
			_node_number--;
			balanceTree(father);
			return;
		}
		if(!node->_left_son || !node->_right_son) {

			Node<T> * father = node->_father;
			if(!father) {
				_root = (node->_right_son)? node->_right_son : node->_left_son;
				_root->_father = NULL;
				delete node;
				_node_number--;
				return;
			}
			if(father->_left_son == node) {
				(node->_right_son)? father->_left_son = node->_right_son : father->_left_son = node->_left_son;
				(node->_right_son)? node->_right_son->_father = father : node->_left_son->_father = father;
			}
			if(father->_right_son == node) {
				(node->_right_son)? father->_right_son = node->_right_son : father->_right_son = node->_left_son;
				(node->_right_son)? node->_right_son->_father = father : node->_left_son->_father = father;
			}
			delete node;
			_node_number--;
			balanceTree(father);
			return;
		}
		if(node->_left_son && node->_right_son) {
			node->_value = getMinAux(node->_right_son);
			removeAux(node->_value, node->_right_son);
		}
	}
}

template <class T, class Comp>
int Tree<T, Comp>::getNodeNumber()   {
    return _node_number;
}

template <class T, class Comp>
T Tree<T, Comp>::getMinAux(Node<T> * p)  {
    if(!p->_left_son) {
        return p->_value;
    }
    return getMinAux(p->_left_son);
}

template <class T, class Comp>
int Tree<T, Comp>::getHeight() {
	return getHeightAux(_root);
}

template <class T, class Comp>
int Tree<T, Comp>::getHeightAux(Node<T> * node) {
	if(!node) return 0;
	int heightLeft = getHeightAux(node->_left_son);
	int heightRight = getHeightAux(node->_right_son);
	int max = heightRight;
	if(heightLeft >= heightRight) max = heightLeft;
	return max + 1;
}

template <class T, class Comp>
void Tree<T, Comp>::leftRotation(Node<T> * node) {
	if(!node) return;
	Node<T> * a = node;
	Node<T> * b = node->_right_son;

	if(a->_father) {
		if(a->_father->_right_son == a) a->_father->_right_son = b;
		else a->_father->_left_son = b;
	}
	b->_father = a->_father;

	if(b->_left_son) b->_left_son->_father = a;
	a->_right_son = b->_left_son;

	a->_father = b;
	b->_left_son = a;
	if(a == _root) _root = b;
	a->_height = getNodeHeight(a);
	b->_height = getNodeHeight(b);
	updateHeight(node);
	return;
}

template <class T, class Comp>
void Tree<T, Comp>::rightRotation(Node<T>* node) {
	if(!node) return;
	Node<T> * b = node;
	Node<T> * a = node->_left_son;

	if(b->_father) {
		if(b->_father->_right_son == b) b->_father->_right_son = a;
		else b->_father->_left_son = a;
	}
	a->_father = b->_father;

	if(a->_right_son) a->_right_son->_father = b;
	b->_left_son = a->_right_son;

	b->_father = a;
	a->_right_son = b;
	if(b == _root) _root = a;
	b->_height = getNodeHeight(b);
	a->_height = getNodeHeight(a);
	updateHeight(node);
	return;
}

template <class T, class Comp>
void Tree<T, Comp>::balanceTree(Node<T> * node){
	updateHeight(node);
	balanceTreeAux(node);
}

template <class T, class Comp>
void Tree<T, Comp>::balanceTreeAux(Node<T>* node) {
	if(!node) return;

	int root_balance = getNodeHeight(node->_left_son) - getNodeHeight(node->_right_son);

	if(root_balance == 2) {

		int left_son_balance = getNodeHeight(node->_left_son->_left_son) - getNodeHeight(node->_left_son->_right_son);

		if(left_son_balance >= 0) {
			rightRotation(node);
		}

		if(left_son_balance == -1) {
			leftRotation(node->_left_son);
			rightRotation(node);
		}
	}

	if(root_balance == -2) {

		int right_son_balance = getNodeHeight(node->_right_son->_left_son) - getNodeHeight(node->_right_son->_right_son);

		if(right_son_balance <= 0){
			leftRotation(node);
		}

		if(right_son_balance == 1) {
			rightRotation(node->_right_son);
			leftRotation(node);
		}
	}
	balanceTree(node->_father);
}

template <class T, class Comp>
int Tree<T, Comp>::getNodeHeight(Node<T>* node) {
	if(!node) return 0;
	int height_left = 0, height_right = 0;
	if(node->_left_son) height_left = node->_left_son->_height;
	if(node->_right_son) height_right = node->_right_son->_height;
	int max = height_right;
	if(height_left > height_right) max = height_left;
	return max + 1;
}

template <class T, class Comp>
void Tree<T, Comp>::updateHeight(Node<T>* node) {
	if(!node) return;
	node->_height = getNodeHeight(node) ;
	updateHeight(node->_father);
}

#endif /* tree_h */
