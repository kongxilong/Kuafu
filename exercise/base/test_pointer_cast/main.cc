#include <iostream>
using namespace std;
class A {
public:
  virtual void test(){}
  virtual ~A(void) { }

  int a;
  int b;
};

class B  {
public:
  virtual void testA(){
    int i = 1; 
  int j = 1;
  int sum = i +j;
  (void)sum;
  }
};

class C : public B, public A {
public:
  C(void) = default;
  ~C(void) = default;

  virtual void test(){ int j = 0; }
  int c;
};
int main(void) {
  // reinterpret_cast 指针类型都能相互转，只是二进制的数据解释
  // ，他不进行指针的地址改变。
  // static_cast 会进行数据指针的转变，用于从子类向父类指针的转换。
  // 但是从父类强转子类的时候，可能会错误。这个要看指针。
  // dynamic_cast runtime安全检查，父类和子类转换都可以。
  // 从其他不相关类型转换到特定指针时，dynamic_cast和static_cast直接编译报错。
  // reinterpret_cast 可以转换任意类型的指针。但不确保正确。
  C* pc = new C;
  cout << "pc = " << pc << endl; // pc = 0x55e81fb9ee70
  A* pa = static_cast<A*>(pc);
  cout << "static_cast pa = " << pa << endl; // static_cast pa = 0x55e81fb9ee78 注意有位移
  pa = reinterpret_cast<A*>(pc);
  cout << "reinterpret_cast pa = " << pa << endl; // reinterpret_cast pa = 0x55e81fb9ee70
  pa = dynamic_cast<A*>(pc);
  cout << "dynamic_cast pa = " << pa << endl; //dynamic_cast pa = 0x55e81fb9ee78  有位移


  B* pb = static_cast<B*>(pc);
  cout << "static_cast pb = " << pb << endl; //static_cast pb = 0x55e81fb9ee70
  pb = reinterpret_cast<B*>(pc);
  cout << "reinterpret_cast pb = " << pb << endl; // reinterpret_cast pb = 0x55e81fb9ee70
  pb = dynamic_cast<B*>(pc);
  cout << "dynamic_cast pb = " << pb << endl;  // dynamic_cast pb = 0x55e81fb9ee70

  A* ppa = new A;
  cout << "origin ppa = " << ppa << endl; // origin ppa = 0x55e81fb9f2b0
  C* ppc = static_cast<C*>(ppa);
  cout << "static_cast ppc = " << ppc << endl; // static_cast ppc = 0x55e81fb9f2a8  有位移 但是不正确
  ppc = reinterpret_cast<C*>(ppa);
  cout << "reinterpret_cast ppc = " << ppc << endl; // reinterpret_cast ppc = 0x55e81fb9f2b0
  ppc = dynamic_cast<C*>(ppa);
  cout << "dynamic_cast ppc = " << ppc << endl; // dynamic_cast ppc = 0 正确校验为0
  return 0;
}
