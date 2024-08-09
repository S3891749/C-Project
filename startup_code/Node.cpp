#include "Node.h"

Node::Node() : data(nullptr), next(nullptr) {}

Node::~Node() {
    delete data;
    data = nullptr;
}