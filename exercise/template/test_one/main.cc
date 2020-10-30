#include <iostream>
#include <string>
#include <typeinfo>
using namespace std;

template<typename T>
auto add(T a, T b) ->decltype(a+b)
{
  typedef T element_type;
  cout << "run template add, typeinfo = " << typeid(element_type).name() << endl;
  return a +b ;
}

double add(double a, double b){
  cout << "run simple add" << endl;
 return a + b;
}

int main(void) {
  cout << "typeid(int) = " << typeid(int).name() << endl;
  add(1,2); //run template add
  add(string("a"), string("b")); //run template add
  add(1.0,2.0);  //run simple add
  add(1,2.0); //run simple add
  return 0;
}
