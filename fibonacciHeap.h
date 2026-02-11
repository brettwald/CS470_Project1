//interfaces to priorityQueue.h

#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H

#include "priorityQueue.h"
#include <stdexcept>
#include <vector>
#include <cmath>

template<typename K, typename V>
struct FibNode : public Node<K,V> {
    FibNode* parent;
    FibNode* child;
    FibNode* left;
    FibNode* right;
    int degree;
    bool marked;

    FibNode(K k, V v) {
        this->key = k;
        this->value = v;
        parent = nullptr;
        child = nullptr;
        left = this;
        right = this;
        degree = 0;
        marked = false;
    }
};

template<typename K, typename V>
class FibonacciHeap : public PriorityQueue<K,V> {
private:
    FibNode<K,V>* minNode;
    int nodeCount;

    void insertIntoList(FibNode<K,V>* listNode, FibNode<K,V>* node) {
        node->left = listNode;
        node->right = listNode->right;
        listNode->right->left = node;
        listNode->right = node;
    }

    void removeFromList(FibNode<K,V>* node) {
        node->left->right = node->right;
        node->right->left = node->left;
    }

    //tree y becomes child of tree x
    void link(FibNode<K,V>* y, FibNode<K,V>* x) {
        removeFromList(y);

        y->parent = x;
        y->marked = false;

        if (x->child == nullptr) {
            x->child = y;
            y->left = y;
            y->right = y;
        } else {
            insertIntoList(x->child, y);
        }

        x->degree++;
    }

    //merge trees of same degree after extract_min
    void consolidate() {
        int maxDegree = static_cast<int>(log2(nodeCount)) + 2; //formula for maximum possible degree of tree
        vector<FibNode<K,V>*> A(maxDegree, nullptr);

        vector<FibNode<K,V>*> roots;
        FibNode<K,V>* curr = minNode;
        if (curr != nullptr) {
            do {
                roots.push_back(curr);
                curr = curr->right;
            } while (curr != minNode);
        }

        for (FibNode<K,V>* w : roots) {
            FibNode<K,V>* x = w;
            int d = x->degree;

            while (d < maxDegree && A[d] != nullptr) {
                FibNode<K,V>* y = A[d];

                if (y->key < x->key) {
                    FibNode<K,V>* temp = x;
                    x = y;
                    y = temp;
                }

                link(y, x);
                A[d] = nullptr;
                d++;
            }

            if (d < maxDegree) {
                A[d] = x;
            }
        }

        minNode = nullptr;
        for (int i = 0; i < maxDegree; i++) {
            if (A[i] != nullptr) {
                A[i]->left = A[i];
                A[i]->right = A[i];
                A[i]->parent = nullptr;

                if (minNode == nullptr) {
                    minNode = A[i];
                } else {
                    insertIntoList(minNode, A[i]);
                    if (A[i]->key < minNode->key) {
                        minNode = A[i];
                    }
                }
            }
        }
    }

    //remove child x from parent y
    void cut(FibNode<K,V>* x, FibNode<K,V>* y) {
        if (x->right == x) {
            y->child = nullptr;
        } else {
            if (y->child == x) {
                y->child = x->right;
            }
            removeFromList(x);
        }
        y->degree--;

        x->parent = nullptr;
        x->marked = false;
        x->left = x;
        x->right = x;
        insertIntoList(minNode, x);
    }

    //cuts marked nodes
    //node is cut if it loses first child
    void cascadingCut(FibNode<K,V>* y) {
        FibNode<K,V>* z = y->parent;
        if (z != nullptr) {
            if (!y->marked) {
                y->marked = true;
            } else {
                cut(y, z);
                cascadingCut(z);
            }
        }
    }

    void deleteAll(FibNode<K,V>* start) {
        if (start == nullptr) return;

        FibNode<K,V>* curr = start;
        do {
            FibNode<K,V>* next = curr->right;
            deleteAll(curr->child);
            delete curr;
            curr = next;
        } while (curr != start);
    }

public:
    FibonacciHeap() : minNode(nullptr), nodeCount(0) {}

    ~FibonacciHeap() override {
        if (minNode != nullptr) {
            deleteAll(minNode);
        }
    }

    bool is_empty() override {
        return minNode == nullptr;
    }

    Node<K,V>* insert(K key, V value) override {
        this->insertCount++;
        FibNode<K,V>* node = new FibNode<K,V>(key, value);

        if (minNode == nullptr) {
            minNode = node;
        } else {
            insertIntoList(minNode, node);
            if (node->key < minNode->key) {
                minNode = node;
            }
        }

        nodeCount++;
        return node;
    }

    pair<K,V> find_min() override {
        if (minNode == nullptr) {
            throw runtime_error("Heap is empty");
        }
        return {minNode->key, minNode->value};
    }

    pair<K,V> extract_min() override {
        this->extractCount++;
        if (minNode == nullptr) {
            throw runtime_error("Heap is empty");
        }

        FibNode<K,V>* z = minNode;
        pair<K,V> result = {z->key, z->value};

        if (z->child != nullptr) {
            FibNode<K,V>* child = z->child;
            do {
                FibNode<K,V>* next = child->right;
                child->left = child;
                child->right = child;
                child->parent = nullptr;
                insertIntoList(minNode, child);
                child = next;
            } while (child != z->child);
        }

        if (z == z->right) {
            minNode = nullptr;
        } else {
            minNode = z->right;
            removeFromList(z);
            consolidate();
        }

        nodeCount--;
        delete z;
        return result;
    }

    void decrease_key(Node<K,V>* node, K new_key) override {
        this->decreaseKeyCount++;
        FibNode<K,V>* x = static_cast<FibNode<K,V>*>(node);

        if (new_key > x->key) {
            throw runtime_error("New key is greater than current key");
        }

        x->key = new_key;
        FibNode<K,V>* y = x->parent;

        if (y != nullptr && x->key < y->key) {
            cut(x, y);
            cascadingCut(y);
        }

        if (x->key < minNode->key) {
            minNode = x;
        }
    }
};

#endif
