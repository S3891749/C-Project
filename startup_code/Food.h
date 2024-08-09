#ifndef FOOD_H
#define FOOD_H

#include "Node.h"
#include <string>

class Food : public FoodItem {
private:
    std::string id;
    std::string name;
    std::string description;
    double price;
    int onHand;

public:
    Food(std::string id, std::string name, std::string description, double price, unsigned onHand)
        : FoodItem(id, name, description, Price::parsePrice(price), onHand) {}

    std::string getID() const { return id; }
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    double getPrice() const { return price; }
    int getOnHand() const { return onHand; }

    bool operator<(const Food &other) const {
        return name < other.name;
    }
};

#endif  // FOOD_H