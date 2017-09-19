//
//  main.cpp
//  FutureTest
//
//  Created by AlexiChen on 2017/9/14.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#include <iostream>

#include <future>
#include <thread>
#include <functional>

int main(int argc, const char * argv[]) {
    // insert code here...
//    std::promise<int> pro;
//    std::future<int> ftu = pro.get_future();
//    std::thread t([&](std::future<int> &f) {
//        int x = ftu.get();
//        std::cout << x << std::endl;
//    },std::ref(ftu));
//    
//    pro.set_value(100);
//    t.join();
//    
//    //================
//    
//    std::promise<int> ap;
//    std::future<int> fu = ap.get_future();
//    
//    auto getF = [&] {
//        int x;
//        std::cin.exceptions(std::ios::failbit);
//        
//        try
//        {
//            std::cin >> x;
//            
//            if (x > 0)
//            {
//                ap.set_value(x);
//            }
//            else
//            {
//                
//                // ap.set_exception(std::logic_error("输入为负"));
//                throw std::logic_error("输入为负");
//            }
//        } catch (std::exception &) {
//            ap.set_exception(std::current_exception());
//        }
//    };
//    
//    auto printF = [&] {
//        try
//        {
//            int x = fu.get();
//            std::cout << "输入值为：" << x << std::endl;
//        } catch (std::exception &e) {
//            std::cout << "捕到异常: " << e.what() << std::endl;
//        }
//    };
//    
//    
//    std::thread t1(getF);
//    std::thread t2(printF);
//    
//    t1.join();
//    t2.join();
//    
//    
//    //============
    
    
//    auto func = [](int from, int to) -> int {
//        for (int i = from; i > to; i--)
//        {
//            std::cout << i << std::endl;
//            std::this_thread::sleep_for(std::chrono::seconds(1));
//        }
//        
//        std::cout << "结束" << std::endl;
//        return from - to;
//    };
//    std::packaged_task<int(int, int)> task(func);
//    
//    std::future<int> ret = task.get_future();
//    
//    std::thread th(std::move(task), 10, 5);
//    
//    int value = ret.get();
//    
//    std::cout << "数了" << value << "光华" << std::endl;
//    
//    th.join();
    
    //=================+
    
//    std::packaged_task<int(int)> foo;
//    std::packaged_task<int(int)> bar([](int x){return x*2;});
//    
//    foo = std::move(bar);
//    
//    std::future<int> ret = foo.get_future();
//    
//    std::thread(std::move(foo), 10).detach();
//    
//    int val = ret.get();
//    
//    std::cout << "double of " << val << std::endl;
    
    //=================+
    
    std::packaged_task<int(int)> task([](int x){return 3*x;});
    
    std::future<int> fget = task.get_future();
    
    std::thread(std::move(task), 100).detach();
    
    int va = fget.get();
    
    std::cout << va;
    
    task.reset();
    std::future<int> fget2 = task.get_future();

    std::thread(std::move(task), 200).detach();
    
    std::cout << fget2.get();

    
    return 0;
}
