#pragma once

#ifdef LRUCACHE_EXPORTS
#define LRUCACHE_API __declspec(dllexport)

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

    LRUCACHE_API ValueType get(int key);

    LRUCACHE_API void put(int key, const ValueType& value);

    LRUCACHE_API std::vector<std::pair<int, ValueType>> getCacheData();
};

#else
#define LRUCACHE_API __declspec(dllimport)
#endif