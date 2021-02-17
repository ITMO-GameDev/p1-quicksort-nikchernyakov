#pragma once

#include "tree.h"


template <typename K, typename V>
class Dictionary final
{
public:

	// Constructor
    Dictionary();

	int size() const;

	// Data manipulation
    void put(const K& key, const V& value);
    void remove(const K& key);
    bool contains(const K& key);

	// Override operators
    const V& operator[](const K& key) const;
    V& operator[](const K& key);

	// Iterator implementation
    class Iterator
    {
    public:

        Iterator(Tree<K, V>& treeRef);

        const K& key() const;
        const V& get() const;
        void set(const V& value);
        void next();
        void prev();
        bool hasNext() const;
        bool hasPrev() const;

    private:

        Tree<K, V>& treeRef;
        typename Tree<K, V>::Node* currentNode;
        typename Tree<K, V>::Node* rootNode;
        typename Tree<K, V>::Node* lastNode;

        bool goDown;
    };

    Iterator iterator();
    const Iterator iterator() const;

private:

    Tree<K, V> tree;
    int _size;

	V& getDefaultValue();
};

#include "dictionary.hpp"