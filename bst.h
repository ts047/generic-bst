#ifndef BST_H
#define BST_H
#include <iostream>
#include <cassert>
using namespace std;

template <class T>
class Node {

	public :
	T data;
	Node *left;
	Node *right;
	Node *parent;
	int height;

	Node(T key): data(key), left(0), right(0), parent(0), height(1) {}
};


template <class T>
class Tree {

    protected :
    Node<T> *root;
    int count;

    public :

    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;

    Tree() : root(0), count(0) {};
    ~Tree();

    void insert(T key);
    void remove(T &key);
    void clear();

    void freeNode(Node<T> *);
    void printTest();
    void inorderIterateTest(Node<T> *);
    int getHeight(Node<T> *);
    void setHeight(Node<T> *);
    Node<T> *min(Node<T> *);

};

template <class T>
class AVLtree : public Tree<T>
{

    public :
    AVLtree() : Tree<T>() {};
    ~AVLtree() {};
    Node<T>* insert_helper(Node<T> *,Node<T> *, Node<T> *);
    void insert(T key);
    Node<T>* rotate(Node<T> *, Node<T> *, Node<T> *,int);
    Node<T>* leftRotate(Node<T> *, Node<T> *, Node<T> *);
    Node<T>* rightRotate(Node<T> *, Node<T> *, Node<T> *);

 
    void remove(T key);
    Node<T>* removeHelper(Node<T> *, T );
};



template <class T>
Node<T> * Tree<T>::min(Node<T> *root) 
{
    if(!root) return 0;
    else {
	while(root && root->left) root = root->left;
	return root;
    }
}


template <class T>
void Tree<T>::setHeight(Node<T> *root)
{
    root->height = max(this->getHeight(root->left), this->getHeight(root->right)) + 1;
}

template <class T>
Node<T>* AVLtree<T>::removeHelper(Node<T> *root, T key)
{
    Node<T> *temp = 0;
    if(root) {
	if(root->data == key) {
	    if(!root->left && !root->right) { // leaf node 
		delete(root);
		return 0;
	    } else if(root->left && root->right) {  // have both child
		temp = this->min(root->right);
		root->data = temp->data;
		root->right = removeHelper(root->right, temp->data);
		this->setHeight(root);
		return root;
	    } else if(root->left) {
		temp = root->left;
		temp->parent = root->parent;
		delete(root);
		return temp;
	    } else {
		temp = root->right;
		temp->parent = root->parent;
		delete(root);
		return temp;
	    }

	} else if(root->data < key) {
	    root->right = removeHelper(root->right, key);
	    this->setHeight(root);
	}else {
	    root->left = removeHelper(root->left, key);
	    this->setHeight(root);
	}
    }

}

template<class T>
void AVLtree<T>::remove(T key)
{
    if(this->root) this->root = removeHelper(this->root, key);
}

template < class T>
int Tree<T>::getHeight(Node<T> *root)
{
    return root ? root->height : 0;
}

template<class T>
Node<T> * AVLtree<T>::leftRotate(Node<T> *parent, Node<T> *curr, Node<T> *child)
{
    curr->right = child->left;
    child->left = curr;
    child->parent = parent;
    curr->parent = child;
    curr->height = max(this->getHeight(curr->left), this->getHeight(curr->left)) + 1;
    child->height = max(this->getHeight(child->left), this->getHeight(child->right)) + 1;
    return child;
}

template<class T>
Node<T> *AVLtree<T>::rightRotate(Node<T> *parent, Node<T> *curr, Node<T> *child)
{
    curr->left = child->right;
    child->right = curr;
    child->parent = parent;
    curr->parent = child;
    child->height = max(this->getHeight(child->left), this->getHeight(child->right)) + 1;
    curr->height = max(this->getHeight(curr->left), this->getHeight(curr->right)) + 1;
    return child;
}

template<class T>
Node<T>* AVLtree<T>::rotate(Node<T> *parent, Node<T> *curr, Node<T> *key, int h_diff)
{
    if(h_diff < -1 ) {
	if(curr->right->data < key->data)
	    return leftRotate(parent, curr, curr->right);
	else {
	    curr->right = rightRotate(curr, curr->right, curr->right->left);
	    return leftRotate(parent, curr, curr->right);
	}
    }
    else {
	if(key->data < curr->left->data)	return rightRotate(parent, curr, curr->left);
	else {
	    curr->left = leftRotate(curr, curr->left, curr->left->right);
	    return rightRotate(parent, curr, curr->left);
	}
    }
}

template <class T>
Node<T>* AVLtree<T>::insert_helper(Node<T> *root, Node<T> *parent, Node<T> *key)
{
    if(root) {
	if (root->data == key->data)
	    return root;
	else if(root->data < key->data)
	    root->right = insert_helper(root->right, root, key);
	else
	    root->left = insert_helper(root->left, root, key);

	int h_diff = this->getHeight(root->left) - this->getHeight(root->right);
	if(h_diff > 1 || h_diff < -1)
	    root = rotate(parent, root, key, h_diff);

	root->height = max(this->getHeight(root->left), this->getHeight(root->right)) + 1;
    } else {
	key->parent = parent;
	return key;
    }

    return root;
}

template <class T>
void AVLtree<T>::insert(T key)
{
    Node<T> *temp = 0;
    temp = new Node<T>(key);
    assert(temp);

    if(!this->root) this->root = temp;
    else this->root = insert_helper(this->root, 0, temp);
}

template <class T>
void Tree<T>::insert(T key) {
    Node<T> *temp = 0;
    
    temp = new Node<T>(key);
    assert(temp);

    if(!root) root = temp;
    else {
	Node<T> *parent = 0;
	Node<T> *curr = root;
	bool isleft = false;

	while(curr) {
	    if(curr->data == key)
		return ;
	    else if (curr->data < key) {
		parent = curr;
		isleft = false;
		curr = curr->right;
	    } else {
		parent = curr;
		isleft = true;
		curr = curr->left;
	    }
	}
	assert(parent);
	if(isleft) parent->left = temp;
	else 
	    parent->right = temp;
	temp->parent = parent;

    }
}

template <class T>
void Tree<T>::freeNode(Node<T> *node)
{
    if(node) {
	freeNode(node->left);
	freeNode(node->right);
	delete node;
    }
}

template <class T>
Tree<T>::~Tree() 
{ 
    if(root) {
	freeNode(root);
    }	
};

template <class T>
void Tree<T>::clear()
{
    if(root) freeNode(root);
}

template <class T>
void Tree<T>::inorderIterateTest(Node<T> *t) {
    if(t) {
	inorderIterateTest(t->left);
	cout << t->data << " ";
	inorderIterateTest(t->right);
    }
}

template <class T>
void Tree<T>::printTest() {
    inorderIterateTest(root);
}
#endif
