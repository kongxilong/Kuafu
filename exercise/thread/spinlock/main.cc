#include <atomic>   // std::atomic, std::atomic_flag, ATOMIC_FLAG_INIT
#include <iostream> // std::cout
#include <thread>   // std::thread, std::this_thread::yield
#include <vector>   // std::vector 
// 由 false 初始化一个 std::atomic<bool>
//
std::atomic_flag lock = ATOMIC_FLAG_INIT;
int cnt = 100000;
void do_count1m(int id) {
  for(int j = 0; j < 10000; j++){
    while(lock.test_and_set(std::memory_order_acquire));
    cnt--;
    std::cout << "Output from thread " << id << std::endl;
    lock.clear(std::memory_order_release);
  }
}

int main() {
  std::vector<std::thread> threads;
  std::cout << "spawning 10 threads that count to 1 million...\n";
  for (int i = 1; i <= 10; ++i)
    threads.push_back(std::thread(do_count1m, i));
  for (auto &th : threads)
    th.join();
  std::cout << "finally cnt = " << cnt << std::endl;
  return 0;
}
