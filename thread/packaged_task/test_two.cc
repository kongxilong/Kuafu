#include <future>   // std::packaged_task, std::future
#include <iostream> // std::cout
#include <thread>   // std::thread
#include <utility>  // std::move
#include <chrono>  // std::move

// 在新线程中启动一个 int(int) packaged_task.
std::future<int> launcher(std::packaged_task<int(int)> &tsk, int arg) {
  std::this_thread::sleep_for(std::chrono::seconds(10));
  if (tsk.valid()) {
    std::future<int> ret = tsk.get_future();
    std::thread(std::move(tsk), arg).detach();
    return ret;
  } else{
    std::cout << "tsk isn't valid." << std::endl;
    return std::future<int>();
  }
}

int main() {
  // std::packaged_task<int(int)> tsk([](int x) { return x * 2; });
  std::packaged_task<int(int)> tsk;

  std::future<int> fut = launcher(tsk, 25);

  // fut.wait();
  if(fut.valid()){
    std::cout << "The double of 25 is " << fut.get() << ".\n";
  } else{
    std::cout << "fut isn't valid" <<'\n';
  }

  return 0;
}
