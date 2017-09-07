//
//  Common.hpp
//  ServerManager
//
//  Created by AlexiChen on 2017/9/1.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#ifndef Common_hpp
#define Common_hpp


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>



class TCPConnectItem
{
public:
    int mSocketID;
    std::string mHost;
    std::string mPort;
    std::string mNick;
    
public:
    
    TCPConnectItem()
    {
        
    }
    
    TCPConnectItem(const TCPConnectItem &item) : TCPConnectItem()
    {
        mSocketID = item.mSocketID;
        mHost =  std::string(item.mHost);
        mPort =  std::string(item.mPort);
        mNick =  std::string(item.mNick);
    }
    
    ~TCPConnectItem()
    {
        std::cout << mHost << ":" << mPort << "release" << std::endl;
    }
};


class TCPServerListener
{
public:
    virtual ~TCPServerListener(){}
    
    virtual void onTipInfo(std::string info) = 0;
    
    virtual void onLinterError(int err, std::string errorinfo) = 0;
    
    virtual void onConnectError(int err, std::string errinfo) = 0;
    
    virtual void onAcceptConnect(TCPConnectItem *item) = 0;
    
    virtual void onExitConnect(TCPConnectItem *item) = 0;
    
    
    virtual void onRecvMsgFrom(TCPConnectItem *item, std::string msg) = 0;
    
};


#endif /* Common_hpp */
