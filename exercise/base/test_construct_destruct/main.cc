#include <iostream>
using namespace std;
class A {
public:
  A(void) { cout << "A构造" << endl; }
  virtual ~A(void) { cout << "A析构" << endl; }
};
class B {
public:
  B(void) { cout << "B构造" << endl; }
  ~B(void) { cout << "B析构" << endl; }
};
class C : public A {
public:
  C(void) { cout << "C构造" << endl; }
  ~C(void) { cout << "C析构" << endl; }

private:
  B m_b;
};
int main(void) {
  // test one
  // {
    // C c;
    // A *p = new C; //指向基类的指针p，基类的析构函数不会调用子类的析构函数
    // delete p; // delete一个指向子类对象的基类指针，将可能引发内存泄漏
  // }
  {
    //test two 当A的析构函数为虚函数时
    C c;
    A *p = new C; //指向基类的指针p，基类的析构函数不会调用子类的析构函数
    delete p; //当A的析构函数为虚函数时 delete一个指向子类对象的基类指针，资源能够正常释放
  }
  return 0;
}
