//
//  main.cpp
//  RightValueTest
//
//  Created by AlexiChen on 2017/9/15.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#include <iostream>

class Person
{
public:
    Person()
    {
        std::cout << "construct" << std::endl;
    }
    
    Person(const Person& p)
    {
        std::cout << "copy construct" << std::endl;
    }
    
    ~Person()
    {
        std::cout << "destruct" << std::endl;
    }
    
    const Person& operator=(const Person& p)
    {
        std::cout << "operator=" << std::endl;
        return *this;
    }
};

Person get()
{
    Person p;
    return p;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    Person p = get();
    std::cout << "test over" << std::endl;
    return 0;
}
