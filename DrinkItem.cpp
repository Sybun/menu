#include "MenuItem.h"
#include <iostream>
#include <iomanip>

DrinkItem::DrinkItem(const std::string& name, double price,
                     int volumeMl, double alcoholPercent)
    : MenuItem(name, price),
      volumeMl(volumeMl),
      alcoholPercent(alcoholPercent) {}

MenuItem* DrinkItem::clone() const {
    return new DrinkItem(*this);
}

void DrinkItem::display() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[DRINK] " << getName() << "  $" << getPrice() << "\n"
              << "        Volume: " << volumeMl << " ml"
              << "  |  Alcohol: " << alcoholPercent << "%\n";
}

int    DrinkItem::getVolumeMl()       const { return volumeMl; }
double DrinkItem::getAlcoholPercent() const { return alcoholPercent; }
