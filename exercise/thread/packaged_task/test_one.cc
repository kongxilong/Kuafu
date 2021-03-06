#include <future>   // std::packaged_task, std::future
#include <iostream> // std::cout
#include <thread>   // std::thread
#include <utility>  // std::move
#include <chrono>  // std::move

int main() {
  std::packaged_task<int(int)> foo; // 默认构造函数.

  // 使用 lambda 表达式初始化一个 packaged_task 对象.
  std::packaged_task<int(int)> bar([](int x) { 
    std::cout << "sleep now" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    return x * 2; });

  foo = std::move(bar); // move-赋值操作，也是 C++11 中的新特性.

  // 获取与 packaged_task 共享状态相关联的 future 对象.
  std::future<int> ret = foo.get_future();

  std::thread(std::move(foo), 10).detach(); // 产生线程，调用被包装的任务.

  int value = ret.get(); // 等待任务完成并获取结果.
  std::cout << "The double of 10 is " << value << ".\n";

  return 0;
}
