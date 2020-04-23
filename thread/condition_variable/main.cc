#include <condition_variable> // std::condition_variable
#include <iostream>           // std::cout
#include <mutex>              // std::mutex, std::unique_lock
#include <thread>             // std::thread

std::mutex mtx;             // 全局互斥锁.
std::condition_variable cv; // 全局条件变量.
int cargo = 0;              // 全局标志位.
int count = 0;

bool has_cargo() { return cargo > 0; }

void do_print_id(int id) {
  while (count < 100) {
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck,[]{return has_cargo();});
    // while (!has_cargo()) // 如果标志位不为 true, 则等待...
    // {
      // // std::cout << " before wait id " << id << " owns_lock = " <<
      // // lck.owns_lock()
      // // << std::endl;
      // cv.wait(lck); // 当前线程被阻塞, 当全局标志位变为 true 之后,
      // // std::cout << " after wait id = " << id << " owns_lock = " <<
      // // lck.owns_lock()
      // // << std::endl;
    // }
    // 线程被唤醒, 继续往下执行打印线程编号id.
    // std::cout << " out of wait cargo = " << cargo
    // << " owns_lock = " << lck.owns_lock() << std::endl;
    cargo--;
    std::cout << "thread " << id << " running. cargo = " << cargo << '\n';
    // std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  std::cout << "thread id = " << id << "exit now." << std::endl;
}

void go() {
  for (int i = 0; i < 120; i++) {
    // while (has_cargo()) {
      // std::cout << "stop going,entry yield cargo = " << cargo << std::endl;
      // // std::this_thread::sleep_for(std::chrono::milliseconds(500));
      // std::this_thread::yield();
    // }
    std::unique_lock<std::mutex> lck(mtx);
    cargo++; // 设置全局标志位为 true.
    std::cout << "count = " << ++count << std::endl;
    cv.notify_one(); // 唤醒一个线程.
  }
  std::cout << "go exit now" << std::endl;
}

int main() {
  std::thread threads[10];
  // spawn 10 threads:
  for (int i = 0; i < 10; ++i)
    threads[i] = std::thread(do_print_id, i);

  std::cout << "10 threads ready to race...\n";
  std::thread td(go); // go!
  td.join();
  std::cout << "td had exited." << std::endl;
  cv.notify_all();
  for (auto &th : threads)
    th.join();

  return 0;
}
