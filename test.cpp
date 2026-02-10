// test.cpp
#include <iostream>
#include <cassert>
#include "priorityQueue.h"
#include "fibHeap.h"
// #include "PairingHeap.h"  // Uncomment when ready

using namespace std;

// Test basic operations
void test_basic_operations(PriorityQueue<int, string>* pq, string heap_name) {
    cout << "\n=== Testing " << heap_name << " - Basic Operations ===" << endl;
    
    // Test insert and find_min
    pq->insert(5, "five");
    pq->insert(3, "three");
    pq->insert(7, "seven");
    pq->insert(1, "one");
    
    auto [key, val] = pq->find_min();
    assert(key == 1);
    assert(val == "one");
    cout << "✓ Insert and find_min work" << endl;
    
    // Test that find_min doesn't remove
    auto [key2, val2] = pq->find_min();
    assert(key2 == 1);
    cout << "✓ find_min doesn't remove element" << endl;
}

// Test extract_min
void test_extract_min(PriorityQueue<int, string>* pq, string heap_name) {
    cout << "\n=== Testing " << heap_name << " - Extract Min ===" << endl;
    
    pq->insert(5, "five");
    pq->insert(3, "three");
    pq->insert(7, "seven");
    pq->insert(1, "one");
    
    auto [k1, v1] = pq->extract_min();
    assert(k1 == 1 && v1 == "one");
    cout << "✓ First extract_min: " << k1 << endl;
    
    auto [k2, v2] = pq->extract_min();
    assert(k2 == 3 && v2 == "three");
    cout << "✓ Second extract_min: " << k2 << endl;
    
    auto [k3, v3] = pq->extract_min();
    assert(k3 == 5 && v3 == "five");
    cout << "✓ Third extract_min: " << k3 << endl;
    
    auto [k4, v4] = pq->extract_min();
    assert(k4 == 7 && v4 == "seven");
    cout << "✓ Fourth extract_min: " << k4 << endl;
    
    assert(pq->is_empty());
    cout << "✓ Heap is empty after extracting all" << endl;
}

// Test decrease_key
void test_decrease_key(PriorityQueue<int, string>* pq, string heap_name) {
    cout << "\n=== Testing " << heap_name << " - Decrease Key ===" << endl;
    
    auto node5 = pq->insert(5, "five");
    auto node3 = pq->insert(3, "three");
    auto node7 = pq->insert(7, "seven");
    auto node10 = pq->insert(10, "ten");
    
    // Decrease 10 to 1 (should become new min)
    pq->decrease_key(node10, 1);
    auto [k1, v1] = pq->find_min();
    assert(k1 == 1 && v1 == "ten");
    cout << "✓ Decrease key works (10 → 1)" << endl;
    
    // Decrease 7 to 2
    pq->decrease_key(node7, 2);
    auto [k2, v2] = pq->extract_min();
    assert(k2 == 1);  // Still 1
    auto [k3, v3] = pq->find_min();
    assert(k3 == 2 && v3 == "seven");
    cout << "✓ Decrease key works (7 → 2)" << endl;
}

// Test with larger dataset
void test_large_dataset(PriorityQueue<int, int>* pq, string heap_name) {
    cout << "\n=== Testing " << heap_name << " - Large Dataset ===" << endl;
    
    int n = 100;
    
    // Insert in reverse order
    for (int i = n; i >= 1; i--) {
        pq->insert(i, i * 10);
    }
    
    // Extract all and verify sorted
    int prev = 0;
    for (int i = 0; i < n; i++) {
        auto [key, val] = pq->extract_min();
        assert(key > prev);
        prev = key;
    }
    
    assert(pq->is_empty());
    cout << "✓ Successfully inserted and extracted " << n << " elements in order" << endl;
}

// Test decrease_key extensively
void test_decrease_key_extensive(PriorityQueue<int, int>* pq, string heap_name) {
    cout << "\n=== Testing " << heap_name << " - Extensive Decrease Key ===" << endl;
    
    vector<Node<int, int>*> nodes;
    
    // Insert 20 elements
    for (int i = 20; i >= 1; i--) {
        nodes.push_back(pq->insert(i, i));
    }
    
    // Decrease every other element
    for (int i = 0; i < nodes.size(); i += 2) {
        pq->decrease_key(nodes[i], -(i + 1));  // Make negative to ensure they become min
    }
    
    // Extract and verify order
    int count = 0;
    while (!pq->is_empty()) {
        auto [key, val] = pq->extract_min();
        count++;
    }
    
    assert(count == 20);
    cout << "✓ Successfully decreased keys and extracted all " << count << " elements" << endl;
}

// Test empty heap exception
void test_empty_heap_exception(PriorityQueue<int, string>* pq, string heap_name) {
    cout << "\n=== Testing " << heap_name << " - Empty Heap Exception ===" << endl;
    
    try {
        pq->find_min();
        cout << "✗ Should have thrown exception!" << endl;
        assert(false);
    } catch (const runtime_error& e) {
        cout << "✓ find_min throws exception on empty heap" << endl;
    }
    
    try {
        pq->extract_min();
        cout << "✗ Should have thrown exception!" << endl;
        assert(false);
    } catch (const runtime_error& e) {
        cout << "✓ extract_min throws exception on empty heap" << endl;
    }
}

// Test single element
void test_single_element(PriorityQueue<int, string>* pq, string heap_name) {
    cout << "\n=== Testing " << heap_name << " - Single Element ===" << endl;
    
    pq->insert(42, "answer");
    
    auto [k1, v1] = pq->find_min();
    assert(k1 == 42);
    cout << "✓ find_min on single element" << endl;
    
    auto [k2, v2] = pq->extract_min();
    assert(k2 == 42 && v2 == "answer");
    assert(pq->is_empty());
    cout << "✓ extract_min on single element" << endl;
}

// Run all tests for a given heap
void run_all_tests(PriorityQueue<int, string>* pq, string heap_name) {
    cout << "\n" << string(50, '=') << endl;
    cout << "TESTING: " << heap_name << endl;
    cout << string(50, '=') << endl;
    
    try {
        test_empty_heap_exception(pq, heap_name);
        delete pq;
        
        pq = (heap_name == "FibonacciHeap") ? 
            (PriorityQueue<int, string>*) new FibonacciHeap<int, string>() : nullptr;
        test_single_element(pq, heap_name);
        delete pq;
        
        pq = (heap_name == "FibonacciHeap") ? 
            (PriorityQueue<int, string>*) new FibonacciHeap<int, string>() : nullptr;
        test_basic_operations(pq, heap_name);
        delete pq;
        
        pq = (heap_name == "FibonacciHeap") ? 
            (PriorityQueue<int, string>*) new FibonacciHeap<int, string>() : nullptr;
        test_extract_min(pq, heap_name);
        delete pq;
        
        pq = (heap_name == "FibonacciHeap") ? 
            (PriorityQueue<int, string>*) new FibonacciHeap<int, string>() : nullptr;
        test_decrease_key(pq, heap_name);
        delete pq;
        
        PriorityQueue<int, int>* pq_int = (heap_name == "FibonacciHeap") ? 
            (PriorityQueue<int, int>*) new FibonacciHeap<int, int>() : nullptr;
        test_large_dataset(pq_int, heap_name);
        delete pq_int;
        
        pq_int = (heap_name == "FibonacciHeap") ? 
            (PriorityQueue<int, int>*) new FibonacciHeap<int, int>() : nullptr;
        test_decrease_key_extensive(pq_int, heap_name);
        delete pq_int;
        
        cout << "\n✅ ALL TESTS PASSED for " << heap_name << "!" << endl;
        
    } catch (const exception& e) {
        cout << "\n❌ TEST FAILED: " << e.what() << endl;
    }
}

int main() {
    cout << "Starting Priority Queue Tests..." << endl;
    
    // Test Fibonacci Heap
    PriorityQueue<int, string>* fib_heap = new FibonacciHeap<int, string>();
    run_all_tests(fib_heap, "FibonacciHeap");
    
    // Test Pairing Heap (uncomment when implemented)
    // PriorityQueue<int, string>* pairing_heap = new PairingHeap<int, string>();
    // run_all_tests(pairing_heap, "PairingHeap");
    
    cout << "\n" << string(50, '=') << endl;
    cout << "ALL TESTS COMPLETE!" << endl;
    cout << string(50, '=') << endl;
    
    return 0;
}