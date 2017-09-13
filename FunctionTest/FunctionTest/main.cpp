//
//  main.cpp
//  FunctionTest
//
//  Created by AlexiChen on 2017/9/12.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#include <iostream>

#include <functional>

#include <algorithm>

void test1()
{
    int a = 1;
    int b = 2;
    std::cout << &a << "     " << &b << std::endl;
    auto func = [=, &b](int c)->int {
        std::cout << &a << "     " << &b << std::endl;
        return b += a + c;
    };
    a = 30;
    int res = func(5);
    std::cout << res << std::endl;
}

void test2()
{
    int a = 2;
    int b = 4;
    std::cout << &a << "     " << &b << std::endl;
    auto func = [&](int c)->int {
        std::cout << &a << "     " << &b << std::endl;
        a *= c;
        b = a + c;
        c = c + a;
        std::cout << b << std::endl;
        return b;
    };
    
    a = 4;
    func(12);
    b = 111;
}

void test3()
{
    int a = 2;
    int b = 5;
    std::cout << &a  << "   " << &b << std::endl;
    auto ml = [=]() mutable {
        std::cout <<  &a  << "   " << &b  << std::endl;
        int b = 11;
        
        std::cout <<  &a  << "   " << &b  << std::endl;
        b = 10;
        a = 5;
        
    };
    std::cout << &a  << "   a = "<< a << std::endl;
    ml();
    std::cout << &a  << "   a = "<< a << std::endl;
    auto m2 = [&](){
        std::cout << &a << std::endl;
        a = 6;
    };
    std::cout << &a  << "   a = "<< a << std::endl;
    m2();
    std::cout << &a  << "   a = "<< a << std::endl;
    auto m3 = [&]() mutable {
        std::cout << &a << std::endl;
        a = 10;
    };
    std::cout << &a  << "   a = "<< a << std::endl;
    m3();
    std::cout << &a  << "   a = "<< a << std::endl;
}

static int comp(const void *p1, const void *p2)
{
    int a = *((int *)p1);
    int b = *((int *)p2);
    
    return a - b;
    if (a < b)
    {
        return -1;
    }
    else if (a == b)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void scan(int *a, int length, std::function<void(int)> process)
{
    for (int i = 0; i < length; i++)
    {
        process(a[i]);
    }
}

void func(int a)
{
    std::cout << "   " << a;
}

void p1()
{
    int a[10] = {9,7,8,4,5,6,3,2,1,0};
    

    qsort((void *)a, sizeof(a)/sizeof(int), sizeof(int), comp);
    for (int i = 0; i < 10; i++) {
        printf("%4d", a[i]);
    }
    printf("\n");
    
    std::sort(a, &a[10], [](int x, int y) { return x-y;});
    for (int i = 0; i < 10; i++) {
        printf("%4d", a[i]);
    }
    printf("\n");
    
    scan(a, 10, func);
    
    int thr = 5;
    int count = 0;
    scan(a, 10, [=, &count](int v){
        if (v > thr)
        {
            printf("%5d", v);
            count++;
        }
    });
    
    printf("\n count = %d", count);
    
    scan(a, 10, [&](int v) {
        a[v] = v*v;
    });
    
    scan(a, 10, [=](int v) {
        printf("%5d", v);
    });
    printf("\n");
}


void constCheck(int *a, int len, std::function<void(int)> pro)
{
    printf("\n");
    for (int i = 0; i < len; i++)
    {
        pro(a[i]);
    }
    for (int i = 0; i < 10; i++) {
        printf("%4d", a[i]);
    }
    printf("\n");
}

void sort(int *a, int len, std::function<int(int, int)> com)
{
    printf("\n");
    for (int i = 0; i < len; i++)
    {
            for (int j = 0; j < len; j++)
            {
                int a1 = a[i];
                int b1 = a[j];
                
                int c1 = com(a1, b1);
                
                if (c1 < 0)
                {
                    a[i] = b1;
                    a[j] = a1;
                }
            }
    }
    
    for (int i = 0; i < 10; i++) {
        printf("%8d", a[i]);
    }
    printf("\n");
}

void mutalbeCheck(int *a, int len, std::function<int(int)> pro)
{
    printf("\n");
    for (int i = 0; i < len; i++)
    {
        a[i] = pro(a[i]);
    }
    for (int i = 0; i < 10; i++) {
        printf("%8d", a[i]);
    }
    printf("\n");
}




int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    test1();
    
    test2();
    
    test3();
    
    
//    p1();
    
    
    int a[10] = {9,3,4,2,5,1,6,8,7,0};
    
    sort(a, 10, [=](int x, int y) {
        return y - x;
    });
    
    sort(a, 10, [=](int x, int y) {
        return x - y;
    });
    
    constCheck(a, 10, [=](int v){
        printf("%5d", v);
    });
    
    mutalbeCheck(a, 10, [=](int v){
        return v*v;
    });
    
    mutalbeCheck(a, 10, [=](int v){
        return v*v*10;
    });
    
    
    
    
    
    return 0;
}
