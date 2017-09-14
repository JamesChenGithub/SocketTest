//
//  main.cpp
//  ThreadTest
//
//  Created by AlexiChen on 2017/9/13.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#include <iostream>

#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <ctime>


void thread_func()
{
    for (int i = 0; i < 10; i++ )
    {
        std::cout << "Hello, World!\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int fun1(int a)
{
    for (int i = 0; i < 10; i++ )
    {
        std::cout << "Hello, World!" << a+i << std::endl;
    }
    
    return a;
}


void fun2(int &a)
{
    std::cout << " t2 Hello brfroe n addres " << &a << std::endl;
    for (int i = 0; i < 10; i++ )
    {
        std::cout << "Hello, World!" << a+i << std::endl;
        a+= i;
    }
}

void fun3(int &&a)
{
    std::cout << " t2 Hello brfroe n addres " << &a << std::endl;
    for (int i = 0; i < 10; i++ )
    {
        std::cout << "Hello, World!" << a+i << std::endl;
        a+= i;
    }
}

int func4(int a, int &b, int &&c)
{
    for (int i = 0; i < 10; i++ )
    {
        std::cout << "Hello, func4!" << (a*b + c ) *i << std::endl;
        a+= i;
        b -= i;
        c *= i;
    }
    
    return a + b + c;
}

void ttest()
{
    // insert code here...
    std::cout << "Hello, World!\n";
    
    std::thread t = std::thread(thread_func);
    t.join();
    std::cout << "Hello, World!  over\n";
    
    std::thread([=]{
        for (int i = 0; i < 10; i++ )
        {
            std::cout << "Hello, lambda!\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }).join();
    
    
    std::thread([=]{
        for (int i = 0; i < 10; i++ )
        {
            std::cout << "Hello, lambda detach!" << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }).detach();
    
    
    
    std::thread t1;
    int n =  t1.hardware_concurrency();
    std::cout << " t2 Hello brfroe n addres " << &n << std::endl;
    std::thread t2(fun1, n);
    t2.join();
    
    std::cout << " t2 Hello" << n << std::endl;
    
    std::thread t3(fun2, std::ref(n));
    t3.join();
    
    std::cout << "t3 Hello" << n << std::endl;
    
    
    std::thread t4(fun3, std::move(n));
    t4.join();
    
    std::cout << "t4 Hello" << n << std::endl;
    //
    
    std::thread t5(fun3, std::move(n));
    std::thread t6(std::move(t5));
    
    t6.join();
    
    
    int aa = 10, bb = 20, cc = 100;
    
    std::thread(func4,aa, std::ref(bb), std::move(cc)).join();
    
    
    std::thread last(func4,aa, std::ref(bb), std::move(cc));
    
    last.join();
    std::thread::id lid = last.get_id();
    
    bool succ = last.joinable();
    
    
    std::thread s1([]{
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "s1 Hello" << std::endl;
    });
    
    std::thread s2([]{
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "s2 Hello" << std::endl;
    });
    std::cout << s1.get_id() << std::endl;
    s1.swap(s2);
    
    s2.join();
    
    s1.join();
    
    
    std::thread([&, bb](int aaaa)->int{
        aa += aaaa;
        std::cout << aa << bb << std::endl;
        return cc;
    }, 1000).join();
    int cinss;
    std::cin >> cinss;
}


int smfun(int m)
{
    if (m == 1)
    {
        return 1;
    }
    else
    {
        return m * smfun(m-1);
    }
    
}

void mutextest()
{
    //    std::mutex mu;
    //
    //    int count = 0;
    //    auto func = [&](int i){
    //        while (1)
    //        {
    //            if(mu.try_lock())
    //            {
    //                std::ostringstream os;
    //                os << "线程"<< i << "输出 : " << count++ << std::endl;
    //
    //                std::cout << os.str();
    //
    //                mu.unlock();
    //            }
    //
    //            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //        }
    //
    //    };
    //
    //    std::thread p1(func, 1);
    //
    //    p1.detach();
    //
    //    std::thread p2(func, 2);
    //
    //    p2.detach();
    //
    //    std::thread p3(func, 2);
    //
    //    p3.detach();
    //
    //
    //    int a;
    //    std::cin >> a;
    //    std::cout << a;
    
    std::mutex mut;
    std::queue<std::string> pool;
    int count = 0;
    
    auto genfunc = [&](int i) {
        while (1)
        {
            if(mut.try_lock())
            {
                std::ostringstream os;
                os << "生产线程"<< i << "生产 : " << count++ << std::endl;
                
                pool.push(os.str());
                
                mut.unlock();
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(arc4random()%1000));
        }
    };
    
    auto getfunc = [&](int a) {
        while (1)
        {
            if(mut.try_lock())
            {
                
                std::string str = pool.front();
                std::cout << "消费线程" << a << "取出："<< str << std::endl;
                pool.pop();
                
                mut.unlock();
            }
            
            
            std::this_thread::sleep_for(std::chrono::milliseconds(arc4random()%1000));
        }
    };
    
    std::thread(genfunc, 1).detach();
    
    std::thread(genfunc, 2).detach();
    
    std::thread(getfunc, 4).detach();
    
    
    
    
    
    int a;
    std::cin >> a;
    std::cout << a;
    
    std::recursive_mutex rm;
    
    
    
    
    std::function<int(int)> sf = [&sf](int a) mutable -> int {
        if (a == 1)
        {
            return 1;
        }
        else
        {
            return a * sf(a -1);
        }
    };
    
    
    int sum = sf(10);
}

void fu()
{
    //    std::mutex mu;
    //    auto mf = [&](int i) {
    //        while(1)
    //        {
    //            if (mu.try_lock())
    //            {
    //                std::cout << "线程" << i << "执行" << std::endl;
    //                mu.unlock();
    //            }
    //            else
    //            {
    //                std::cout << "线程" << i << "未执行========" << std::endl;
    //            }
    //        }
    //    };
    //
    //    std::thread ts[4];
    //    for (int i = 0; i < 4; i++)
    //    {
    //        ts[i] = std::thread(mf, i);
    //    }
    //
    //    for (int i = 0; i < 4; i++)
    //    {
    //        ts[i].join();
    //    }
    
    //======================
    //    std::timed_mutex tmu;
    //    auto funcc = [&] {
    //        std::cout<<"testtest stat";
    //        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //        if (tmu.try_lock_for(std::chrono::milliseconds(10000)))
    //        {
    //            std::cout<<"testtest";
    //            tmu.unlock();
    //        }
    //
    //        std::cout<<"over";
    //    };
    //
    //    auto testf = [&] {
    //        if (tmu.try_lock()) {
    //
    //            std::cout<<"testf sleep with mutex";
    //            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //            tmu.unlock();
    //        }
    //    };
    //
    //    std::thread(testf).detach();
    //
    //    std::thread(funcc).join();
    
    //======================
    
    //    std::timed_mutex tmu;
    //
    //    std::mutex cm;
    //
    //    auto wait = [&] (int i) {
    //
    //        std::cout << "线程" << i << "开始等待";
    //
    //        //        std::this_thread::sleep_for(std::chrono::milliseconds(arc4random()%100));
    //        while (!tmu.try_lock_for(std::chrono::milliseconds(100)))
    //        {
    //            if (cm.try_lock())
    //            {
    //                std::cout << "-" << i;
    //                cm.unlock();
    //            }
    //        }
    //        std::this_thread::sleep_for(std::chrono::milliseconds(arc4random()%100));
    //        if (cm.try_lock())
    //        {
    //            std::cout << "wait over" << std::endl;
    //            cm.unlock();
    //        }
    //        tmu.unlock();
    //    };
    //
    //    const int tc = 5;
    //    std::thread ts[tc];
    //    for (int i = 0; i < tc; i++)
    //    {
    //        ts[i] = std::thread(wait, i);
    //    }
    //
    //    for (int i = 0; i < tc; i++)
    //    {
    //        ts[i].join();
    //    }
}

int main(int argc, const char * argv[]) {
    
//    std::mutex mu;
//    auto f = [&](int i){
//        mu.lock();
//        std::lock_guard<std::mutex> lck(mu, std::adopt_lock);
//        
//        std::cout << "线程" << i << std::endl;
//    };
//    const int tc = 10;
//    std::thread ts[tc];
//    for (int i = 0; i < tc; i++)
//    {
//        ts[i] = std::thread(f, i);
//    }
//    
//    for (int i = 0; i < tc; i++)
//    {
//        ts[i].join();
//    }
    
    //===================
    
//    auto p = [] (int i) {
//        if (i % 2 == 0)
//        {
//            std::cout << i << "is even" << std::endl;
//        }
//        else
//        {
//            throw std::logic_error("not even");
//        }
//    };
//    
//    std::mutex m;
//    auto f = [&](int i) {
//        try
//        {
//            std::lock_guard<std::mutex> lck(m);
//            p(i);
//        }
//        catch (std::logic_error &error)
//        {
//            std::cout << "[exception caught]" << std::endl;
//        }
//    };
//
//    const int tc = 10;
//    std::thread ts[tc];
//    for (int i = 0; i < tc; i++)
//    {
//        ts[i] = std::thread(f, i);
//    }
//
//    for (int i = 0; i < tc; i++)
//    {
//        ts[i].join();
//    }
    
    //==================
    
//    std::mutex f, b;
//    
//    auto taska = [&] {
//        std::cout << "begin a" << std::endl;
//        std::lock(f, b);
//        std::unique_lock<std::mutex>  lock1(f, std::defer_lock);
//        std::unique_lock<std::mutex>  lock2(b, std::adopt_lock);
//        std::cout << "task a" << std::endl;
//    };
//    
//    auto taskb = [&] {
//        std::cout << "begin b" << std::endl;
//        std::unique_lock<std::mutex>  lock1, lock2;
//        lock1 = std::unique_lock<std::mutex>(b, std::defer_lock);
//        lock2 = std::unique_lock<std::mutex>(f, std::defer_lock);
//        
//        std::lock(lock1, lock2);
//        std::cout << "task b" << std::endl;
//    };
//    
//    std::thread t1(taska);
//    std::thread t2(taskb);
//    
//    t1.join();
//    t2.join();
    
    //===========
    std::mutex mm;
    auto taskc = [&] (char c) {
        std::unique_lock<std::mutex> lck;
        lck = std::unique_lock<std::mutex>(mm);
        for (int i = 0; i < 50; i++)
        {
            std::cout << c;
        }
        
        std::cout << c << std::endl;
        
    };
    std::thread t3(taskc, '-');
    std::thread t4(taskc, '*');
    
    t3.join();
    t4.join();
    
    
    std::timed_mutex lm;
    auto lu1 = [&] {
        if (lm.try_lock())
        {
            std::cout<< "我先睡会" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            lm.unlock();
        }
        
    };
    
    auto lu2 = [&] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
        if (lm.try_lock_until(std::chrono::steady_clock::now() + std::chrono::milliseconds(6000)))
        {
            std::cout<< "对方已睡醒" << std::endl;
            lm.unlock();
        }
        else
        {
            std::cout<< "对方没睡醒，先走了" << std::endl;
        }
        
        
    };

    
    std::thread t5(lu1);
    std::thread t6(lu2);
    
    t5.join();
    t6.join();
    
    
    std::mutex m;
    auto print = [&] {
        std::unique_lock<std::mutex> lck(m, std::defer_lock);
        if (lck.try_lock())
        {
           std::cout << "==";
        }
        else
        {
            std::cout << "XX";
        }
    };
    
    std::vector<std::thread> ts;
    for (int i = 0; i < 500; ++i)
    {
        ts.emplace_back(print);
    }
    
    for (auto &x  : ts) {
        x.join();
    }
    
    

    return 0;
}
