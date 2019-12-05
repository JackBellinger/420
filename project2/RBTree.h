#ifndef RBTREE_H
#define RBTREE_H
//#include "TNode.h"
#include <iostream>
using namespace std;

enum color_t{ RED, BLACK, DOUBLEBLACK};

template <class DT>
class TNode
{

	public:
		TNode* parent;
		TNode* left;
		TNode* right;
		DT data;
		color_t color;
		//public:
		TNode(DT* = new DT());
		TNode(DT);
		TNode(DT, color_t);
};
template <class DT>
class RBTree
{
	private:
		TNode<DT>* root;

		TNode<DT>* max(); 
		TNode<DT>* max(TNode<DT>*);

		TNode<DT>* min();
		TNode<DT>* min(TNode<DT>*);

		TNode<DT>* successor();
		TNode<DT>* successor(TNode<DT>*);

		TNode<DT>* predecessor();
		TNode<DT>* predecessor(TNode<DT>*);

		void transplant(TNode<DT>*, TNode<DT>*);
		void lRotate(TNode<DT>*);
		void rRotate(TNode<DT>*);

		TNode<DT>* createNode();

	public:
		RBTree();
		~RBTree();
		
		size_t numElements;
		size_t size();
		static TNode<DT>* nil;

		DT* rbtToArray(DT*, int, TNode<DT>*);

		void inOrder();
		void inOrder(TNode<DT>*);

		void preOrder();
		void preOrder(TNode<DT>*);

		void postOrder();
		void postOrder(TNode<DT>*);

		void rbtInsert();
		void rbtInsert(DT);
		void rbtInsert(TNode<DT>*);

		void rbtInsFixup(TNode<DT>*);
		void rbtDelFixup(TNode<DT>*);

		TNode<DT>* rbtSearch(DT data);
		TNode<DT>* rbtSearch(DT* dataPtr);
		void rbtDelete(DT);
		void rbtDelete(TNode<DT>*);

		void cleanNodes();
		void cleanNodes(TNode<DT>* x);
};
template <class DT>
TNode<DT>* RBTree<DT>::nil = new TNode<DT>(0, BLACK);

#endif
