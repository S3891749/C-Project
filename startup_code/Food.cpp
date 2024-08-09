#include <iostream>
#include <string>
#include <iomanip>
#include "LinkedList.h"
#include "Food.h"

Food::Food(std::string id, std::string name, std::string description, double price, unsigned onHand)
    : FoodItem(id, name, description, Price::parsePrice(price), onHand) {}

std::string Food::getID() const {
    return id;
}

std::string Food::getName() const {
    return name;
}

std::string Food::getDescription() const {
    return description;
}

double Food::getPrice() const {
    return price;
}

int Food::getOnHand() const {
    return onHand;
}

bool Food::operator<(const Food &other) const {
    return name < other.name;
}