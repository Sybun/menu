#include "MenuItem.h"
#include <iostream>
#include <iomanip>

FoodItem::FoodItem(const std::string& name, double price,
                   const std::string& ingredients)
    : MenuItem(name, price), ingredients(ingredients) {}

MenuItem* FoodItem::clone() const {
    return new FoodItem(*this);
}

void FoodItem::display() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[FOOD]  " << getName() << "  $" << getPrice() << "\n"
              << "        Ingredients: " << ingredients << "\n";
}

std::string FoodItem::getIngredients() const {
    return ingredients;
}
