#include "MenuItem.h"

MenuItem::MenuItem(const std::string& name, double price)
    : name(name), price(price) {}

void MenuItem::display() const {
    throw NotImplementedException();
}

std::string MenuItem::getName()  const { return name; }
double      MenuItem::getPrice() const { return price; }

void MenuItem::setPrice(double newPrice) {
    price = newPrice;
}

bool MenuItem::operator<(const MenuItem& other) const {
    return name < other.name;
}
