#pragma once

template <typename K, typename V>
Dictionary<K, V>::Dictionary() : _size(0)
{
	
}

template <typename K, typename V>
void Dictionary<K, V>::put(const K& key, const V& value)
{
	if (!contains(key))
	{
		_size++;
	}
	
    tree.insert(key, value);
}

template <typename K, typename V>
void Dictionary<K, V>::remove(const K& key)
{
    if (contains(key))
    {
        tree.remove(key);
        _size--;
    }
}

template <typename K, typename V>
bool Dictionary<K, V>::contains(const K& key)
{
    return tree.search(key) != nullptr;
}

template <typename K, typename V>
const V& Dictionary<K, V>::operator[](const K& key) const
{
    V* elementPtr = tree.search(key);
    if (elementPtr != nullptr)
    {
        return *elementPtr;
    }

    return getDefaultValue();
}

template <typename K, typename V>
V& Dictionary<K, V>::operator[](const K& key)
{
    V* elementPtr = tree.search(key);
    if (elementPtr != nullptr)
    {
        return *elementPtr;
    }

    V& defaultValue = getDefaultValue();
    tree.insert(key, defaultValue);
    return defaultValue;
}

template <typename K, typename V>
int Dictionary<K, V>::size() const
{
    return _size;
}

template <typename K, typename V>
Dictionary<K, V>::Iterator::Iterator(Tree<K, V>& treeRef) : treeRef(treeRef)
{
    currentNode = treeRef.root;
    rootNode = treeRef.root;

    lastNode = currentNode;

    while (lastNode->right != nullptr)
    {
        lastNode = lastNode->right;
    }

    if (lastNode->left != nullptr)
    {
        lastNode = lastNode->left;
    }

    goDown = true;
}

template <typename K, typename V>
const K& Dictionary<K, V>::Iterator::key() const
{
    return currentNode->key;
}

template <typename K, typename V>
const V& Dictionary<K, V>::Iterator::get() const
{
    return currentNode->val;
}

template <typename K, typename V>
void Dictionary<K, V>::Iterator::set(const V& value)
{
    currentNode->val = value;
}

template <typename K, typename V>
void Dictionary<K, V>::Iterator::next()
{
    if (goDown)
    {
        if (currentNode->left != nullptr)
        {
            currentNode = currentNode->left;
        }
        else
        {
            goDown = false;
        }
    }

    if (!goDown)
    {
    	// Move up in the tree and get next right node
        while (true)
        {
            if (currentNode == rootNode)
            {
                break;
            }
        	
            typename Tree<K, V>::Node* prevNode = currentNode;
            currentNode = currentNode->parent;
            if (currentNode->right != nullptr && currentNode->right != prevNode)
            {
            	// Check if right node exists and it's not a previous iteration node
                break;
            }
        }

        if (currentNode == rootNode)
        {
            rootNode = currentNode->right;
        }
        currentNode = currentNode->right;

        goDown = true;
    }
}

template <typename K, typename V>
void Dictionary<K, V>::Iterator::prev()
{
    if (currentNode == currentNode->parent->left)
    {
        currentNode = currentNode->parent;
    }
    else
    {
        if (currentNode->parent->left == nullptr)
        {
            currentNode = currentNode->parent;
        }
        else
        {
            currentNode = currentNode->parent->left;
        	// Go to the right end of subtree
            while (currentNode->right != nullptr)
            {
                currentNode = currentNode->right;
            }
        }
    }
}

template <typename K, typename V>
bool Dictionary<K, V>::Iterator::hasNext() const
{
    return currentNode != lastNode;
}

template <typename K, typename V>
bool Dictionary<K, V>::Iterator::hasPrev() const
{
    return currentNode != treeRef.root;
}

template <typename K, typename V>
typename Dictionary<K, V>::Iterator Dictionary<K, V>::iterator()
{
    return *(new Iterator(this->tree));
}

template <typename K, typename V>
const typename Dictionary<K, V>::Iterator Dictionary<K, V>::iterator() const
{
    return *(new Iterator(this->tree));
}

template <typename K, typename V>
V& Dictionary<K, V>::getDefaultValue()
{
	return *new V();
}
