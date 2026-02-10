//interfaces to priorityQueue.h

#ifndef PAIRING_HEAP_H
#define PAIRING_HEAP_H

#include "priorityQueue.h"
#include <stdexcept>
#include <utility>
#include <vector>

template <typename K, typename V>
struct PairNode : public Node<K, V> {
    PairNode* child;
    PairNode* next;
    PairNode* prev; //points to parent if first child, otherwise left sibling

    PairNode(const K& k, const V& v)
        : child(nullptr), next(nullptr), prev(nullptr) {
        this->key = k;
        this->value = v;
    }
};

template <typename K, typename V>
class PairingHeap : public PriorityQueue<K, V> {
private:
    PairNode<K, V>* root_;
    int nodeCount_;

    //combines two heaps, smaller root becomes parent
    static PairNode<K, V>* meld(PairNode<K, V>* a, PairNode<K, V>* b) {
        if (!a) return b;
        if (!b) return a;

        if (b->key < a->key) {
            PairNode<K, V>* tmp = a;
            a = b;
            b = tmp;
        }

        //attach b as first child of a
        b->prev = a;
        b->next = a->child;
        if (a->child) {
            a->child->prev = b;
        }
        a->child = b;

        return a;
    }

    //detach node from its parent or left sibling
    static void cutFromParentOrSibling(PairNode<K, V>* x) {
        if (!x || !x->prev) return;

        PairNode<K, V>* p = x->prev;

        if (p->child == x) {
            p->child = x->next;
            if (x->next) {
                x->next->prev = p;
            }
        } else {
            p->next = x->next;
            if (x->next) {
                x->next->prev = p;
            }
        }

        x->prev = nullptr;
        x->next = nullptr;
    }

    //two pass pairing used after extract_min
    //pass 1: meld pairs left to right
    //pass 2: meld results right to left
    static PairNode<K, V>* twoPassMerge(PairNode<K, V>* firstSibling) {
        if (!firstSibling) return nullptr;

        std::vector<PairNode<K, V>*> merged;
        PairNode<K, V>* cur = firstSibling;

        while (cur) {
            PairNode<K, V>* a = cur;
            PairNode<K, V>* b = cur->next;

            PairNode<K, V>* nextPair = nullptr;
            if (b) nextPair = b->next;

            a->prev = nullptr;
            a->next = nullptr;

            if (b) {
                b->prev = nullptr;
                b->next = nullptr;
            }

            merged.push_back(meld(a, b));
            cur = nextPair;
        }

        PairNode<K, V>* res = nullptr;
        for (int i = static_cast<int>(merged.size()) - 1; i >= 0; --i) {
            res = meld(res, merged[i]);
        }
        return res;
    }

    //recursively delete all nodes
    static void deleteAll(PairNode<K, V>* n) {
        if (!n) return;
        PairNode<K, V>* c = n->child;
        while (c) {
            PairNode<K, V>* next = c->next;
            deleteAll(c);
            c = next;
        }
        delete n;
    }

public:
    PairingHeap() : root_(nullptr), nodeCount_(0) {}

    ~PairingHeap() override {
        deleteAll(root_);
        root_ = nullptr;
        nodeCount_ = 0;
    }

    bool is_empty() override {
        return root_ == nullptr;
    }

    Node<K, V>* insert(K key, V value) override {
        PairNode<K, V>* n = new PairNode<K, V>(key, value);
        root_ = meld(root_, n);
        nodeCount_++;
        return n;
    }

    std::pair<K, V> find_min() override {
        if (!root_) throw std::runtime_error("Heap is empty");
        return {root_->key, root_->value};
    }

    std::pair<K, V> extract_min() override {
        if (!root_) throw std::runtime_error("Heap is empty");

        PairNode<K, V>* oldRoot = root_;
        std::pair<K, V> result = {oldRoot->key, oldRoot->value};

        PairNode<K, V>* children = oldRoot->child;
        if (children) {
            children->prev = nullptr;
        }

        //detach old root so delete doesnt follow children
        oldRoot->child = nullptr;

        root_ = twoPassMerge(children);

        delete oldRoot;
        nodeCount_--;
        return result;
    }

    void decrease_key(Node<K, V>* node, K new_key) override {
        PairNode<K, V>* x = static_cast<PairNode<K, V>*>(node);
        if (!x) throw std::runtime_error("Null node handle");
        if (new_key > x->key) throw std::runtime_error("New key is greater than current key");

        x->key = new_key;

        //if not root, cut and meld back in
        if (x != root_ && x->prev) {
            cutFromParentOrSibling(x);
            root_ = meld(root_, x);
        }
    }
};

#endif