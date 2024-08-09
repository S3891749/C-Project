#ifndef NODE_H
#define NODE_H

#include <string>
#include "Coin.h"

// The length of the id string not counting the null terminator
#define IDLEN 5

// The maximum length of a food item name not counting the null terminator
#define NAMELEN 40

// The maximum length of a food item description not counting the null terminator
#define DESCLEN 255

// The default coin level to reset the coins to on request
#define DEFAULT_COIN_COUNT 20

// The possible default food stock level that all new stock should start at and that we should reset to on restock
#define DEFAULT_FOOD_STOCK_LEVEL 20

// The number of denominations of currency available in the system 
#define NUM_DENOMS 8

class Price {
public:
    int dollars;
    int cents;

    Price(int dollars = 0, int cents = 0) : dollars(dollars), cents(cents) {}

    static Price parsePrice(double value) {
        int dollars = static_cast<int>(value);
        int cents = static_cast<int>((value - dollars) * 100.0 + 0.5); // Adding 0.5 to round to the nearest cent
        return Price(dollars, cents);
    }

};

class FoodItem {
public:
    std::string id;
    std::string name;
    std::string description;
    Price price;
    unsigned on_hand;

    FoodItem(std::string id, std::string name, std::string description, Price price, unsigned on_hand)
        : id(id), name(name), description(description), price(price), on_hand(on_hand) {}
};

class Node {
public:
    Node();
    ~Node();
    FoodItem* data;
    Node* next;
};

#endif  // NODE_H