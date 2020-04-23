#include <iostream> // std::cout
#include <typeinfo>

template <bool B, typename T = void> struct enable_if {};

template <typename T> struct enable_if<true, T> { typedef T type; };

template <typename T>
typename enable_if<std::is_integral<T>::value>::type  calculate( T a) {}


template <typename T>
void  sum(T a,typename enable_if<std::is_integral<T>::value,T>::type t = 0) {
}

// 经由模板形参启用 A 的偏特化
template <class T, class Enable = void> class A {}; // 初等模板
                                                    //
template <class T>
class A<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
}; // 浮点类型的特化

int main() {
  // calculate<int>(1);
  // calculate<double>(1.0);
  A<int>{};    // OK ：匹配初等模板
  A<double>{}; // OK ：匹配偏特化+
  // calculate<double>(3.0); //error
  calculate<int>(3); //ok
  sum<int>(3); // ok
  // sum<double>(3); // error
  return 0;
}
