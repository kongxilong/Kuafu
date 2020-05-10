#include <iostream>
#include <memory>
using namespace std;

/// forward (as per N3143)
template <typename _Tp>
inline _Tp &&
_forward(typename std::remove_reference<_Tp>::type &__t) {
  cout<< "forward one" << endl;
  if(std::is_rvalue_reference<_Tp>::value){
    cout << "forward one _Tp is_rvalue_reference" << endl;
  }else if(std::is_lvalue_reference<_Tp>::value){
    cout << "forward one _Tp is_lvalue_reference" << endl;
  }
  return static_cast<_Tp &&>(__t);
}

template <typename _Tp>
inline _Tp &&
_forward(typename std::remove_reference<_Tp>::type &&__t) {
  cout<< "forward two" << endl;
  if(std::is_rvalue_reference<_Tp>::value){
    cout << "forward two _Tp is_rvalue_reference" << endl;
  }else if(std::is_lvalue_reference<_Tp>::value){
    cout << "forward two _Tp is_lvalue_reference" << endl;
  }
  static_assert(!std::is_lvalue_reference<_Tp>::value,
                "template argument"
                " substituting _Tp is an lvalue reference type");
  return static_cast<_Tp &&>(__t);
}

struct X {};
void inner(const X &) { cout << "inner(const X&)" << endl; }
void inner(X &&) { cout << "inner(X&&)" << endl; }
template <typename T> void outer(T &&t) { inner(_forward<T>(t)); }

int main() {
  X a;
  // outer(a);
  // outer(X());
  // inner(_forward<X>(X()));

  // string s1 = "123";
  // string s2(move(s1));
  // cout << "s1 = " << s1 << endl; //s1 = ""
  // cout << " s2 = " << s2 << endl;  //s2 = "123
  
  // shared_ptr<int> s1(new int(1));
  // shared_ptr<int> s2(move(s1));
  // cout << "s1 use_count = " << s1.use_count() << " s2 = " << s2.use_count() << endl;  //s1 count = 0 s2 count = 1
  

  shared_ptr<int> s1(new int(1));
  shared_ptr<int> s2(_forward<shared_ptr<int>>(s1));
  cout << "s1 use_count = " << s1.use_count() << " test s2 use_count = " << s2.use_count() << endl;  //s1 count = 0 s2 count = 1
}
