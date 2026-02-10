#ifndef PAIRING_HEAP_H
#define PAIRING_HEAP_H

#include "priorityQueue.h"
#include <stdexcept>
#include <vector>

using namespace std;

template<typename K, typename V>
struct PairingNode : public Node<K,V> {
    PairingNode* child;
    PairingNode* sibling;
    PairingNode* parent;
    
    PairingNode(K k, V v) {
        this->key = k;
        this->value = v;
        child = nullptr;
        sibling = nullptr;
        parent = nullptr;
    }
};

template<typename K, typename V>
class PairingHeap : public PriorityQueue<K,V> {
private:
    PairingNode<K,V>* root;
    int nodeCount;
    
    PairingNode<K,V>* meld(PairingNode<K,V>* h1, PairingNode<K,V>* h2) {
        if (h1 == nullptr) return h2;
        if (h2 == nullptr) return h1;
        
        if (h1->key <= h2->key) {
            h2->sibling = h1->child;
            if (h1->child != nullptr) {
                h1->child->parent = nullptr;
            }
            h1->child = h2;
            h2->parent = h1;
            return h1;
        } else {
            h1->sibling = h2->child;
            if (h2->child != nullptr) {
                h2->child->parent = nullptr;
            }
            h2->child = h1;
            h1->parent = h2;
            return h2;
        }
    }
    
    PairingNode<K,V>* mergePairs(PairingNode<K,V>* node) {
        if (node == nullptr) return nullptr;
        
        vector<PairingNode<K,V>*> siblings;
        PairingNode<K,V>* curr = node;
        
        while (curr != nullptr) {
            PairingNode<K,V>* next = curr->sibling;
            curr->sibling = nullptr;
            curr->parent = nullptr;
            siblings.push_back(curr);
            curr = next;
        }
        
        if (siblings.empty()) return nullptr;
        if (siblings.size() == 1) return siblings[0];
        
        vector<PairingNode<K,V>*> merged;
        for (size_t i = 0; i < siblings.size(); i += 2) {
            if (i + 1 < siblings.size()) {
                merged.push_back(meld(siblings[i], siblings[i + 1]));
            } else {
                merged.push_back(siblings[i]);
            }
        }
        
        PairingNode<K,V>* result = merged[merged.size() - 1];
        for (int i = merged.size() - 2; i >= 0; i--) {
            result = meld(merged[i], result);
        }
        
        return result;
    }
    
    void cut(PairingNode<K,V>* node) {
        if (node->parent == nullptr) return;
        
        PairingNode<K,V>* parent = node->parent;
        
        if (parent->child == node) {
            parent->child = node->sibling;
        } else {
            PairingNode<K,V>* prev = parent->child;
            while (prev->sibling != node) {
                prev = prev->sibling;
            }
            prev->sibling = node->sibling;
        }
        
        node->parent = nullptr;
        node->sibling = nullptr;
    }
    
    void deleteAll(PairingNode<K,V>* node) {
        if (node == nullptr) return;
        
        PairingNode<K,V>* child = node->child;
        while (child != nullptr) {
            PairingNode<K,V>* next = child->sibling;
            deleteAll(child);
            child = next;
        }
        
        delete node;
    }
    
public:
    PairingHeap() : root(nullptr), nodeCount(0) {}
    
    ~PairingHeap() override {
        deleteAll(root);
    }
    
    bool is_empty() override {
        return root == nullptr;
    }
    
    Node<K,V>* insert(K key, V value) override {
        PairingNode<K,V>* node = new PairingNode<K,V>(key, value);
        
        if (root == nullptr) {
            root = node;
        } else {
            root = meld(root, node);
        }
        
        nodeCount++;
        return node;
    }
    
    pair<K,V> find_min() override {
        if (root == nullptr) {
            throw runtime_error("Heap is empty");
        }
        return {root->key, root->value};
    }
    
    pair<K,V> extract_min() override {
        if (root == nullptr) {
            throw runtime_error("Heap is empty");
        }
        
        PairingNode<K,V>* oldRoot = root;
        pair<K,V> result = {oldRoot->key, oldRoot->value};
        
        root = mergePairs(oldRoot->child);
        
        nodeCount--;
        delete oldRoot;
        return result;
    }
    
    void decrease_key(Node<K,V>* node, K new_key) override {
        PairingNode<K,V>* pNode = static_cast<PairingNode<K,V>*>(node);
        
        if (new_key > pNode->key) {
            throw runtime_error("New key is greater than current key");
        }
        
        pNode->key = new_key;
        
        if (pNode != root) {
            cut(pNode);
            root = meld(root, pNode);
        }
    }
};

#endif