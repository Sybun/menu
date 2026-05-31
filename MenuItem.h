//AI tools by Anthropic were used in the creation of this project


#pragma once
#include <string>
#include <stdexcept>

class NotImplementedException : public std::logic_error {
public:
    NotImplementedException() : std::logic_error("Method not implemented") {}
};

class StrategyNotSet : public std::runtime_error {
public:
    StrategyNotSet() : std::runtime_error("Sort strategy not set") {}
};

/// Abstract base class for any item on the restaurant menu.
/// display() is the polymorphic method overridden by each subclass.
/// clone() implements the Prototype pattern for polymorphic copying.
class MenuItem {
public:
    MenuItem(const std::string& name, double price);
    virtual ~MenuItem() = default;

    virtual MenuItem* clone() const = 0;
    virtual void display() const;   // base version throws NotImplementedException

    std::string getName()  const;
    double      getPrice() const;
    void        setPrice(double newPrice);

    bool operator<(const MenuItem& other) const;

protected:
    std::string name;
    double      price;
};

/// A food dish with an ingredient list.
class FoodItem : public MenuItem {
public:
    FoodItem(const std::string& name, double price, const std::string& ingredients);

    MenuItem* clone()   const override;
    void      display() const override;

    std::string getIngredients() const;

private:
    std::string ingredients;
};

/// A beverage with volume (ml) and alcohol percentage.
class DrinkItem : public MenuItem {
public:
    DrinkItem(const std::string& name, double price, int volumeMl, double alcoholPercent);

    MenuItem* clone()   const override;
    void      display() const override;

    int    getVolumeMl()       const;
    double getAlcoholPercent() const;

private:
    int    volumeMl;
    double alcoholPercent;
};

/// Chef's specials, seasonal dishes, limited-availability items.
/// DECLARED here only — see SpecialItem.cpp for the implementation description.
class SpecialItem : public MenuItem {
public:
    SpecialItem(const std::string& name, double price, const std::string& description);

    MenuItem* clone()   const override;
    void      display() const override;

    std::string getSpecialDescription() const;

private:
    std::string description;
};
