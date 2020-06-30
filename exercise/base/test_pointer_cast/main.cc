#include <iostream>
using namespace std;
class A {
public:
  virtual ~A(void) { }

private:
  int a;
};

class C : public A {
public:
  C(void) = default;
  ~C(void) = default;

private:
  int c;
};
int main(void) {
  {
    A *p = new C; //指向基类的指针p，基类的析构函数不会调用子类的析构函数
    cout << "p = " << p << endl;
    cout << "dynamic_cast<C*>(p) = " << dynamic_cast<C*>(p) << endl;
    delete p; //当A的析构函数为虚函数时 delete一个指向子类对象的基类指针，资源能够正常释放
  }
  return 0;
}
