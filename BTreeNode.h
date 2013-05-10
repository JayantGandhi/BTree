//BTreeNode.h
//
//Author: Jayant Gandhi
//Date Created: 12/1/12
//Date Last Edited: 12/3/12

#ifndef BTREENODE_H
#define BTREENODE_H

#include <cstdlib>
#include <cstring>
#include <cstddef>

template <class T, int M>
struct BTreeNode
{
	int totalKeys;
	int parentPos;
	T keys[M-1];
	BTreeNode<T,M> *child[M];
	BTreeNode<T,M> *parent;
	bool leaf;

	BTreeNode();
	BTreeNode(const BTreeNode<T,M> * & aNode);
	~BTreeNode();
	const BTreeNode<T,M>& operator=(const BTreeNode<T,M> * & rhs);
};

template <class T, int M>
BTreeNode<T,M>::BTreeNode()
{
	for (int i=0; i<(M-1); i++)
		keys[i]=0;
	for (int j=0; j<M; j++)
		child[j]=NULL;
	leaf=true;
	totalKeys=0;
	parentPos=NULL;
	parent=NULL;
}

template <class T, int M>
BTreeNode<T,M>::~BTreeNode()
{
}

template <class T, int M>
const BTreeNode<T,M>& BTreeNode<T,M>::operator=(const BTreeNode<T,M> * & rhs)
{
	totalKeys=rhs->totalKeys;
	parentPos=rhs->parentPos;
	parent=rhs->parent;
	leaf=rhs->leaf;
	for (int i=0; i<(M-1); i++)
		keys[i]=rhs->keys[i];
	for (int j=0; j<M; j++)
		child[j]=rhs->child[j];
	return *this;
}

#endif