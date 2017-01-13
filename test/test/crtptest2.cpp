#include <iostream>
#include <vector>
#include <memory>

class VFormatter {
public:
  virtual void print(const std::string &something) = 0;
  void printTwice(const std::string &something) {
    print(something);
    print(something);
  }
  virtual ~VFormatter() = default;
};

class VStdOutFormatter : public VFormatter {
  void print(const std::string &something) override {
    std::cout << something << std::endl;
  }
};

class VStdErrFormatter : public VFormatter {
  void print(const std::string &something) override {
    std::cerr << something << std::endl;
  }
};

template <typename Derived> class Formatter {
  Derived &GetDerived() { return static_cast<Derived &>(*this); }

public:
  void printTwice(const std::string &something) {
    GetDerived().print(something);
    GetDerived().print(something);
  }
};
#if 0 // When is an incomplete type allowed?
class Foo;

Foo *pointer;
std::unique_ptr<Foo> ptr;

int foo() {
   std::unique_ptr<Foo> ptr = std::make_unique<Foo>();
   return 0;
}
#endif
class StdOutFormatter : public Formatter<StdOutFormatter> {
public:
  void print(const std::string &something) {
    std::cout << something << std::endl;
    print2(); // allowed to call forward
  }

  void print2() {}
};

class StdErrFormatter : public Formatter<StdErrFormatter> {
public:
  void print(const std::string &something) {
    std::cerr << something << std::endl;
  }
};

struct Foo {
  int a;
  int b;
  template <typename OutStream> OutStream &print(OutStream &out) {
    return out << a << ", " << b;
  }
};

std::ostream &operator<<(std::ostream &out, Foo foo) { return foo.print(out); }

template <typename OutStream> OutStream &operator<<(OutStream &out, Foo foo) {
  return foo.print(out);
}
// Using =delete to enforce strict type in template
template <typename T> class StrictType {
private:
  T value{};

public:
  StrictType() = default;
  template <class U> StrictType(U u) = delete;
  StrictType(T v) : value(v) {}
  operator T() const { return value; }
};
uint64_t netToFromHost64(StrictType<uint64_t> value) { return value; }

int main() {
  (StdOutFormatter{}).printTwice("Hi");
  std::vector<std::unique_ptr<VFormatter>> formatters;
  formatters.emplace_back(new VStdOutFormatter);
  formatters.emplace_back(new VStdErrFormatter);
  for (auto &ptr : formatters) {
    ptr->printTwice("Hello.");
  }
  return 0;
}
