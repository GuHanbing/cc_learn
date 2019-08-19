//
// Created by 顾涵彬 on 2019-08-19.
//

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>

struct Base
{
    Base() { std::cout << "  Base::Base()\n"; }
    // 注意：此处非虚析构函数 OK
    ~Base() { std::cout << "  Base::~Base()\n"; }
};

struct Derived: public Base
{
    Derived() { std::cout << "  Derived::Derived()\n"; }
    ~Derived() { std::cout << "  Derived::~Derived()\n"; }
};

static std::mutex io_mutex;  //保证三个线程互斥
void thr(std::shared_ptr<Base> p)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::shared_ptr<Base> lp = p; // 线程安全，虽然自增共享的 use_count
    {


        std::lock_guard<std::mutex> lk(io_mutex);
        std::cout << "local pointer in a thread:\n"
                  << "  lp.get() = " << lp.get()
                  << ", lp.use_count() = " << lp.use_count() << '\n';
    }
}

bool hold = true;
void thr2(std::shared_ptr<Base> p)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::shared_ptr<Base> lp = p; // 线程安全，虽然自增共享的 use_count
    {
        std::lock_guard<std::mutex> lk(io_mutex);
        std::cout << "local pointer in a thread:\n"
                  << "  lp2.get() = " << lp.get()
                  << ", lp2.use_count() = " << lp.use_count() << '\n';

    }
    while(hold)
    {}
}

int main()
{
    std::shared_ptr<Base> p = std::make_shared<Derived>();

    std::cout << "Created a shared Derived (as a pointer to Base)\n"
              << "  p.get() = " << p.get()
              << ", p.use_count() = " << p.use_count() << '\n';
    std::thread t1(thr, p), t2(thr, p);
    std::thread t3(thr2,p);
    p.reset(); // 从 main 释放所有权
    auto a = p.get();       //线程t3仍在占用p的拷贝，但由于p释放了所以use为0？
    std::cout << "Shared ownership between 3 threads and released\n"
              << "ownership from main:\n"
              << "  p.get() = " << a
              << ", p.use_count() = " << p.use_count() << '\n';
    t1.join();
    t2.join();
    hold = false;
    t3.join();
    std::cout << "Shared2 ownership between 3 threads and released\n"
              << "ownership from main:\n"
              << "  p.get() = " << p.get()
              << ", p.use_count() = " << p.use_count() << '\n';
    std::cout << "All threads completed, the last one deleted Derived\n";
}