#pragma once

#ifdef LRUCACHE_EXPORTS
#define LRUCACHE_EXPORTS __declspec(dllexport)

#include <iostream>
#include <unordered_map>
#include <vector>

template <typename ValueType>
class LRUCache {
private:
    struct Node {
        int key;
        ValueType value;
        Node* prev;
        Node* next;

        Node(int k, const ValueType& v) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };

    std::unordered_map<int, Node*> cache;
    Node* head;
    Node* tail;
    int capacity;

    void moveToHead(Node* node) {
        // Remove the node from its current position
        if (node->prev) {
            node->prev->next = node->next;
        }
        else {
            // If the node is the head, update the head
            head = node->next;
        }

        if (node->next) {
            node->next->prev = node->prev;
        }
        else {
            // If the node is the tail, update the tail
            tail = node->prev;
        }

        // Move the node to the head
        node->next = head;
        node->prev = nullptr;

        if (head) {
            head->prev = node;
        }

        head = node;

        // If the list was empty, update the tail
        if (!tail) {
            tail = head;
        }
    }

public:
    LRUCache(int capacity) : capacity(capacity), head(nullptr), tail(nullptr) {}

    ~LRUCache() {
        // Destructor to deallocate memory for all nodes
        Node* current = head;
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }

    // Copy operations are disabled to prevent issues related to dynamic memory
    LRUCache(const LRUCache& other) = delete;
    LRUCache& operator=(const LRUCache& other) = delete;

    // Move constructor
    LRUCache(LRUCache&& other) noexcept : capacity(other.capacity), head(other.head), tail(other.tail), cache(std::move(other.cache)) {
        // Reset the other object
        other.capacity = 0;
        other.head = nullptr;
        other.tail = nullptr;
    }

    // Move assignment operator
    LRUCache& operator=(LRUCache&& other) noexcept {
        if (this != &other) {
            // Destructor to deallocate memory for current object
            Node* current = head;
            while (current) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }

            // Move resources from other to current object
            capacity = other.capacity;
            head = other.head;
            tail = other.tail;
            cache = std::move(other.cache);

            // Reset the other object
            other.capacity = 0;
            other.head = nullptr;
            other.tail = nullptr;
        }

        return *this;
    }

    LRUCACHE_EXPORTS ValueType get(int key);

    LRUCACHE_EXPORTS void put(int key, const ValueType& value);

    LRUCACHE_EXPORTS std::vector<std::pair<int, ValueType>> getCacheData();
};

#endif