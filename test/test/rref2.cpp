#include <iostream>
#include <string>
#include <vector>

struct Logging {
   std::string s;

   void log_message(const char *msg) {
      std::cerr << msg << ": " << s << std::endl;
   }

   Logging() {
      log_message("Default constructor");
   }

   Logging(const char *c) : s(c) {
     log_message("Implicit conversion from const char *");
   }

   Logging(const Logging &l) : s(l.s) {
      log_message("Copy constructor");
   }

   Logging(Logging &&l) noexcept : s(std::move(l.s)) {
      log_message("Move constructor");
   }

   Logging &operator=(Logging log) noexcept {
      s = std::move(log.s);
      log_message("Assignment");
      return *this;
   }

   ~Logging() {
      log_message("Destructor");
   }
};

int main() {
   {
      std::cout << "Example #1" << std::endl;
      std::vector<Logging> v;
      v.push_back("hi");
   }
   {
      std::cout << "Example #2" << std::endl;
      std::vector<Logging> v;
      v.emplace_back("hi");
   }
   {
      std::cout << "Example #3 setup" << std::endl;
      std::vector<Logging> v;
      v.reserve(10);
      v.push_back("obj0");
      v.emplace_back("obj1");
      v.emplace_back("obj2");
      std::cout << "Example #3 proper" << std::endl;
      v.emplace(v.begin(), "obj-to-insert");
      std::cout << "Example #3 clean-up" << std::endl;
   }
}
