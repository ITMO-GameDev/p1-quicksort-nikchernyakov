#pragma once

/*
 * Left-leaning red-black tree implementation
 * https://www.cs.princeton.edu/~rs/talks/LLRB/LLRB.pdf
 */

template <typename K, typename V>
Tree<K, V>::Node::Node(const K& key, const V& val) : key(key), val(val), color(RED)
{
	
}

template <typename K, typename V>
V* Tree<K, V>::search(const K& key)
{
	Node* node = root;

	while (node != nullptr)
	{
		if (key == node->key)
		{
			return &(node->val);
		}
		else if (key < node->key)
		{
			node = node->left;
		}
		else if (key > node->key)
		{
			node = node->right;
		}
	}

	return nullptr;
}

template <typename K, typename V>
void Tree<K, V>::insert(const K& key, const V& value)
{
	root = insert(root, key, value);
	root->color = BLACK;
}

template <typename K, typename V>
typename Tree<K, V>::Node* Tree<K, V>::insert(Node* node, const K& key, const V& value)
{
	if (node == nullptr)
	{
		return new Node(key, value);
	}

	if (key == node->key)
	{
		node->val = value;
	}
	else if (key < node->key)
	{
		Node* newNode = insert(node->left, key, value);

		newNode->parent = node;
		node->left = newNode;
	}
	else
	{
		Node* newNode = insert(node->right, key, value);

		newNode->parent = node;
		node->right = newNode;
	}

	node = fixUp(node);

	return node;
}

template <typename K, typename V>
void Tree<K, V>::remove(const K& key)
{
	root = remove(root, key);

	if (root != nullptr)
	{
		root->color = BLACK;
	}
}

template <typename K, typename V>
typename Tree<K, V>::Node* Tree<K, V>::remove(Node* node, const K& key)
{
	if (key < node->key)
	{
		if (!isRed(node->left) && !isRed(node->left->left))
		{
			node = moveRedLeft(node);
		}

		node->left = remove(node->left, key);
	}
	else
	{
		if (isRed(node->left))
		{
			node = rotateRight(node);
		}

		if (key == node->key && node->right == nullptr)
		{
			return nullptr;
		}

		if (!isRed(node->right) && !isRed(node->right->left))
		{
			node = moveRedRight(node);
		}

		if (key == node->key)
		{
			Node* mn = minNode(node->right);

			node->val = mn->val;
			node->key = mn->key;
			node->right = deleteMin(node->right);
		}
		else
		{
			node->right = remove(node->right, key);
		}
	}

	return fixUp(node);
}

template <typename K, typename V>
typename Tree<K, V>::Node* Tree<K, V>::rotateLeft(Node* node)
{
	Node* x = node->right;

	node->right = x->left;
	x->left = node;

	if (node->right != nullptr)
	{
		node->right->parent = node;
	}
	x->parent = node->parent;
	node->parent = x;

	x->color = node->color;
	node->color = RED;

	return x;
}

template <typename K, typename V>
typename Tree<K, V>::Node* Tree<K, V>::rotateRight(Node* node)
{
	Node* x = node->left;

	node->left = x->right;
	x->right = node;

	if (node->left != nullptr)
	{
		node->left->parent = node;
	}
	x->parent = node->parent;
	node->parent = x;

	x->color = node->color;
	node->color = RED;

	return x;
}

template <typename K, typename V>
typename Tree<K, V>::Node* Tree<K, V>::moveRedLeft(Node* node)
{
	flipColors(node);

	if (isRed(node->right->left))
	{
		node->right = rotateRight(node->right);
		node = rotateLeft(node);

		flipColors(node);
	}

	return node;
}

template <typename K, typename V>
typename Tree<K, V>::Node* Tree<K, V>::moveRedRight(Node* node)
{
	flipColors(node);

	if (isRed(node->left->left))
	{
		node = rotateRight(node);

		flipColors(node);
	}

	return node;
}

template <typename K, typename V>
void Tree<K, V>::flipColors(Node* node)
{
	node->color = !node->color;
	node->left->color = !node->left->color;
	node->right->color = !node->right->color;
}

template <typename K, typename V>
bool Tree<K, V>::isRed(Node* node)
{
	if (node == nullptr)
	{
		return false;
	}

	return node->color == RED;
}

template <typename K, typename V>
typename Tree<K, V>::Node* Tree<K, V>::fixUp(Node* node)
{
	if (isRed(node->right) && !isRed(node->left))
	{
		node = rotateLeft(node);
	}

	if (isRed(node->left) && isRed(node->left->left))
	{
		node = rotateRight(node);
	}

	if (isRed(node->left) && isRed(node->right))
	{
		flipColors(node);
	}

	return node;
}

template <typename K, typename V>
typename Tree<K, V>::Node* Tree<K, V>::deleteMin(Node* node)
{
	if (node->left == nullptr)
	{
		return nullptr;
	}

	if (!isRed(node->left) && !isRed(node->left->left))
	{
		node = moveRedLeft(node);
	}

	node->left = deleteMin(node->left);

	return fixUp(node);
}

template <typename K, typename V>
typename Tree<K, V>::Node* Tree<K, V>::minNode(Node* node)
{
	return (node->left == nullptr) ? node : minNode(node->left);
}
