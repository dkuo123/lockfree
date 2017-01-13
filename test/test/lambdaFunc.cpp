#include <algorithm>
#include <memory>
#include <iostream>

struct iStringCallable {
   virtual void operator() (char *) = 0;
};

template <typename T>
class StringFunctionAdapter : public iStringCallable {
   T obj;
public:
   StringFunctionAdapter(T obj_) : obj(std::move(obj_)) { }
   void operator() (char *arg) override {
      obj(arg);
   }
};

class StringFunction {
   std::shared_ptr<iStringCallable> func;
   // Extra points for changing this back to unique_ptr, and
   // implementing copy semantics that copy the underlying
   // lambda/callable object.

public:
   StringFunction() = default;
   template <typename LambdaType>
   StringFunction(LambdaType l)
   : func(new StringFunctionAdapter<LambdaType>(std::move(l))) { }

   void operator() (char *arg) {
      (*func)(arg);
   }
};

struct PrintArgument {
   std::string prefix;
   void operator()(char *val) {
      std::cout << prefix << val << std::endl;
   }
};

typedef void (*char_func)(char *);

int main(int argc, char **argv) {
   int i = 0;

   char_func f = [] (char *val) { };

   auto func = [&i] (char *val) {
      std::cout << i << ": " << val << std::endl;
      ++i;
   };
   PrintArgument arg;
   arg.prefix = "Hi: ";
   StringFunction func2 (func);
   StringFunction func3 (arg);
   std::for_each(argv, argv + argc, func2);
   std::for_each(argv, argv + argc, func3);
   return i;
}
