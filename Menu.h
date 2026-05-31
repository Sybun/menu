#pragma once
#include <functional>
#include <memory>
#include <vector>
#include "MenuItem.h"
#include "SortStrategy.h"

struct MenuImpl;  // pimpl — defined in Menu.cpp

/// Container of MenuItem* (the Restaurant Menu Manager).
/// Owns every item it stores.  State is hidden via pimpl (MenuImpl).
/// Provides CRUD, Strategy-pattern sorting, callbacks, and a ForwardIterator.
class Menu {
public:
    Menu();
    Menu(const Menu& other);
    Menu& operator=(const Menu& other);
    ~Menu();

    void      add(MenuItem* item);                  // Menu takes ownership
    void      remove(const std::string& name);
    MenuItem* get(const std::string& name) const;
    void      update(const std::string& name, double newPrice);
    int       size() const;

    void setSortStrategy(SortStrategy* strategy);
    void sort();   // throws StrategyNotSet if no strategy is set

    void applyToAll(std::function<void(MenuItem*)> callback);
    void displayAll() const;

    class ForwardIterator {
    public:
        using value_type        = MenuItem*;
        using difference_type   = std::ptrdiff_t;
        using pointer           = MenuItem**;
        using reference         = MenuItem*&;
        using iterator_category = std::forward_iterator_tag;

        ForwardIterator();
        explicit ForwardIterator(std::vector<MenuItem*>::iterator it);

        reference        operator*();
        pointer          operator->();
        ForwardIterator& operator++();
        ForwardIterator  operator++(int);
        bool operator==(const ForwardIterator& other) const;
        bool operator!=(const ForwardIterator& other) const;

    private:
        std::vector<MenuItem*>::iterator it;
    };

    ForwardIterator begin();
    ForwardIterator end();

private:
    std::unique_ptr<MenuImpl> impl;
};
