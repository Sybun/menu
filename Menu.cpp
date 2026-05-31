#include "Menu.h"
#include <algorithm>

struct MenuImpl {
    std::vector<MenuItem*> items;
    SortStrategy*          strategy = nullptr;
};

Menu::Menu() : impl(new MenuImpl()) {}

Menu::Menu(const Menu& other) : impl(new MenuImpl()) {
    impl->strategy = other.impl->strategy;
    for (MenuItem* item : other.impl->items) {
        impl->items.push_back(item->clone());
    }
}

Menu& Menu::operator=(const Menu& other) {
    if (this == &other) return *this;
    for (MenuItem* item : impl->items) delete item;
    impl->items.clear();
    impl->strategy = other.impl->strategy;
    for (MenuItem* item : other.impl->items) {
        impl->items.push_back(item->clone());
    }
    return *this;
}

Menu::~Menu() {
    for (MenuItem* item : impl->items) delete item;
}

void Menu::add(MenuItem* item) {
    impl->items.push_back(item);
}

void Menu::remove(const std::string& name) {
    auto it = std::find_if(impl->items.begin(), impl->items.end(),
                           [&](MenuItem* m) { return m->getName() == name; });
    if (it != impl->items.end()) {
        delete *it;
        impl->items.erase(it);
    }
}

MenuItem* Menu::get(const std::string& name) const {
    for (MenuItem* item : impl->items) {
        if (item->getName() == name) return item;
    }
    return nullptr;
}

void Menu::update(const std::string& name, double newPrice) {
    MenuItem* item = get(name);
    if (item) item->setPrice(newPrice);
}

int Menu::size() const {
    return static_cast<int>(impl->items.size());
}

void Menu::setSortStrategy(SortStrategy* strategy) {
    impl->strategy = strategy;
}

void Menu::sort() {
    if (!impl->strategy) throw StrategyNotSet();
    impl->strategy->sort(impl->items);
}

void Menu::applyToAll(std::function<void(MenuItem*)> callback) {
    for (MenuItem* item : impl->items) callback(item);
}

void Menu::displayAll() const {
    for (MenuItem* item : impl->items) item->display();
}

Menu::ForwardIterator::ForwardIterator() : it() {}

Menu::ForwardIterator::ForwardIterator(std::vector<MenuItem*>::iterator it)
    : it(it) {}

Menu::ForwardIterator::reference Menu::ForwardIterator::operator*()  { return *it; }
Menu::ForwardIterator::pointer   Menu::ForwardIterator::operator->() { return &(*it); }

Menu::ForwardIterator& Menu::ForwardIterator::operator++() {
    ++it;
    return *this;
}

Menu::ForwardIterator Menu::ForwardIterator::operator++(int) {
    ForwardIterator tmp(*this);
    ++it;
    return tmp;
}

bool Menu::ForwardIterator::operator==(const ForwardIterator& other) const {
    return it == other.it;
}
bool Menu::ForwardIterator::operator!=(const ForwardIterator& other) const {
    return it != other.it;
}

Menu::ForwardIterator Menu::begin() { return ForwardIterator(impl->items.begin()); }
Menu::ForwardIterator Menu::end()   { return ForwardIterator(impl->items.end());   }
