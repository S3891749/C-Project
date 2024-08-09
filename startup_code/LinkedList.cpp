#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "LinkedList.h"

LinkedList::LinkedList() : head(nullptr), count(0) {}

LinkedList::~LinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    count = 0;
}

void LinkedList::insertNode(const FoodItem& data) {
    Node* newNode = new Node();
    newNode->data = new FoodItem(data);
    newNode->next = nullptr;

    if (head == nullptr || head->data->id > data.id) {
        newNode->next = head;
        head = newNode;
    } else {
        Node* current = head;
        while (current->next != nullptr && current->next->data->id <= data.id) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
    count++;
}

void LinkedList::displayMenu() const {
    std::cout << "Food Menu\n";
    std::cout << "---------\n";
    std::cout << "ID    | Name                                     | Price\n";
    std::cout << "------------------------------------------------------------------\n";

    Node* current = head;
    while (current != nullptr) {
        std::ostringstream nameStream;
        nameStream << std::setw(NAMELEN) << std::left << current->data->name;

        std::cout << std::setw(IDLEN) << std::left << current->data->id << " | "
                  << nameStream.str() << " | $"
                  << current->data->price.dollars << '.';
        
        if (current->data->price.cents < 10) {
            std::cout << '0';
        }
        std::cout << current->data->price.cents << std::endl;
        current = current->next;
    }
    std::cout << "" << std::endl;
}

void LinkedList::loadMenuFromFile(const std::string& filename) {
    std::ifstream file(filename);
    bool success = file.is_open();
    if (!success) {
        std::cerr << "Error opening file: " << filename << std::endl;
    } else {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string id, name, description, priceStr;
            char delimiter = '|';
            if (std::getline(iss, id, delimiter) &&
                std::getline(iss, name, delimiter) &&
                std::getline(iss, description, delimiter) &&
                std::getline(iss, priceStr, delimiter)) {
                size_t dotPos = priceStr.find('.');
                if (dotPos != std::string::npos) {
                    unsigned int dollars = std::stoi(priceStr.substr(0, dotPos));
                    unsigned int cents = std::stoi(priceStr.substr(dotPos + 1));
                    Price price(dollars, cents);
                    FoodItem item(id, name, description, price, DEFAULT_FOOD_STOCK_LEVEL);
                    insertNode(item);
                } else {
                    std::cerr << "Error reading price from file: " << filename << std::endl;
                }
            } else {
                std::cerr << "Error reading line from file: " << filename << std::endl;
            }
        }
        file.close();
    }
}

Node* LinkedList::findItem(const std::string& itemId) const {
    Node* current = head;
    Node* foundNode = nullptr;

    while (current != nullptr && foundNode == nullptr) {
        if (current->data->id == itemId) {
            foundNode = current;
        } else {
            current = current->next;
        }
    }
    return foundNode;
}

bool LinkedList::removeItem(const std::string& itemId) {
    Node* current = head;
    Node* previous = nullptr;
    bool itemRemoved = false;

    while (current != nullptr && !itemRemoved) {
        if (current->data->id == itemId) {
            if (previous == nullptr) {
                head = current->next;
            } else {
                previous->next = current->next;
            }
            std::cout << "\"" << current->data->id << " - " << current->data->name << " - " << current->data->description << "\" has been removed from the system." << std::endl;
            delete current;
            count--;
            itemRemoved = true;
        } else {
            previous = current;
            current = current->next;
        }
    }

    if (!itemRemoved) {
        std::cerr << "Food item with ID " << itemId << " not found." << std::endl;
    }

    return itemRemoved;
}

void LinkedList::saveMenuToFile(const std::string& filename) const {
    std::ofstream file(filename);
    bool fileOpened = file.is_open();

    if (!fileOpened) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
    } else {
        Node* current = head;
        while (current != nullptr) {
            file << current->data->id << '|'
                 << current->data->name << '|'
                 << current->data->description << '|'
                 << current->data->price.dollars << '.';
            if (current->data->price.cents < 10) {
                file << '0';
            }
            file << current->data->price.cents << std::endl;
            current = current->next;
        }
        file.close();
    }
}