#pragma once

/*
 * Left-leaning red-black tree interface
 * https://www.cs.princeton.edu/~rs/talks/LLRB/LLRB.pdf
 */

template <typename K, typename V>
class Tree final
{
	template <typename, typename>
	friend class Dictionary;
	
public:

	V* search(const K& key);
	void insert(const K& key, const V& value);
	void remove(const K& key);

private:

	const static bool RED = true;
	const static bool BLACK = false;

	class Node
	{
	public:

		Node(const K& key, const V& val);

		K key;
		V val;

		Node* parent = nullptr;
		Node* left = nullptr;
		Node* right = nullptr;

		bool color;

	};

	Node* root = nullptr;

	Node* insert(Node* node, const K& key, const V& value);
	Node* remove(Node* node, const K& key);
	
	Node* rotateLeft(Node* node);
	Node* rotateRight(Node* node);
	
	Node* moveRedLeft(Node* node);
	Node* moveRedRight(Node* node);

	void flipColors(Node* node);
	bool isRed(Node* node);
	Node* fixUp(Node* node);
	Node* deleteMin(Node* node);
	Node* minNode(Node* node);
};

#include "tree.hpp"
