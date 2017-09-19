//
//  main.cpp
//  ThreadPool
//
//  Created by AlexiChen on 2017/9/15.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#include <iostream>
#include <string>

#include "ThreadPool.hpp"

void print(int i)
{
    std::cout << "hello" << i << std::endl;
}

struct G{
    int operator()()
    {
        std::cout<< " hello g" << std::endl;
        return 50;
    }
};

int main(int argc, const char * argv[]) {
    // insert code here...
    
    
    try
    {
        
        
//        ThreadPool pool(6);
//        
//        for (int i = 100; i > 0; i--)
//        {
//            if (arc4random() % 3 == 2)
//            {
//                std::future<void> f = pool.commit(print, i);
//                f.get();
//            }
//            else if (arc4random() % 3 == 1)
//            {
//                std::future<int> g = pool.commit(G{});
//                std::cout << g.get() << std::endl;
//            }
//            else
//            {
//                std::future<std::string> h = pool.commit([]()->std::string {
//                    std::cout<< " hello h" << std::endl;
//                    return "hell hjjjjj";
//                });
//                
//                std::cout << h.get() << std::endl;
//            }
//        }
//        
//        pool.shutdown();
        
        
        int a[10] = {1,5,3,4,2,9,8,7,0,6};
        
        std::vector<int> vec(a, a + sizeof(a)/sizeof(int));
        
        std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, "\n"));
        
//        std::count_if(vec.begin(), vec.end(), [](int x){
//            std::cout << x;
//        });
        
    } catch (std::exception &err)
    {
        std::cout << "发生异常" << err.what() << std::endl;
    }

    
    
    
    
    return 0;
}
