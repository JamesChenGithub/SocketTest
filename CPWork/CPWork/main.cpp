//
//  main.cpp
//  CPWork
//
//  Created by AlexiChen on 2017/9/11.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#include <iostream>


#include <vector>
#include <list>
#include <functional>
#include <algorithm>
#include <iomanip>
#include <string>
#include <time.h>
#include <bitset>


void calCP(std::vector<std::vector<int>> list, std::vector<int> &result)
{
    int cps[33];
    bzero(&cps, sizeof(cps));
    
    int count = 0;
    
    for (std::vector<std::vector<int>>::iterator it = list.begin(); it != list.end(); it++)
    {
        int aaa = 0;
        for (std::vector<int>::iterator ait = it->begin(); ait != it->end(); ait++)
        {
            aaa++;
            if (aaa == 7)
            {
                break;
            }
//            printf("%5d", *ait);
            if (cps[*ait - 1] == 0)
            {
                cps[*ait - 1] = 1;
                count++;
            }
        }
        
//        printf("\n");
    }
    
    if (count== 27)
    {
        // 说明符合
        for (int i = 0; i < 33; i++)
        {
            if (cps[i] == 0)
            {
                result.push_back(i+1);
            }
        }
        
        result.push_back((unsigned int)arc4random()%16);
        
        
    }
    
}


void function1()
{
    int cpCount = 0;
    std::vector<std::vector<int>> veclist;
    const int cpMax = 10;
    while (cpCount < cpMax)
    {
        
        int count = 0;
        std::vector<int> cpItem;
        
        while (count < 7)
        {
            int num = 0;
            if (count < 6)
            {
                num = arc4random()%33 + 1;
                bool isRepeated = false;
                for (std::vector<int>::iterator it = cpItem.begin(); it != cpItem.end(); it++)
                {
                    if (num == *it)
                    {
                        isRepeated = true;
                        break;
                    }
                }
                
                if (!isRepeated)
                {
                    count++;
                    cpItem.push_back(num);
                }
            }
            else if (count == 6)
            {
                sort(cpItem.begin(), cpItem.end());
                
                num = arc4random()%16 + 1;
                count++;
                cpItem.push_back(num);
            }
            else
            {
                count = 0;
                cpItem.clear();
            }
            
        }
        
        
        std::setw(10);
        cpCount++;
        
        printf("随机第%5d组号码:", cpCount);
        
        for (std::vector<int>::iterator it = cpItem.begin(); it != cpItem.end(); it++)
        {
            printf("%5d", *it);
        }
        printf("\n");
        
        veclist.push_back(cpItem);
    }
    
    
    clock_t start,ends;
    start = clock();
    std::cout<< "计算开始时:" << start <<std::endl;
    std::list<std::vector<std::vector<int>>> resultList;
    for (int a = 0; a < cpMax; a++)
    {
        for (int b = a + 1; b < cpMax; b++)
        {
            for (int c = b + 1; c < cpMax; c++)
            {
                for (int d = c + 1; d < cpMax; d++)
                {
                    for (int e = d + 1; e < cpMax; e++)
                    {
                        std::vector<std::vector<int>> list;
                        list.push_back(veclist[a]);
                        list.push_back(veclist[b]);
                        list.push_back(veclist[c]);
                        list.push_back(veclist[d]);
                        list.push_back(veclist[e]);
                        
                        std::vector<int> result;
                        
                        calCP(list, result);
                        
                        if (result.size() == 7)
                        {
                            printf("选到合适的组合\n=============================\n");
                            list.push_back(result);
                            for (std::vector<std::vector<int>>::iterator it = list.begin(); it != list.end(); it++)
                            {
                                for (std::vector<int>::iterator ait = it->begin(); ait != it->end(); ait++)
                                {
                                    printf("%5d", *ait);
                                }
                                printf("\n");
                            }
                            printf("\n=============================\n");
                            
                            resultList.push_back(list);
                        }
                        else
                        {
                            
                        }
                    }
                }
            }
        }
    }
    
    ends = clock();
    std::cout<< "计算结束时间"<< ends << " , 整个耗时:" << (ends - start)/1000000.0 << " 秒 "<<std::endl;
}


int temp1 = 0;
int temp2 = 0;

void printCP(unsigned long long cp)
{
    int count = 1;
    unsigned long long cpt = cp;
    while (count <= 33 && cp > 0)
    {
        if((cp & 1ul) == 1ul)
        {
            printf("%5d", count);
        }
        count++;
        cp >>=1 ; // 移位
    }
    
    unsigned long long b = cpt & 0xF000000000000000;
    b = (unsigned long long)(b >> 60);
    
    printf("%5llu\n", b + 1);
    
}

void convertCPList(std::vector<unsigned long long> cpList, std::vector<std::vector<int>> &test1List)
{
    std::cout << "开始转>>>>>>>>>>>>>>>>" << std::endl;
    int acount = 0;
    for (std::vector<unsigned long long>::iterator it = cpList.begin(); it != cpList.end(); it++)
    {
        acount++;
        printf("生成第%5d组号码",acount);
        int count = 1;
        unsigned long long cp = *it;
        unsigned long long cpt = cp;
        
        std::vector<int> item;
        while (count <= 33 && cp > 0)
        {
            if((cp & (unsigned long long)1) ==  (unsigned long long)1)
            {
                printf("%5d", count);
                item.push_back(count);
                
            }
            count++;
            cp >>=1 ; // 移位
        }
        
        unsigned long long b = cpt & 0xF000000000000000;
        b = b >> 60;
        printf("%5llu\n", b + 1);
        item.push_back((int)b + 1);
        
        test1List.push_back(item);
    }
}

void function2()
{
    int cpCount = 0;
    std::vector<unsigned long long> cpList;
    const int cpMax = 20;
    while (cpCount < cpMax)
    {
        int count = 0;
        unsigned long long cp = 0;
        
        printf("生成第%5d组号码>>>>>>>:", cpCount+1);
        while (count < 7)
        {
            if (count < 6)
            {
                unsigned int ball = (unsigned int)(arc4random()%33 + 1);
                unsigned long long num = (unsigned long long)(1ul << (ball - 1));
                bool isRepeated = false;
                unsigned long long check = cp & num;
                if (check == num)
                {
                    isRepeated = true;
                }
                
                if (!isRepeated)
                {
                    cp |= num;
                    printf("%5d", ball);
                    count++;
                }
            }
            else if (count == 6)
            {
                unsigned long long ball = (unsigned long long)(arc4random()%16);
                unsigned long long num = (unsigned long long)ball << 60;
                cp = cp | num;
                count++;
                printf("%5d", (int)ball + 1);
                temp1 = int (ball + 1);
            }
            else
            {
                count = 0;
            }
            
        }
        
        if (count == 7)
        {
            printf(" 对应标识: %30llu   ", cp);
            printCP(cp);
            cpList.push_back(cp);
            cpCount++;
        }
    }
    
    clock_t start,ends;
    start = clock();
    std::cout<< "计算开始时:" << start <<std::endl;
    
    int ccc = 0;
    for (int a = 0; a < cpMax; a++)
    {
        for (int b = a + 1; b < cpMax; b++)
        {
            for (int c = b + 1; c < cpMax; c++)
            {
                for (int d = c + 1; d < cpMax; d++)
                {
                    for (int e = d + 1; e < cpMax; e++)
                    {
                        unsigned long long ca = cpList[a] & 0x00000001FFFFFFFF;
                        unsigned long long cb = cpList[b] & 0x00000001FFFFFFFF;
                        unsigned long long cc = cpList[c] & 0x00000001FFFFFFFF;
                        unsigned long long cd = cpList[d] & 0x00000001FFFFFFFF;
                        unsigned long long ce = cpList[e] & 0x00000001FFFFFFFF;
                        
                        
                        unsigned long long cpc = ca | cb | cc | cd | ce;
                        
                        unsigned long long res = cpc;
                        int count = 0;
                        for ( count = 0; cpc; ++count)
                        {
                            cpc &= (cpc -1) ; // 清除最低位的1
                        }
                        
                        if (count == 27)
                        {
                            // 是有效组合
                            printf("打印[%d]组合\n=======================\n", ++ccc);
                            printf("[%5d] : ", a + 1);
                            printCP(cpList[a]);
                            printf("[%5d] : ", b + 1);
                            printCP(cpList[b]);
                            printf("[%5d] : ", c + 1);
                            printCP(cpList[c]);
                            printf("[%5d] : ", d + 1);
                            printCP(cpList[d]);
                            printf("[%5d] : ", e + 1);
                            printCP(cpList[e]);
                            
                            res = ~res;
                            res = res & 0x00000001FFFFFFFF;
                            unsigned long long ball = arc4random()%16 + 1;
                            unsigned long long num = ball << 60;
                            res = res | num;
                            printf("[反  补] : ");
                            printCP(res);
                            printf("打印组合结束\n=======================\n");
                            
//                            std::vector<std::vector<int>> veclist;
//                            
//                            std::vector<unsigned long long> testCpList;
//                            testCpList.push_back(cpList[a]);
//                            testCpList.push_back(cpList[b]);
//                            testCpList.push_back(cpList[c]);
//                            testCpList.push_back(cpList[d]);
//                            testCpList.push_back(cpList[e]);
//                            convertCPList(testCpList, veclist);
//                            
//                            std::vector<int> result1;
//                            
//                            calCP(veclist, result1);
//                            
//                            if (result1.size() == 7)
//                            {
//                            
//                                for (std::vector<std::vector<int>>::iterator it = veclist.begin(); it != veclist.end(); it++)
//                                {
//                                    for (std::vector<int>::iterator ait = it->begin(); ait != it->end(); ait++)
//                                    {
//                                        printf("%5d", *ait);
//                                    }
//                                    printf("\n");
//                                }
//                            }
//                            else
//                            {
//                                
//                            }
                            
                        }
                        
                        
                    }
                }
            }
        }
    }
    
    ends = clock();
    std::cout<< "计算结束时间"<< ends << " , 整个耗时:" << (ends - start)/1000000.0 << " 秒 "<<std::endl;
    
    
    std::vector<std::vector<int>> veclist;
    
    convertCPList(cpList, veclist);
    
        printf("方案2:\n=========================================================\n");
        start = clock();
        std::cout<< "计算开始时:" << start <<std::endl;
        std::list<std::vector<std::vector<int>>> resultList;
        int bbb = 0;
        for (int a = 0; a < cpMax; a++)
        {
            for (int b = a + 1; b < cpMax; b++)
            {
                for (int c = b + 1; c < cpMax; c++)
                {
                    for (int d = c + 1; d < cpMax; d++)
                    {
                        for (int e = d + 1; e < cpMax; e++)
                        {
                            std::vector<std::vector<int>> list;
                            list.push_back(veclist[a]);
                            list.push_back(veclist[b]);
                            list.push_back(veclist[c]);
                            list.push_back(veclist[d]);
                            list.push_back(veclist[e]);
    
                            std::vector<int> result;
    
                            calCP(list, result);
    
                            if (result.size() == 7)
                            {
                                printf("选到合适的组合 [%d]\n=============================\n", ++bbb);
                                list.push_back(result);
                                for (std::vector<std::vector<int>>::iterator it = list.begin(); it != list.end(); it++)
                                {
                                    for (std::vector<int>::iterator ait = it->begin(); ait != it->end(); ait++)
                                    {
                                        printf("%5d", *ait);
                                    }
                                    printf("\n");
                                }
                                printf("\n=============================\n");
    
                                resultList.push_back(list);
                            }
                            else
                            {
    
                            }
                        }
                    }
                }
            }
        }
        
        ends = clock();
        std::cout<< "计算结束时间"<< ends << " , 整个耗时:" << (ends - start)/1000000.0 << " 秒 "<<std::endl;
}






int main(int argc, const char * argv[])
{
    function2();
    return 0;
}
