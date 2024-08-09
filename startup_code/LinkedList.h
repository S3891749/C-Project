#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Node.h"

class LinkedList {
public:
    LinkedList();
    ~LinkedList();

    void insertNode(const FoodItem& data);
    void displayMenu() const;
    void loadMenuFromFile(const std::string& filename);
    Node* findItem(const std::string& itemId) const;
    bool removeItem(const std::string& itemId);
    void saveMenuToFile(const std::string& filename) const;
    Node* getHead() const { return head; }

private:
    Node* head;
    unsigned count;
};

#endif  // LINKEDLIST_H