#pragma once
#include <memory_resource>
#include <stdexcept>
#include <iterator>
#include <memory>

template <typename T>
class Stack {
private:
    struct Node {
        T value;
        Node* next;
    };

    std::pmr::polymorphic_allocator<Node> allocator;
    Node* head = nullptr;

public:
    explicit Stack(std::pmr::memory_resource* mr) : allocator(mr) {}

    void push(const T& val) {
        Node* new_node = allocator.allocate(1);
        try {
            allocator.construct(new_node, Node{val, head});
        } catch (...) {
            allocator.deallocate(new_node, 1);
            throw;
        }
        head = new_node;
    }

    void pop() {
        if (empty()) throw std::out_of_range("Stack is empty");
        Node* temp = head;
        head = head->next;
        std::destroy_at(temp);
        allocator.deallocate(temp, 1);
    }

    T& top() {
        if (empty()) throw std::out_of_range("Stack is empty");
        return head->value;
    }

    bool empty() const {return head == nullptr;}

    ~Stack() {
        while (!empty()) pop();
    }

    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;

        Iterator(Node* ptr) : current(ptr) {}

        T& operator*() const { return current->value; }
        T* operator->() { return &current->value; }

        Iterator& operator++() {
            if (current) current = current->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator& other) const {return current == other.current;}
        bool operator!=(const Iterator& other) const {return current != other.current;}

    private:
        Node* current;
    };

    Iterator begin() {return Iterator(head);}
    Iterator end() {return Iterator(nullptr);}
};