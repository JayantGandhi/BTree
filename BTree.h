//BTree.h
//
//Author: Jayant Gandhi
//Date Created: 12/1/12
//Date Last Edited: 12/3/12

#ifndef BTREE_H
#define BTREE_H

#include "BTreeNode.h"
#include <iostream>
#include <iomanip>

using namespace std;

template <class T, int M>
class BTree
{
public:
	BTree();
		//default constructor for BTree
	~BTree();
		//destructor for BTree

	void insert(T& x);
		//inserts object x into the BTree at its proper place
		//begins from the root
	void remove (T& x);
		//removes object x from the tree
		//rebalances tree if necessary
	void print();
		//prints out tree in level-order
		//indents each level
	bool find(T& x);
		//returns true if x is in the tree
		//otherwise returns false

private:
	BTreeNode<T,M> *root;
		//pointer to the root node
	void print(BTreeNode<T,M> * & r, int numSpaces);
		//private member print function
		//sets indentation for level-order printing of the tree
		//recursively calls itself to print out the tree
	void insert(T& x, BTreeNode<T,M> * & r);
		//private member insert function
		//recursively calls to itself to find x's proper location
		//uses put function to place the actual value
	void split (T& x, BTreeNode<T,M> * & r, int index);
		//helper function for insert
		//handles overflow scenarios by splitting nodes
	void remove(T& x, BTreeNode<T,M> * & r);
		//private member remove function
		//recursively calls itself to remove x from tree
		//and ensure that B-Tree requirements are maintained
	void merge(BTreeNode<T,M> * & left, BTreeNode<T,M> * & right);
		//helper function for rebalance
		//handles all mergers
	void rebalance(BTreeNode<T,M> * & r);
		//helper function for remove
		//rebalances tree in the case of underflow
	bool contains (T& x, BTreeNode<T,M> * & r);
		//helper function for find
		//recursively calls itself until x is found or
		//until it reaches the end of the tree
	T findPivot (T& x, BTreeNode<T,M> * & r);
		//helper function for split
		//finds the pivot value to split around
};

//implementation
//private functions

template <class T, int M>
void BTree<T,M>::print(BTreeNode<T,M> * & r, int numSpaces)
{
	if(r != NULL)
	{
		// used to indent levels
		std::cout << setw(numSpaces) << "";
		// print out keys for current level
		for(int i = 0; i < r->totalKeys; i++)
			std::cout << setw(3) << r->keys[i] << "";
		std::cout << endl;
		// recursively print out a node, then its childeren
		// add an additional indentation each time we go one level deeper
		for(int i = 0; i <= r->totalKeys; i++)
			print(r->child[i], numSpaces + 8 );
	}
}

template <class T, int M>
void BTree<T,M>::insert (T& x, BTreeNode<T,M> * & r)
{
	if (r->leaf==true)
	{
		if (r->totalKeys<M-1) //not full so no danger of overflow
		{
			if (r->keys[0]==0) //nothing in the node
					r->keys[0]=x;//begin the node with x
			else
			{
				for (int i=0; i<r->totalKeys; i++)
				{
					if (x==r->keys[i]) //duplicate found
					{
						cout << "Duplicate found. Ignoring..." << endl;
						return;
					}
					else if (x<r->keys[i]) //found where x should go
					{
						for (int j=r->totalKeys; j>i; j--) //move all the other values over 1
							r->keys[j]=r->keys[j-1];
						r->keys[i]=x; //place x in its proper spot
						break;
					}
					else if (i+1==r->totalKeys) //reached the end of node and did not find higher value
					{
						r->keys[r->totalKeys]=x; //place x there, guaranteed to exist because no danger of overflow per previous condition
						break;
					}
				}
			}
			r->totalKeys++;
		}
		else //overflow, so split the node
		{
			for (int i=0; i<r->totalKeys; i++)
			{
				if (x==r->keys[i]) //duplicate found
				{
					cout << "Duplicate found. Ignoring..." << endl;
					return;
				}
				else if (x<r->keys[i]) //found where x should go
				{
					split(x,r, i); //split the node
					break;
				}
				else if (i+1==r->totalKeys) //reached the end of node and did not find higher value
				{
					split(x,r, M-1);; //split the node
					break;
				}
			}
		} //end overflow case 
	} //end leaf case
	else //internal node
	{
		for(int i=0; i<r->totalKeys; i++)
		{
			if (x==r->keys[i]) //duplicate found
			{
				cout << "Duplicate found. Ignoring..." << endl;
				return;
			}
			else if (x<r->keys[i]) //found where x should go
			{
				insert(x, r->child[i]); //move to left child at that index
				break;
			}
			else if (i+1==r->totalKeys) //reached the end of node and did not find higher value
			{
				insert(x, r->child[i+1]); //move to right most child
				break;
			}
		}
	}
}

template <class T, int M>
void BTree<T,M>::split (T& x, BTreeNode<T,M> * & r, int index)
{
	if (r->totalKeys<M-1) //no overflow and room to insert
	{
		for (int i=0; i<r->totalKeys; i++)
		{
			if (x==r->keys[i]) //duplicate found
			{
				cout << "Duplicate found. Ignoring..." << endl;
				return;
			}
			else if (x<r->keys[i]) //found where x should go
			{
				for (int j=r->totalKeys; j>i; j--) //move all the other values over 1
					r->keys[j]=r->keys[j-1];
				r->keys[i]=x; //place x in its proper spot
				break;
			}
			else if (i+1==r->totalKeys) //reached the end of node and did not find higher value
			{
				r->keys[r->totalKeys]=x; //place x there, guaranteed to exist because no danger of overflow per previous condition
				break;
			}
		}
		r->totalKeys++;
	}
	else //overflow is a problem
	{
		BTreeNode<T,M> *newNode = new BTreeNode<T,M>; //allocate memory for a new node
		int k=0;
		int w=M/2;
		T pivot = findPivot(x,r); //store pivot value
		for(int z=0; z<r->totalKeys; z++)
		{
			if (pivot==r->keys[z])
			{
				r->keys[z]=NULL; 
				w=z;
				break;
			}
		}
		for(int i=w+1;i<(M-1);i++) //move greater half to new node
		{
			newNode->keys[k]=r->keys[i];//move each individual unit over
			r->keys[i]=NULL; //remove data from old node
			r->totalKeys--;
			newNode->totalKeys++;
			k++;
		}
		r->totalKeys--; //account for pivot point leaving old node
		if (x<pivot) //x belongs in the lesser half
			insert(x,r);
		else //x belongs in the greater half
			insert(x,newNode);
		
		if(r->parent==NULL) //we are at the root, so grow the tree
		{
			BTreeNode<T,M> *newRoot = new BTreeNode<T,M>;
			newRoot->keys[0]=pivot; //places pivot in the new root as the only value
			newRoot->child[0]=r;
			r->parentPos=0;
			r->parent=newRoot;
			newRoot->child[1]=newNode;
			newNode->parentPos=1;
			newNode->parent=newRoot;
			newRoot->leaf=false;
			newRoot->parent=NULL;
			root=newRoot;
			newRoot->totalKeys=1;
			//move some children from r to newnode
			int g=0;
			for(int q=r->totalKeys+1; q<M; q++)
			{
				newNode->child[g]=r->child[q];
				r->child[q]=NULL;
				g++;
			}
			
		}
		
		else //we are not at the root
		{
			split (pivot, r->parent, index); //tries to insert pivot into parent node, splits if full
			newNode->parent=r->parent; //organize all the parent child relationships
			newNode->parentPos=r->parentPos+1;
			if (r->parent->child[newNode->parentPos]==NULL) //no child to the right
				r->parent->child[newNode->parentPos]=newNode;
			else
			{
				for (int q=r->parent->totalKeys; q>newNode->parentPos; q--) //adjust all the other children's relative position
				{
					r->parent->child[q]=r->parent->child[q-1];
					r->parent->child[q]->parentPos=q;
				}
				r->parent->child[newNode->parentPos]=newNode;
			}
		}
	}
}

template <class T, int M>
T BTree<T,M>::findPivot (T& x, BTreeNode<T,M> * & r)
{
	T allItems[M]; //array to hold all the values including the extra one
	for(int i=0; i<M-1; i++) //place values from node
		allItems[i]=r->keys[i];
	for(int j=0; j<M; j++) //find where x goes
	{
		if(x<allItems[j])
		{
			for(int k=M-1; k>j; k--)//move all the values over
				allItems[k]=allItems[k-1];
			allItems[j]=x;
			break;
		}
		else if(j+1==M-1)//reached the end without finding value greater than x
			allItems[M-1]=x;
	}
	return (allItems[(M/2)]);
}

template <class T, int M>
void BTree<T,M>::remove(T& x, BTreeNode<T,M> * & r)
{
	if (r->leaf==true) //node is a leaf
	{ 
		for(int i=0; i<r->totalKeys; i++) //find x
		{
			if (r->keys[i]==x) //found x
			{
				r->keys[i]=NULL; //remove x
				for(int j=i; j<r->totalKeys-1; j++) //move other values down
					r->keys[j]=r->keys[j+1];
				r->totalKeys--;
				rebalance(r); //rebalance the node
				break;
			}
		}
	}//end of leaf case
	else //not at a leaf so might have to decend
	{
		for(int i=0; i<r->totalKeys; i++) //find x
		{
			if (x==r->keys[i])//found it!
			{
				if(r->child[i]) //you can take from left child
				{
					r->keys[i]=r->child[i]->keys[r->child[i]->totalKeys-1]; //replace x with largest value in left subtree
					remove (r->child[i]->keys[r->child[i]->totalKeys-1], r->child[i]); //now remove that value from left subtree
				}
				else if (r->child[i+1]) //you can take from the right child
				{
					r->keys[i]=r->child[i+1]->keys[r->child[i+1]->totalKeys-1]; //replace x with smallest value in the right child
					remove (r->child[i+1]->keys[r->child[i+1]->totalKeys-1], r->child[i+1]); //now remove that value from right child
				}
				break;
			}
			else if (x<r->keys[i]) //less than a key, so go to child
				remove(x, r->child[i]);
			else if (i+1==r->totalKeys) //reached the last value without finding x or key greater than it so go to right child
				remove(x, r->child[i+1]);
		}
	}
}

template <class T, int M>
void BTree<T,M>::merge(BTreeNode<T,M> * & left, BTreeNode<T,M> * & right)
{
	left->keys[left->totalKeys]=left->parent->keys[left->parentPos];//move parent key down
	left->totalKeys++;
	int j=0;
	for(int i=left->totalKeys; i<M-1; i++) //move values in right over to left
	{
		left->keys[i]=right->keys[j];
		j++;
	}
	left->totalKeys+=right->totalKeys; //account for added values
	right=NULL;//remove right
}

template <class T, int M>
void BTree<T,M>::rebalance(BTreeNode<T,M> * & r)
{
	if (r->parent!=root)//we are not in the root
	{
		if(r->totalKeys>=(M-1)/2) //no underflow occured from deletion
			return; //no need to continue
		else //underflow occured
		{
			//the borrows:
			if(r->parent->child[r->parentPos+1]&&r->parent->child[r->parentPos+1]->totalKeys>M-1/2) //There exists a right sibling with enough values
			{
				r->keys[r->totalKeys]=r->parent->keys[r->parentPos]; //move key value from parent down to next available spot
				r->parent->keys[r->parentPos]=r->parent->child[r->parentPos+1]->keys[0]; //moves the first value of the right sibling to parent
				remove(r->parent->child[r->parentPos+1]->keys[0], r->parent->child[r->parentPos+1]); //removes the duplicate value in the child
				r->parent->child[r->parentPos+1]->totalKeys--;
			} //borrow from right complete
			else if(r->parent->child[r->parentPos-1]&&r->parent->child[r->parentPos-1]->totalKeys>M-1/2) //There exists a left sibling with enough values
			{
				for(int k=r->totalKeys; k>0; k--) //open up the first spot
					r->keys[k]=r->keys[k-1];
				r->keys[0]=r->parent->keys[r->parentPos-1]; //move key value from parent down
				r->parent->keys[r->parentPos-1]=r->parent->child[r->parentPos-1]->keys[r->parent->child[r->parentPos-1]->totalKeys-1]; //moves last value in left to parent
				remove(r->parent->child[r->parentPos-1]->keys[r->parent->child[r->parentPos-1]->totalKeys-1], r->parent->child[r->parentPos-1]); //removes the duplicate value in the child
				r->parent->child[r->parentPos+1]->totalKeys--;
			} //borrow from left complete
			//the mergers:
			if (r->parent->child[r->parentPos+1]) //there is a sibling to the right
			{
				merge(r,r->parent->child[r->parentPos+1]);//merge the two
				rebalance(r->parent);//ensure no underflow in parent as a result of the merger
			}
			else if(r->parent->child[r->parentPos-1]) //there is a left sibling
				{
					merge(r->parent->child[r->parentPos-1],r);//merge the two
					rebalance(r->parent);//ensure no underflow in parent as a result of the merger
				}
			else //no siblings, parent is the root
			{
				if (r->parent->keys[r->parentPos]) //it is the left child
				{
					r->keys[r->totalKeys]=r->parent->keys[r->parentPos];
					r->totalKeys++;
					rebalance(r->parent); //call rebalance on root to collapse it
				}
				else //must be the right child
				{
					for (int j=r->totalKeys; j>0; j--) //move all values over 1
						r->keys[j]=r->keys[j-1];
					r->keys[0]=r->parent->keys[r->parentPos-1]; //move parent value down
					rebalance(r->parent); //call rebalance on root to collapse it
				}
			}
		}//underflow taken care of
	} //non-root case end
	else if(r->parent==NULL) //we are at the root, may have to collapse it
		{
			if(r->totalKeys==0)
			{
					root=r->child[0]; //that becomes the new root
					r=NULL; //remove old root
			}
			else //root is a special case, it can have less than M-1 keys, so return
				return;
			}
}

template <class T, int M>
bool BTree<T,M>::contains (T& x, BTreeNode<T,M> * & r)
{
	bool found=false;
	if(r->leaf==true)
	{
		for (int i=0; i<r->totalKeys; i++)
		{
			if (x==r->keys[i])//found it!
			{
			found=true;
			break;
			}
			else //not in tree
				found=false;
		}
	}
	else
	{
		for (int i=0; i<r->totalKeys; i++)
		{
			if (found==true)
				break;
			else if (x==r->keys[i])//found it!
			{
				found=true;
				break;
			}
			else if (x<r->keys[i]) //less than a key, so go to child
			{
				found=contains (x, r->child[i]);
				break;
			}
			else if (i+1==r->totalKeys) //reached the last value without finding x or key greater than it so go to right child
			{
				found=contains (x, r->child[i+1]);
				break;
			}
		}
	}
	return found;
}

//public functions
template <class T, int M>
BTree<T,M>::BTree()
{
	root= new BTreeNode<T,M>;
	root->leaf=true;
	//root->parent=NULL;
}

template <class T, int M>
BTree<T,M>::~BTree()
{
	delete root;
}

template <class T, int M>
void BTree<T,M>::insert(T& x)
{
	insert(x,root);
}

template <class T, int M>
void BTree<T,M>::remove(T& x)
{
	if(find(x)==false)
	{
		cout<<"No such value in the tree."<<endl;
		return;
	}
	else //value does exist so continue with removal process
		remove (x, root);
}

template <class T, int M>
void BTree<T,M>::print()
{
	print(root,2);
}

template <class T, int M>
bool BTree<T,M>::find(T& x)
{
	return contains(x,root);
}

#endif