#include <iostream>
#include <typeinfo>
#include <memory>
#include <string>

// A way of implementing python-like class methods in C++
// not sure of the usefulness as the main use case in python is for multiple constructors, which C++ supports anyway

class Base
{
public:
  Base(int i) : i(i) { }

  static int static_method() { return 0; }

  template<typename Cls>
  static std::shared_ptr<Base> class_method(int i)
  {
    // assert Cls is a Base or subclass thereof
    static_assert(std::is_base_of_v<Base, Cls>, "Cls must be a subclass of Base");
    return std::make_shared<Cls>(i);
  };

  virtual std::string repr() const
  {
    return "Base " + std::to_string(i);
  }

protected:
  int i;
};

std::ostream& operator<<(std::ostream& s, const std::shared_ptr<Base> b)
{
  s << b->repr();
  return s;
}

class Derived0: public Base
{
public:
  Derived0(int i) : Base(i+10) { }

  virtual std::string repr() const
  {
    return "Derived0 " + std::to_string(i);
  }

};

class Derived1: public Base
{
public:
  Derived1(int i) : Base(i+20) { }

  virtual std::string repr() const
  {
    return "Derived1 " + std::to_string(i);
  }

};

class NotDerived {};


int main()
{
  std::cout << Base::class_method<Base>(0) << std::endl;
  std::cout << Derived0::class_method<Base>(1) << std::endl;
  std::cout << Base::class_method<Derived0>(1) << std::endl;
  std::cout << Derived0::class_method<Derived0>(2) << std::endl;
  std::cout << Base::class_method<Derived1>(1) << std::endl;
  std::cout << Derived0::class_method<Derived1>(2) << std::endl;
  //std::cout << Derived::class_method<NotDerived>(2) << std::endl;
}

