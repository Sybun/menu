/*
 * SpecialItem — implementation description.
 * This file intentionally contains no code; implementing the class is left
 * to a future contributor.  Until then SpecialItem cannot be instantiated,
 * but it is declared in MenuItem.h so aspirational tests can reference it.
 *
 * Purpose
 *   Items that are not regular FoodItem or DrinkItem: chef's daily special,
 *   seasonal dishes, limited-availability items.
 *
 * Fields to add (private section in MenuItem.h)
 *   std::string description     promotional one-liner (already declared)
 *   bool        isAvailable     currently being served
 *   int         quantityLeft    portions left; -1 means unlimited
 *   std::string category        "Food" | "Drink" | "Dessert" | ...
 *
 * Extended constructor
 *   SpecialItem(const std::string& name, double price,
 *               const std::string& description,
 *               bool               isAvailable  = true,
 *               int                quantityLeft = -1,
 *               const std::string& category     = "Food");
 *
 * Methods to implement
 *   MenuItem*   clone()   const override;    // return new SpecialItem(*this)
 *   void        display() const override;    // print SPECIAL banner + fields;
 *                                            // show SOLD OUT if !isAvailable
 *   std::string getSpecialDescription() const;
 *   bool        isAvailableToday() const;    // returns isAvailable
 *   void        orderOne();                  // quantityLeft--; if 0 -> isAvailable=false
 *   int         getQuantityLeft() const;
 *   std::string getCategory()     const;
 *
 * Suggested display() output
 *   [SPECIAL] Chef's Salmon   $18.50   *** TODAY'S SPECIAL ***
 *             Pan-seared with lemon butter
 *             Available: Yes  |  Remaining: 5 portions
 *
 * Notes
 *   - clone() must copy ALL fields.
 *   - SpecialItem already works with Menu's sort strategies because they only
 *     call getPrice() / getName().
 *   - Acceptance criteria live in test.cpp behind #ifdef SPECIAL_ITEM_IMPLEMENTED.
 */
