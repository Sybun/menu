#include <iostream>
#include <iomanip>
#include <algorithm>
#include "MenuItem.h"
#include "Menu.h"
#include "SortStrategy.h"

static void printSeparator(const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
}

int main() {
    std::cout << "*** Restaurant Menu Manager Demo ***\n";

    Menu menu;
    menu.add(new FoodItem ("Margherita Pizza", 11.50, "Dough, Tomato, Mozzarella, Basil"));
    menu.add(new DrinkItem("Sparkling Water",   2.00, 330,  0.0));
    menu.add(new FoodItem ("Caesar Salad",      9.00, "Romaine, Parmesan, Croutons, Dressing"));
    menu.add(new DrinkItem("House Red Wine",    7.50, 200, 13.0));
    menu.add(new FoodItem ("Beef Burger",      14.00, "Beef patty, Brioche bun, Lettuce, Tomato, Cheddar"));
    menu.add(new DrinkItem("Orange Juice",      4.50, 300,  0.0));

    printSeparator("Full menu (polymorphic display)");
    // polymorphism
    menu.displayAll();

    printSeparator("dynamic_cast to FoodItem*");
    MenuItem* basePtr = menu.get("Margherita Pizza");
    if (FoodItem* foodPtr = dynamic_cast<FoodItem*>(basePtr)) {
        std::cout << "Ingredients of \"Margherita Pizza\":\n  "
                  << foodPtr->getIngredients() << "\n";
    }

    SortByPrice priceStrategy;
    // strategy
    menu.setSortStrategy(&priceStrategy);
    menu.sort();
    printSeparator("Sorted by price");
    menu.displayAll();

    SortByName nameStrategy;
    menu.setSortStrategy(&nameStrategy);
    menu.sort();
    printSeparator("Sorted by name");
    menu.displayAll();

    printSeparator("ForwardIterator + std::for_each");
    // iterator
    std::for_each(menu.begin(), menu.end(), [](MenuItem* item) {
        std::cout << "  " << std::left << std::setw(22) << item->getName()
                  << " $" << std::fixed << std::setprecision(2)
                  << item->getPrice() << "\n";
    });

    printSeparator("Deep copy");
    // deep copy
    Menu menuCopy = menu;
    menuCopy.add(new DrinkItem("Craft Beer", 5.50, 500, 4.8));
    std::cout << "Original size: " << menu.size()
              << "  |  Copy size: " << menuCopy.size() << "\n";

    printSeparator("Callback #1 — items under $5.00");
    // callback
    menu.applyToAll([](MenuItem* item) {
        if (item->getPrice() < 5.00) {
            std::cout << "  Budget pick: " << item->getName()
                      << "  $" << item->getPrice() << "\n";
        }
    });

    printSeparator("Callback #2 — 10% happy-hour discount");
    menu.applyToAll([](MenuItem* item) {
        item->setPrice(item->getPrice() * 0.90);
    });
    menu.displayAll();

    printSeparator("Exception: StrategyNotSet");
    Menu noStrategy;
    noStrategy.add(new FoodItem("Test Item", 5.00, "Test ingredients"));
    try {
        noStrategy.sort();
    } catch (const StrategyNotSet& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    printSeparator("Exception: NotImplementedException");
    // PlaceholderItem inherits MenuItem::display() (which throws) by not overriding it.
    class PlaceholderItem : public MenuItem {
    public:
        PlaceholderItem(const std::string& n, double p) : MenuItem(n, p) {}
        MenuItem* clone() const override { return new PlaceholderItem(*this); }
    };
    PlaceholderItem placeholder("Unknown Item", 10.00);
    try {
        placeholder.display();
    } catch (const NotImplementedException& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    std::cout << "\n*** Demo complete ***\n";
    return 0;
}
