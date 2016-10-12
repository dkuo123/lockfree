// cont/refsem1.cpp
#include <iostream>
#include <string>
#include <set>
#include <deque>
#include <algorithm>
#include <memory>
#include <vector>

class Item {
private:
  std::string name;
  float price;
public:
  Item (const std::string& n, float p = 0) : name(n), price(p) {
  }
  std::string getName () const {
    return name;
  }

  void setName (const std::string& n) {
    name = n;
  }
  float getPrice () const {
    return price;
  }
  float setPrice (float p) {
    price = p;
  }
};

main() {
  //std::vector<Item&> book1s; // wouldn't compile
  std::vector<std::reference_wrapper<Item>> books; // elements are references
  Item f("Faust",12.99);
  books.push_back(f); // insert book by reference
  // print books:
  for (const auto& book : books) {
    std::cout << book.get().getName() << ": "
	      << book.get().getPrice() << std::endl; //12.99
  }
  f.setPrice(9.99); // modify book outside the containers
  std::cout << books[0].get().getPrice() << std::endl; // print price of first book 9.99
  // print books using type of the elements (no get() necessary):
  for (const Item& book : books) {
    std::cout << book.getName() << ": " << book.getPrice() << std::endl; // 9.99
  }
}
