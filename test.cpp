#include <iostream>
#include <algorithm>
#include "MenuItem.h"
#include "Menu.h"
#include "SortStrategy.h"

static int passed = 0;
static int failed = 0;

static void check(bool condition, const char* msg) {
    if (condition) { std::cout << "  [PASS] " << msg << "\n"; ++passed; }
    else           { std::cout << "  [FAIL] " << msg << "\n"; ++failed; }
}

static void testFoodItem() {
    std::cout << "\n--- FoodItem ---\n";
    FoodItem f("Pasta", 10.50, "Wheat, Tomato, Garlic");

    check(f.getName()        == "Pasta",                  "getName()");
    check(f.getPrice()       == 10.50,                    "getPrice()");
    check(f.getIngredients() == "Wheat, Tomato, Garlic",  "getIngredients()");

    MenuItem* c = f.clone();
    check(c != nullptr,                       "clone() not null");
    check(c->getName() == "Pasta",            "clone keeps name");
    f.setPrice(12.00);
    check(c->getPrice() == 10.50,             "clone is independent of original");

    FoodItem* fc = dynamic_cast<FoodItem*>(c);
    check(fc != nullptr,                      "dynamic_cast<FoodItem*> on clone");

    bool ok = false;
    try { f.display(); ok = true; } catch (...) {}
    check(ok, "display() does not throw");

    delete c;
}

static void testDrinkItem() {
    std::cout << "\n--- DrinkItem ---\n";
    DrinkItem d("Red Wine", 7.50, 200, 13.0);

    check(d.getName()           == "Red Wine", "getName()");
    check(d.getPrice()          == 7.50,       "getPrice()");
    check(d.getVolumeMl()       == 200,        "getVolumeMl()");
    check(d.getAlcoholPercent() == 13.0,       "getAlcoholPercent()");

    MenuItem* c = d.clone();
    check(c != nullptr,                 "clone() not null");
    check(c->getName() == "Red Wine",   "clone keeps name");
    delete c;

    bool ok = false;
    try { d.display(); ok = true; } catch (...) {}
    check(ok, "display() does not throw");
}

static void testMenuCRUD() {
    std::cout << "\n--- Menu CRUD ---\n";
    Menu m;
    check(m.size() == 0, "empty menu has size 0");

    m.add(new FoodItem ("Burger", 12.00, "Beef, Bun, Lettuce"));
    m.add(new DrinkItem("Cola",    2.50, 330, 0.0));
    check(m.size() == 2, "size after two adds");

    check(m.get("Burger") != nullptr, "get() finds existing item");
    check(m.get("Cola")   != nullptr, "get() finds second item");
    check(m.get("Pizza")  == nullptr, "get() returns nullptr for missing item");

    m.update("Burger", 13.00);
    check(m.get("Burger")->getPrice() == 13.00, "update() changes price");

    m.remove("Cola");
    check(m.size() == 1,            "size shrinks after remove");
    check(m.get("Cola") == nullptr, "removed item is gone");
}

static void testDeepCopy() {
    std::cout << "\n--- Deep copy ---\n";
    Menu original;
    original.add(new FoodItem("Pizza", 11.00, "Dough, Tomato"));

    Menu copy = original;
    check(copy.size() == original.size(), "copy starts with same size");

    copy.update("Pizza", 99.00);
    check(original.get("Pizza")->getPrice() == 11.00,
          "modifying copy does NOT modify original (deep copy)");

    copy.add(new DrinkItem("Beer", 5.00, 500, 5.0));
    check(copy.size() == 2 && original.size() == 1,
          "sizes diverge after copy is modified");
}

static void testStrategy() {
    std::cout << "\n--- Strategy ---\n";
    Menu m;
    m.add(new FoodItem("Zucchini Fritters", 8.00, "Zucchini, Egg, Flour"));
    m.add(new FoodItem("Apple Tart",        6.50, "Apple, Pastry, Sugar"));
    m.add(new FoodItem("Mushroom Soup",     5.00, "Mushroom, Cream, Onion"));

    bool threw = false;
    try { m.sort(); } catch (const StrategyNotSet&) { threw = true; }
    check(threw, "sort() throws StrategyNotSet when strategy is unset");

    SortByPrice byPrice;
    m.setSortStrategy(&byPrice);
    m.sort();
    auto it = m.begin();
    double p1 = (*it)->getPrice(); ++it;
    double p2 = (*it)->getPrice(); ++it;
    double p3 = (*it)->getPrice();
    check(p1 <= p2 && p2 <= p3, "SortByPrice produces ascending order");

    SortByName byName;
    m.setSortStrategy(&byName);
    m.sort();
    it = m.begin();
    std::string n1 = (*it)->getName(); ++it;
    std::string n2 = (*it)->getName(); ++it;
    std::string n3 = (*it)->getName();
    check(n1 <= n2 && n2 <= n3, "SortByName produces alphabetical order");
}

static void testIterator() {
    std::cout << "\n--- ForwardIterator ---\n";
    Menu m;
    m.add(new FoodItem("A", 1.00, "x"));
    m.add(new FoodItem("B", 2.00, "x"));
    m.add(new FoodItem("C", 3.00, "x"));

    int count = 0;
    for (auto it = m.begin(); it != m.end(); ++it) ++count;
    check(count == 3, "iterator traverses every item");

    int cheap = (int)std::count_if(m.begin(), m.end(), [](MenuItem* item) {
        return item->getPrice() < 2.5;
    });
    check(cheap == 2, "std::count_if works with ForwardIterator");

    auto it   = m.begin();
    auto prev = it++;
    check((*prev)->getName() == "A", "post-increment returns previous position");
    check((*it)->getName()   == "B", "post-increment advances iterator");
}

static void testCallback() {
    std::cout << "\n--- Callback ---\n";
    Menu m;
    m.add(new FoodItem ("Steak", 25.00, "Beef, Salt, Pepper"));
    m.add(new DrinkItem("Water",  2.00, 330, 0.0));

    m.applyToAll([](MenuItem* item) {
        item->setPrice(item->getPrice() / 2.0);
    });
    check(m.get("Steak")->getPrice() == 12.50, "callback halved Steak");
    check(m.get("Water")->getPrice() == 1.00,  "callback halved Water");

    int expensive = 0;
    m.applyToAll([&expensive](MenuItem* item) {
        if (item->getPrice() > 10.00) ++expensive;
    });
    check(expensive == 1, "callback counts items above $10");
}

static void testBaseDisplayThrows() {
    std::cout << "\n--- MenuItem::display() throws NotImplementedException ---\n";

    // PlaceholderItem inherits MenuItem::display() (which throws) by not overriding it.
    class PlaceholderItem : public MenuItem {
    public:
        PlaceholderItem(const std::string& n, double p) : MenuItem(n, p) {}
        MenuItem* clone() const override { return new PlaceholderItem(*this); }
    };

    PlaceholderItem item("Placeholder", 5.00);

    bool threw = false;
    try { item.display(); } catch (const NotImplementedException&) { threw = true; }
    check(threw, "MenuItem::display() throws when not overridden");

    MenuItem* base = &item;
    MenuItem* c = base->clone();
    check(c != nullptr,                  "polymorphic clone() works");
    check(c->getName() == "Placeholder", "clone keeps name");
    delete c;
}

// Aspirational tests for the full SpecialItem implementation.
// Compile with -DSPECIAL_ITEM_IMPLEMENTED once SpecialItem is implemented.
#ifdef SPECIAL_ITEM_IMPLEMENTED

static void testSpecialItemFullImplementation() {
    std::cout << "\n--- SpecialItem (full implementation) ---\n";

    SpecialItem s("Salmon Steak", 18.50, "Pan-seared with lemon butter",
                  true, 3, "Food");

    bool threw = false;
    try { s.display(); } catch (...) { threw = true; }
    check(!threw, "full display() does not throw");

    check(s.isAvailableToday(),     "isAvailableToday() returns true initially");
    check(s.getQuantityLeft() == 3, "getQuantityLeft() returns 3");
    check(s.getCategory() == "Food","getCategory() returns 'Food'");

    MenuItem* c = s.clone();
    check(c != nullptr,                                    "SpecialItem::clone() not null");
    check(c->getName() == "Salmon Steak",                  "clone keeps name");
    SpecialItem* sc = dynamic_cast<SpecialItem*>(c);
    check(sc != nullptr,                                   "dynamic_cast<SpecialItem*> on clone");
    check(sc->getSpecialDescription()
            == "Pan-seared with lemon butter",             "getSpecialDescription()");
    delete c;

    s.orderOne();
    check(s.getQuantityLeft() == 2, "orderOne() decrements quantity");

    s.orderOne();
    s.orderOne();
    check(!s.isAvailableToday(),    "becomes unavailable once sold out");

    Menu m;
    m.add(new SpecialItem("Daily Special", 15.00, "Ask the waiter", true, -1, "Food"));
    m.add(new FoodItem("Pasta", 10.00, "Wheat, Tomato"));
    SortByPrice byPrice;
    m.setSortStrategy(&byPrice);
    m.sort();
    check((*m.begin())->getPrice() == 10.00,
          "SpecialItem is sorted correctly alongside FoodItem");
}

#endif

int main() {
    std::cout << "*** Restaurant Menu Manager — Test Suite ***\n";

    testFoodItem();
    testDrinkItem();
    testMenuCRUD();
    testDeepCopy();
    testStrategy();
    testIterator();
    testCallback();
    testBaseDisplayThrows();

#ifdef SPECIAL_ITEM_IMPLEMENTED
    testSpecialItemFullImplementation();
#else
    std::cout << "\n(Skipping full-SpecialItem tests — define "
                 "SPECIAL_ITEM_IMPLEMENTED to enable.)\n";
#endif

    std::cout << "\n=== Results: " << passed << " passed, "
              << failed << " failed ===\n";
    return failed == 0 ? 0 : 1;
}
