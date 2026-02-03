//interface for implementing priority queues with different heaps
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <utility>

using namespace std;

template<typename K, typename V>
struct Node {
    K key;
    V value;
};

template<typename K, typename V>
class PriorityQueue {
public:

    virtual Node<K,V>* insert(K key, V value) = 0;
    
    virtual pair<K,V> extract_min() = 0;
    
    virtual void decrease_key(Node<K,V>* node, K new_key) = 0;
    
    virtual bool is_empty() = 0;
    
    virtual ~PriorityQueue() {}
};

#endif