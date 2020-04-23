#include <future>   // std::packaged_task, std::future
#include <iostream> // std::cout
#include <thread>   // std::thread
#include <utility>  // std::move

// a simple task:
int triple(int x) { return x * 3; }

int main() {
  // std::packaged_task<int(int)> tsk(triple); // package task

  // std::future<int> fut = tsk.get_future();
  // std::thread td1(std::move(tsk), 100);
  // std::cout << "The triple of 100 is " << fut.get() << ".\n";

 // td1.join();
  // // re-use same task object:
  // tsk.reset();
  // fut = tsk.get_future();
  // std::thread(std::move(tsk), 200).detach();
  // std::cout << "Thre triple of 200 is " << fut.get() << ".\n";

  return 0;
}
