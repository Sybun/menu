#include "SortStrategy.h"
#include <algorithm>

void SortByPrice::sort(std::vector<MenuItem*>& items) const {
    std::sort(items.begin(), items.end(),
              [](const MenuItem* a, const MenuItem* b) {
                  return a->getPrice() < b->getPrice();
              });
}

void SortByName::sort(std::vector<MenuItem*>& items) const {
    std::sort(items.begin(), items.end(),
              [](const MenuItem* a, const MenuItem* b) {
                  return a->getName() < b->getName();
              });
}
