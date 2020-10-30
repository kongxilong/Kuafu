#include <iostream>
#include <vector>

using namespace std;

vector<int> vi;

int sum(int n){
  int total = 0;
  for(int i = 1; i <= n ; i++){
    total += i;
  }
  return total;
}

int main(void) {
  for(int i = 0; i < 100; i++){
  vi.push_back(i);
  }

  volatile int a[10];
  for(int i = 0; i < 10; i++){
   a[i] = i;
  }
  (void)a;
  volatile int *b = new int[10];
  for(int i = 0; i < 10; i++){
   b[i] = i;
  }
  (void)b;

  {
    sum(100);
  }
  return 0;
}
