//#include "TNode.h"
#include "RBTree.h"
#include <iostream>
using namespace std;

	template <class DT>
TNode<DT>::TNode(DT* d)
{
	parent = RBTree<DT>::nil;
	left = RBTree<DT>::nil;
	right = RBTree<DT>::nil;
	data = *d;
	color = BLACK;
}
	template <class DT>
TNode<DT>::TNode(DT d)
{
	parent = RBTree<DT>::nil;
	left = RBTree<DT>::nil;
	right = RBTree<DT>::nil;
	data = d;
	color = BLACK;
}
	template <class DT>
TNode<DT>::TNode(DT d, color_t c)
{
	parent = RBTree<DT>::nil;
	left = RBTree<DT>::nil;
	right = RBTree<DT>::nil;
	data = d;
	color = c;
}
	template <class DT>
RBTree<DT>::RBTree()
{
	root = RBTree<DT>::nil;
	numElements = 0;
}

	template <class DT>
RBTree<DT>::~RBTree()
{
	cleanNodes();
}

	template <class DT>
TNode<DT>* RBTree<DT>::createNode()
{
	DT* data = new DT();
	TNode<DT>* newNode = new TNode<DT>(data);
	return newNode;
}
	template <>
TNode<int>* RBTree<int>::createNode()
{
	int* data = new int();
	cout << "What would you like to insert?" << endl;
	cin >> *data;
	TNode<int>* newNode = new TNode<int>(data);
	return newNode;
}

	template <class DT>
void RBTree<DT>::rbtInsert()
{
	rbtInsert(createNode());
}
	template <class DT>
void RBTree<DT>::rbtInsert(DT data)
{
	TNode<DT>* node = new TNode<DT>(data);
	rbtInsert(node);
}
template <class DT>
size_t RBTree<DT>::size()
{
	return numElements;
}
	template <class DT>
void RBTree<DT>::rbtInsert(TNode<DT>* z)
{
//	cout << "inserting " << z->data << endl;
	numElements++;
	TNode<DT>* y = RBTree<DT>::nil;
	TNode<DT>* x = root;
	while(x != RBTree<DT>::nil)
	{
		y = x;
		if(z->data < x->data)
			x = x->left;
		else
			x = x->right;
	}
	z->parent = y;
	if(y == RBTree<DT>::nil)
		root = z;
	else if(z->data < y->data)
		y->left = z;
	else
		y->right = z;
	z->left = RBTree<DT>::nil;
	z->right = RBTree<DT>::nil;
	z->color = RED;
	rbtInsFixup(z);
	/*if(root == RBTree<DT>::nil)
	  root = newNode;
	  else
	  {
	  TNode<DT>* current = root;
	  TNode<DT>* prev = root;
	//cout << "inserting " << newNode->data << endl;
	while(current != RBTree<DT>::nil)
	{
	prev = current;
	if(newNode->data <= current->data)
	current = current->left;
	else if(newNode->data > current->data)
	current = current->right;
	}//end while
	if(newNode->data <= prev->data)
	prev->left = newNode;
	else if(newNode->data > prev->data)
	prev->right = newNode;
	newNode->parent = prev;
	}
	newNode->color = RED;
	newNode->left = RBTree<DT>::nil;
	newNode->right = RBTree<DT>::nil;
	rbtInsFixup(newNode);*/
}
	template <class DT>
void RBTree<DT>::rbtDelete(DT data)
{

	/*TNode<DT>* node();
	  do{
	//find the node w/ data and keep pushing it down to successor until it has only one child
	node = rbtSearch(data);
	if(node->right != RBTree<DT>::nil)
	{
	TNode<DT>* temp = successor(node);
	DT d = node->data;
	node->data = temp->data;
	temp->data = d;
	}
	}while(node->left != RBTree<DT>::nil && node->right != RBTree<DT>::nil);
	rbtDelFixup(node);*/
	rbtDelete(rbtSearch(data));
}
	template <class DT>
void RBTree<DT>::rbtDelete(TNode<DT>* z)
{
//	cout << "deleting " << z->data << endl;
	numElements--;
	TNode<DT>* y = z;
	color_t y_orig_color = y->color;
	TNode<DT>* x;

	if(z->left == RBTree<DT>::nil)
	{
		x = z->right;
		transplant(z, z->right);
	}else if(z->right == RBTree<DT>::nil){
		x = z->left;
		transplant(z, z->left);
	}else{
		y = successor(z);
		y_orig_color = y->color;
		x = y->right;
		if(y->parent == z)
			x->parent = y;
		else
		{
			transplant(y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		transplant(z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}
	if(y_orig_color == BLACK)
		rbtDelFixup(x);
	/*
	   if(node == RBTree<DT>::nil)
	   cout << "Couldn't delete" << endl;
	   else
	   {
	//cout << "deleting " << node->data << endl;
	TNode<DT>* del = node;
	TNode<DT>* replace = node;
	if(node->left == RBTree<DT>::nil || node->right == RBTree<DT>::nil)
	del = node;
	else
	del = successor(node);
	if(del->left != RBTree<DT>::nil)
	replace = del->left;
	else
	replace = del->right;
	transplant(del, replace);
	//if the node to be deleted is a leaf del will = replace
	if( del != replace)
	node->data = del->data;
	delete del;
	}
	 */
}
	template <class DT>
void RBTree<DT>::rbtInsFixup(TNode<DT>* z)
{
	while(z->parent->color == RED)
	{
		if(z->parent == z->parent->parent->left)
		{
			TNode<DT>* y = z->parent->parent->right;
			if(y->color == RED)
			{
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else
			{
				if(z == z->parent->right)
				{
					z = z->parent;
					lRotate(z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rRotate(z->parent->parent);
			}	
		}
		else
		{
			TNode<DT>* y = z->parent->parent->left;
			if(y->color == RED)
			{
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else
			{
				if(z == z->parent->left)
				{
					z = z->parent;
					rRotate(z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				lRotate(z->parent->parent);
			}	
		}
	}
	root->color = BLACK;
}
	template <class DT>
void RBTree<DT>::rbtDelFixup(TNode<DT>* x)
{
	while(x!= root && x->color == BLACK)
	{
		if(x == x->parent->left)
		{
			TNode<DT>* w = x->parent->right;
			if(w->color == RED)
			{
				w->color = BLACK;
				x->parent->color = RED;
				lRotate(x->parent);
				w = x->parent->right;
			}
			if(w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;
				x = x->parent;
			}else{
				if(w->right->color == BLACK)
				{
					w->left->color = BLACK;
					w->color = RED;
					rRotate(w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				lRotate(x->parent);
				x = root;
			}
		}else{
			TNode<DT>* w = x->parent->left;
			if(w->color == RED)
			{
				w->color = BLACK;
				x->parent->color = RED;
				rRotate(x->parent);
				w = x->parent->left;
			}
			if(w->right->color == BLACK && w->left->color == BLACK)
			{
				w->color = RED;
				x = x->parent;
			}else{
				if(w->left->color == BLACK)
				{
					w->right->color = BLACK;
					w->color = RED;
					lRotate(w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				rRotate(x->parent);
			}
		}
	}//while
	x->color = BLACK;
}

	template <class DT>
TNode<DT>* RBTree<DT>::rbtSearch(DT* dataPtr)
{
	return rbtSearch(*dataPtr);
}
	template <class DT>
TNode<DT>* RBTree<DT>::rbtSearch(DT data)
{
	TNode<DT>* current = root;
	while(current != RBTree<DT>::nil)
	{
		if(current->data == data)
		{
			//cout << "found" <<endl;
			return current;
		}
		if(data <= current->data)// && current->left != RBTree<DT>::nil)
			current = current->left;
		else if(data > current->data)// && current->right != RBTree<DT>::nil)
			current = current->right;
	}
	cout << "node not found" << endl;
	return RBTree<DT>::nil;//if the loop exits without returning that means the data it's 	looking for isn't in the tree
}

	template <class DT>
void RBTree<DT>::cleanNodes()
{
	cleanNodes(root);
}

	template <class DT>
void RBTree<DT>::cleanNodes(TNode<DT>* x)
{
	if(x != RBTree<DT>::nil)
	{
		cleanNodes(x->left);
		cleanNodes(x->right);
		delete x;
	}
}

	template <class DT>
TNode<DT>* RBTree<DT>::max()
{
	max(root);
}

	template <class DT>
TNode<DT>* RBTree<DT>::max(TNode<DT>* max)
{
	if(max != RBTree<DT>::nil)
		while(max->right != RBTree<DT>::nil)
		{
			max = max->right;
		}
	return max;
}

	template <class DT>
TNode<DT>* RBTree<DT>::min()
{
	min(root);
}

	template <class DT>
TNode<DT>* RBTree<DT>::min(TNode<DT>* min)
{
	if(min != RBTree<DT>::nil)	
		while(min->left != RBTree<DT>::nil)
		{
			min = min->left;
		}
	return min;
}

	template <class DT>
TNode<DT>* RBTree<DT>::successor()
{
	successor(root);
}

	template <class DT>
TNode<DT>* RBTree<DT>::successor(TNode<DT>* current)
{
	return min(current->right);
}

	template <class DT>
TNode<DT>* RBTree<DT>::predecessor()
{
	predecessor(root);
}

	template <class DT>
TNode<DT>* RBTree<DT>::predecessor(TNode<DT>* current)
{
	return max(current->left);
}
	template <class DT>
void RBTree<DT>::transplant(TNode<DT>* del, TNode<DT>* replace)
{
	if(del->parent == RBTree<DT>::nil)
		root = replace;
	else if(del == del->parent->left)
		del->parent->left = replace;
	else
		del->parent->right = replace;
	replace->parent = del->parent;
	/*
	   if(replace != RBTree<DT>::nil)
	   replace->parent = del->parent;
	   if(del->parent == RBTree<DT>::nil)
	   root = replace;
	   else if(del == del->parent->left)
	   del->parent->left = replace;
	   else
	   del->parent->right = replace; 
	 */
}
	template <class DT>
void RBTree<DT>::lRotate(TNode<DT>* x)
{
	if(x == RBTree<DT>::nil)
		cout << "x is nil" << endl;
	else{
		TNode<DT>* y =  x->right;
		x->right = y->left;
		if(y->left != RBTree<DT>::nil)
			y->left->parent = x;
		y->parent = x->parent;
		if(x->parent != RBTree<DT>::nil)
			if(x == x->parent->left)
				x->parent->left = y;
			else
				x->parent->right = y;
		else
			root = y;
		y->left = x;
		x->parent = y;
	}
}
	template <class DT>
void RBTree<DT>::rRotate(TNode<DT>* x)
{
	TNode<DT>* y =  x->left;
	x->left = y->right;
	if(y->right != RBTree<DT>::nil)
		y->right->parent = x;
	y->parent = x->parent;
	if(x->parent != RBTree<DT>::nil)
		if(x == x->parent->right)
			x->parent->right = y;
		else
			x->parent->left = y;
	else
		root = y;
	y->right = x;
	x->parent = y;
}
	template <class DT>
void RBTree<DT>::inOrder()
{
	inOrder(root);
}

	template <class DT>
void RBTree<DT>::inOrder(TNode<DT>* current)
{
	if(current != RBTree<DT>::nil)
	{
		inOrder(current->left);
		cout << current->data << ", ";
		inOrder(current->right);
	}
}

	template <class DT>
void RBTree<DT>::preOrder()
{
	preOrder(root);
}

	template <class DT>
void RBTree<DT>::preOrder(TNode<DT>* current)
{
	if(current != RBTree<DT>::nil)
	{
		cout << current->data << ", ";
		preOrder(current->left);
		preOrder(current->right);
	}
}

	template <class DT>
void RBTree<DT>::postOrder()
{
	postOrder(root);
}

	template <class DT>
void RBTree<DT>::postOrder(TNode<DT>* current)
{
	if(current != RBTree<DT>::nil)
	{
		postOrder(current->left);
		postOrder(current->right);
		cout << current->data << ", ";
	}
}

	template <class DT>
DT* RBTree<DT>::rbtToArray(DT* array, int index, TNode<DT>* current)
{

	//	if(current != RBTree<DT>::nil)
	//	{
	//		array[index] = current;
	//		rbtToArray(array, index/2, current->left);
	//		rbtToArray(array, index + index/2, current->right);
	//	}
}
