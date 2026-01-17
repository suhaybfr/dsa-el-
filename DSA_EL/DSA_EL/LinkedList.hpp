#pragma once

template<typename T>
struct Node {
    T data;
    Node* next;
};

template<typename T>
class LinkedList {
public:
    LinkedList() : head(nullptr) {}

    void push_front(const T& value) {
        Node<T>* n = new Node<T>{ value, head };
        head = n;
    }

    void clear() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    Node<T>* getHead() const { return head; }

private:
    Node<T>* head;
};
