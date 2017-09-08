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
    std::string mServKey;    // 所连服务器的标识
    
public:
    ~TCPConnectItem()
    {
        std::cout << mHost << ":" << mPort << "release" << std::endl;
    }
    
    TCPConnectItem()
    {
        
    }
    
    TCPConnectItem(std::shared_ptr<TCPConnectItem> item)
    {
        if (item)
        {
            
        }
    }
    
//    TCPConnectItem(const TCPConnectItem &item) : TCPConnectItem()
//    {
//        mSocketID = item.mSocketID;
//        mHost =  std::string(item.mHost);
//        mPort =  std::string(item.mPort);
//        mNick =  std::string(item.mNick);
//        mIsDirectConnect = true;
//    }
//    
//    TCPConnectItem(const TCPConnectItem *item) : TCPConnectItem()
//    {
//        if(item)
//        {
//            
//            mSocketID = item->mSocketID;
//            mHost =  std::string(item->mHost);
//            mPort =  std::string(item->mPort);
//            mNick =  std::string(item->mNick);
//            mIsDirectConnect = true;
//        }
//    }
    
    
    
    const std::string getKey() const
    {
        return std::string(mHost + ":" + mPort);
    }
};


class TCPServerListener
{
public:
    virtual ~TCPServerListener(){}
    
    virtual void onTipInfo(std::string info) = 0;
    
    virtual void onLinterError(int err, std::string errorinfo) = 0;
    
    virtual void onConnectError(int err, std::string errinfo) = 0;
    
    virtual void onAcceptConnect(std::shared_ptr<TCPConnectItem> item) = 0;
    virtual void onAcceptConnectList(std::list<std::shared_ptr<TCPConnectItem> > itemList) = 0;
    virtual void onExitConnect(std::shared_ptr<TCPConnectItem> item) = 0;
    
    
    virtual void onRecvMsgFrom(std::shared_ptr<TCPConnectItem> item, std::string msg) = 0;
    
};


#endif /* Common_hpp */
