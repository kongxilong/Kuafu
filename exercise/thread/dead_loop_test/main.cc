#include <condition_variable> // std::condition_variable
#include <iostream>           // std::cout
#include <mutex>              // std::mutex, std::unique_lock
#include <thread>             // std::thread
#include <chrono>             // std::chrono
#include <functional>             // std::chrono


void do_print_id(int id, int times = 1e6) {
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(times));
    std::cout << "thread id = " << id << "test dead loop." << '\n';
  }
}



int main() {
  std::thread threads[100];
  for(int i = 0; i < 99; i++){
    // auto func = std::bind(do_print_id,i,1e6);
  // threads[i] = std::thread(func);
  threads[i] = std::move(std::thread(do_print_id,i,1e6));
  }
    auto func = std::bind(do_print_id,99,10);
  threads[99] = std::move(std::thread(func));
  for(int i = 0; i < 100; i++){
  threads[i].join();
  }
  std::cout << "td had exited." << std::endl;

  return 0;
}
