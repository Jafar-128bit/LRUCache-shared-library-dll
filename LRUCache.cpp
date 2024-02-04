#include "pch.h"
#include "LRUCache.h"

template <typename ValueType>
ValueType LRUCache<ValueType>::get(int key) {
    if (cache.find(key) != cache.end()) {
        Node* node = cache[key];
        moveToHead(node);
        return node->value;
    }

    // Return a default-constructed object if the key is not found
    return ValueType();
}

template <typename ValueType>
void LRUCache<ValueType>::put(int key, const ValueType& value) {
    if (cache.find(key) != cache.end()) {
        // If the key already exists, update the value and move to the head
        cache[key]->value = value;
        moveToHead(cache[key]);
    }
    else {
        // If the cache is at capacity, remove the least recently used item (tail)
        if (cache.size() == capacity) {
            cache.erase(tail->key);

            if (tail->prev) {
                tail->prev->next = nullptr;
            }
            else {
                head = nullptr;
            }

            Node* temp = tail;
            tail = tail->prev;
            delete temp;
        }

        // Add a new node to the head
        Node* newNode = new Node(key, value);
        newNode->next = head;

        if (head) {
            head->prev = newNode;
        }

        head = newNode;

        // If the list was empty, update the tail
        if (!tail) {
            tail = head;
        }

        // Update the cache
        cache[key] = newNode;
    }
}

template <typename ValueType>
std::vector<std::pair<int, ValueType>> LRUCache<ValueType>::getCacheData() {
    std::vector<std::pair<int, ValueType>> cacheData;
    Node* current = head;
    while (current) {
        cacheData.push_back(std::make_pair(current->key, current->value));
        current = current->next;
    }
    return cacheData;
}