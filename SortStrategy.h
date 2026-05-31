#pragma once
#include <vector>
#include "MenuItem.h"

/// Strategy pattern: pluggable sort orders for Menu.
/// Add a new ordering by deriving from SortStrategy and implementing sort().
class SortStrategy {
public:
    virtual ~SortStrategy() = default;
    virtual void sort(std::vector<MenuItem*>& items) const = 0;
};

class SortByPrice : public SortStrategy {
public:
    void sort(std::vector<MenuItem*>& items) const override;
};

class SortByName : public SortStrategy {
public:
    void sort(std::vector<MenuItem*>& items) const override;
};
