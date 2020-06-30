#include <iostream>
#include <string>
using namespace std;

template<typename T>
auto add(T a, T b) ->decltype(a+b)
{
  cout << "run template add" << endl;
  return a +b ;
}

double add(double a, double b){
  cout << "run simple add" << endl;
 return a + b;
}

int main(void) {
  add(1,2); //run template add
  add(string("a"), string("b")); //run template add
  add(1.0,2.0);  //run simple add
  add(1,2.0); //run simple add
  return 0;
}
