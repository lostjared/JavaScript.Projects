#ifndef __MX__TREE_H_
#define __MX__TREE_H_

/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
	and other api's to provide a set of classes and functions
	to produce multi media applications, using object oriented
	techniques.

 	(C) 2008 LostSideDead


	This source file includes, the code for a tree.
	with right and left leaves, to sort the items
	The Key type must support a overloaded operator >
	and operator =

	also contains a few overloaded hashing functions
	if you create a mxList with a key of type integer
	you can use the tree as a hash table, using each
	index as a tree or bucket for duplicate hashes.

 	(C) 2008 LostSideDead

*/

#include<iostream>
#include<cstdlib>

namespace mx
{

	template<typename Key, typename Value>
	class mxNode {
	public:
		mxNode() { left = right = 0; }
		mxNode( Key k, Value v )
		{
			key = k;
			value = v;
			left = right = 0;
		}

		~mxNode()
		{
			if(left) delete left;
			if(right) delete right;
			left = 0;
			right = 0;
		}

		Value value;
		Key   key;
		mxNode *left, *right;
	};


	template<typename Key, typename Value>
	class mxList {

	public:

		mxList() : head(0) { } // default constructor


		mxList( mxList<Key, Value> &lst ); // copy constructor

		~mxList();

		mxNode<Key,Value> *addItem( Key k, Value val ); // addItem to list
		Value &operator[]( Key k ); // overloaded [] operator for random access if not found then insert
		Value *operator()( Key k ); // overloaded () operator for random access if not found return 0

		mxList<Key,Value> &operator=( mxList<Key,Value> &lst );
		mxList<Key,Value> &operator+=( mxList<Key,Value> &lst ) { concatList(lst); return *this; } // operator += concat list
		void concatList( mxList<Key,Value> &lst ); // concat List of Same Type
		void clear(); // clear Items
		void removeItem( Key k );

		void printNode( mxNode<Key,Value> *n); // print nodes to stdout from any point
		void printNode();


		mxNode<Key,Value> *root() const { return head; } // root node
		mxNode<Key,Value> *find( Key k ); // find by key


	protected:
		mxNode<Key,Value> *head;
		void catNode( mxNode<Key,Value> *n );
		void nocatNode( mxNode<Key,Value> *n, Key k );

	};


	template<typename Key, typename Value>
	mxList<Key,Value>::mxList( mxList<Key, Value> &lst ) : head(0)
	{
		clear();
		concatList( lst );

	}

	template<typename Key, typename Value>
	mxNode<Key,Value> *mxList<Key,Value>::addItem( Key k, Value value )
	{
		if(head == 0)
		{
			head = new mxNode<Key,Value>(k,value);
			return head;

		}

		mxNode<Key,Value> **ptr = &head, *p;

		while ( ( p = *ptr ) != 0 )
		{
			if(p->key == k) return p;
			if( (p->key > k) == 0 ) ptr = &p->left; else ptr = &p->right;
		}

		p = new mxNode<Key,Value>(k,value);
		*ptr = p;

		return p;
	}

	template<typename Key, typename Value>
	mxNode<Key,Value> *mxList<Key,Value>::find( Key k )
	{

		if(head == 0) return 0;

		mxNode<Key,Value> **ptr = &head, *p;

		while( ( p = *ptr ) != 0 )
		{

			if(p->key == k) return p;
			if((p->key > k) == 0) ptr = &p->left; else ptr = &p->right;
		}
		return 0;
	}

	template<typename Key, typename Value>
	mxList<Key, Value>::~mxList()
	{

		clear();

	}

	template<typename Key, typename Value>
	void mxList<Key,Value>::clear()
	{

		if(head)
		delete head;
		head = 0;

	}

	template<typename Key, typename Value>
	void mxList<Key,Value>::printNode( mxNode<Key,Value> *n)
	{


		if(n->left) printNode(n->left);
		if(n->right) printNode(n->right);

		std::cout << "Node: " << n->key << " : "  << n->value << "\n";
	}

	template<typename Key, typename Value>
	void mxList<Key,Value>::printNode()
	{

		if(root())
		printNode(root());

	}

	template<typename Key, typename Value>
	Value  &mxList<Key,Value>::operator[]( Key k )
	{

		mxNode<Key,Value> *node_ptr;

		node_ptr = this->find(k);

		Value v;
		if(node_ptr == 0) return addItem(k, v)->value;

		return node_ptr->value;
	}

	template<typename Key, typename Value>
	Value *mxList<Key,Value>::operator()( Key k )
	{
		mxNode<Key,Value> *node_ptr;
		node_ptr = this->find(k);
		if(node_ptr == 0) return 0;
		return &node_ptr->value;
	}

	template<typename Key, typename Value>
	void mxList<Key,Value>::concatList( mxList<Key,Value> &lst )
	{
		if(lst.root())
		catNode( lst.root() );
	}

	template<typename Key, typename Value>
	void mxList<Key,Value>::catNode( mxNode<Key,Value> *n )
	{

		if(n->left)  catNode(n->left);
		if(n->right) catNode(n->right);

		addItem(n->key, n->value);

	}

	template<typename Key, typename Value>
	void mxList<Key,Value>::nocatNode( mxNode<Key,Value> *n, Key k )
	{

		if(n->left) nocatNode(n->left, k);
		if(n->right) nocatNode(n->right, k);

		if(n->key != k)
		addItem(n->key, n->value);
	}

	template<typename Key, typename Value>
	void mxList<Key,Value>::removeItem ( Key k )
	{
		mxList<Key,Value> lst(*this);
		clear();

		if(lst.root())
		nocatNode( lst.root(), k);
	}


	template<typename Key, typename Value>
	mxList<Key,Value> &mxList<Key,Value>::operator=( mxList<Key,Value> &lst )
	{
		clear();
		concatList( lst );
		return *this;
	}


	// hashing function for Trees indexed by integers
	// tree[HashKey<std::string,unsigned int, 100>(str)] = value;
	template<typename Type, typename Accum, size_t HASH_SIZE>
	Accum  HashKey(Type t)
	{
		size_t i = 0;
		Accum h = Accum(), g = Accum();

		for(i = 0; t[i] != 0; i++)
		{
			h = ( h << 4 ) + ( t[i] );
			if( (g = h&0xF0000000) )
			{
				h = h ^ (g >> 24);
				h = h ^ g;
			}
		}

		return (h%HASH_SIZE);
	}

	// Another implementation of a hashing function ( taken from "Compilers Principals Techniques and Tools" aka the Dragon Book )
	template<size_t HASH_SIZE, typename InputIterator, typename Return>
	Return HashKey(InputIterator begin, InputIterator end)
	{
		Return h = Return(), g = Return();
		InputIterator i;
		for(i = begin; i != end; i++)
		{
			h = ( h << 4 ) + ( *i );
			if( (g = h&0xF0000000) )
			{
				h = h ^ (g >> 24);
				h = h ^ g;
			}
		}
		return (h%HASH_SIZE);
	}

	// overloaded non-template implementation of HashKey
	template<typename String>
	unsigned int HashKey(String key, unsigned int size)
	{

		unsigned int i = 0, h = 0, g = 0;
		for(i = 0;  key[i] != 0; i++)
		{
			h = ( h << 4 ) + ( key[i] );
			if( (g = h&0xF0000000) )
			{
				h = h ^ (g >> 24);
				h = h ^ g;
			}
		}
		return (h%size);
	}



}




#endif

