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



typedef struct TCPConnectItem
{
    int mSocketID;
    std::string mHost;
    std::string mPort;
    std::string mNick;
} TCPConnectItem;


class TCPServerListener
{
public:
    virtual ~TCPServerListener(){}
    
    virtual void onLinterError(int err, std::string errorinfo) = 0;
    virtual void onRecvMsgFrom(TCPConnectItem *item, std::string msg) = 0;
    
};


#endif /* Common_hpp */
