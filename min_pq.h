
// code translated to C++ from https://algs4.cs.princeton.edu/24pq/IndexMinPQ.java.html

#include <string>
#include <iostream>
#include <cassert>
using namespace std;

template<class T>
class MinPQ {
public:
    MinPQ(int maxN); // maxN is the maximum number of elements in the PQ
    ~MinPQ();

    bool isEmpty() const; // checks if no elements are currently in the PQ
    int size() const;     // number of elements currently in the PQ

    void insert(int i, T key); // Associates key with index i.
    int delMin();              // Removes a minimum key and returns its associated index.
    void remove(int i);        // Remove the key associated with index i.

    int minIndex() const;  // Returns an index associated with a minimum key.
    T minKey() const;      // Returns a minimum key.

    bool contains(int i) const; // Is i an index on this priority queue?
    T keyOf(int i) const;       // Returns the key associated with index i.

    void changeKey(int i, T key);   // Change the key associated with index i 
                                    // to the specified value.
    void decreaseKey(int i, T key); // Decrease the key associated with index i 
                                    // to the specified value.
    void increaseKey(int i, T key); // Increase the key associated with index i 
                                    // to the specified value.

private:
    int maxN;       // maximum number of elements on PQ
    int n;          // number of elements on PQ
    int* pq;        // binary heap using 1-based indexing
    int* qp;        // inverse of pq - qp[pq[i]] = pq[qp[i]] = i
    T* keys;        // keys[i] = priority of i

    // Helper functions
    void validateIndex(int i) const;
    bool greater(int i, int j) const;
    void exch(int i, int j);
    void swim(int k);
    void sink(int k);
};

template<class T>
MinPQ<T>::MinPQ(int maxN) {
    if (maxN < 0) throw string("Size can't be negative");
    this->maxN = maxN;
    n = 0;

    keys = new T[maxN + 1];      
    pq   = new int[maxN + 1];
    qp   = new int[maxN + 1];    
    for (int i = 0; i <= maxN; i++)
        qp[i] = -1;
}

template<class T>
MinPQ<T>::~MinPQ() {
    delete [] pq;
    delete [] qp;
    delete [] keys;
}

template<class T>
bool MinPQ<T>::isEmpty() const { return n == 0; }

template<class T>
int MinPQ<T>::size() const { return n; }

// Is i an index on this priority queue?
template<class T>
bool MinPQ<T>::contains(int i) const {
    validateIndex(i);
    return qp[i] != -1;
}

// Associates key with index i.
template<class T>
void MinPQ<T>::insert(int i, T key) {
    validateIndex(i);
    if (contains(i)) throw  string("index is already in the priority queue");
    n++;
    qp[i] = n;
    pq[n] = i;
    keys[i] = key;
    swim(n);
}

// Returns an index associated with a minimum key.
template<class T>
int MinPQ<T>::minIndex() const {
    if (n == 0) throw string("Priority queue underflow");
    return pq[1];
}

// Returns a minimum key.
template<class T>
T MinPQ<T>::minKey() const {
    if (n == 0) throw string("Priority queue underflow");
    return keys[pq[1]];
}

// Removes a minimum key and returns its associated index.
template<class T>
int MinPQ<T>::delMin() {
    if (n == 0) throw string("Priority queue underflow");
    int min = pq[1];
    exch(1, n--);
    sink(1);
    assert(min == pq[n+1]);
    qp[min] = -1;        // delete
    pq[n+1] = -1;        // not needed
    return min;
}

// Returns the key associated with index i.
template<class T>
T MinPQ<T>::keyOf(int i) const {
    validateIndex(i);
    if (!contains(i)) throw string("index is not in the priority queue");
    else return keys[i];
}

// Change the key associated with index i to the specified value.
template<class T>
void MinPQ<T>::changeKey(int i, T key) {
    validateIndex(i);
    if (!contains(i)) throw string("index is not in the priority queue");
    keys[i] = key;
    swim(qp[i]);
    sink(qp[i]);
}

// Decrease the key associated with index i to the specified value.
template<class T>
void MinPQ<T>::decreaseKey(int i, T key) {
    validateIndex(i);
    if (!contains(i)) 
        throw string("index is not in the priority queue");
    if (keys[i] == key)
        throw string("Calling decreaseKey() with a key equal to the key in the priority queue");
    if (keys[i] < key)
        throw string("Calling decreaseKey() with a key > the key in the priority queue");
    keys[i] = key;
    swim(qp[i]);
}

// Increase the key associated with index i to the specified value.
template<class T>
void MinPQ<T>::increaseKey(int i, T key) {
    validateIndex(i);
    if (!contains(i)) 
        throw string("index is not in the priority queue");
    if (keys[i] == key)
        throw string("Calling increaseKey() with a key equal to the key in the priority queue");
    if (keys[i] > key)
        throw string("Calling increaseKey() with a key < the key in the priority queue");
    keys[i] = key;
    sink(qp[i]);
}

// Remove the key associated with index i.
template<class T>
void MinPQ<T>::remove(int i) {
    validateIndex(i);
    if (!contains(i)) 
        throw string("index is not in the priority queue");
    int index = qp[i];
    exch(index, n--);
    swim(index);
    sink(index);
    qp[i] = -1;
}

// throw an IllegalArgumentException if i is an invalid index
template<class T>
void MinPQ<T>::validateIndex(int i) const {
    if (i < 0) throw string("index is negative: " + i);
    if (i >= maxN) throw string("index >= capacity: " + i);
}

template<class T>
bool MinPQ<T>::greater(int i, int j) const {
    return keys[pq[i]] > keys[pq[j]];
}

template<class T>
void MinPQ<T>::exch(int i, int j) {
    int swap = pq[i];
    pq[i] = pq[j];
    pq[j] = swap;
    qp[pq[i]] = i;
    qp[pq[j]] = j;
}

template<class T>
void MinPQ<T>::swim(int k) {
    while (k > 1 && greater(k/2, k)) {
        exch(k, k/2);
        k = k/2;
    }
}

template<class T>
void MinPQ<T>::sink(int k) {
    while (2*k <= n) {
        int j = 2*k;
        if (j < n && greater(j, j+1)) j++;
        if (!greater(k, j)) break;
        exch(k, j);
        k = j;
    }
}