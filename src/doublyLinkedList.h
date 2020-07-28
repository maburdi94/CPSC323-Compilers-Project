#pragma once

#include "lexer.h"

class Node {
public:
    Lexer::OutputType m_token;
    Node* next;
    Node* prev;
};

class DoublyLinkedList {
    Node* head;
    Node* tail;

public:
    DoublyLinkedList() {
        head = nullptr;
        tail = nullptr;
    }
    void add (Lexer::OutputType token) {
        if (head == nullptr && tail == nullptr) {
            head->m_token = token;
            tail = head;
            return;
        }
        else {
            while (head->next->next != nullptr) {
                head = head->next;

            }
        }
    }
};