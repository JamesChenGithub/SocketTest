//
//  TCPServerSwitch.hpp
//  ServerManager
//
//  Created by AlexiChen on 2017/9/5.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#ifndef TCPServerSwitch_hpp
#define TCPServerSwitch_hpp

#include <stdio.h>
#include "TCPServer.hpp"

class TCPServerSwitch : public TCPServer
{
    
protected:
    std::shared_ptr<TCPConnectItem>  mServerMgrItem;
    std::thread mRecvThread;
    
public:
    ~TCPServerSwitch();
    TCPServerSwitch();
    TCPServerSwitch(std::shared_ptr<TCPConnectItem> item);
    
public:
    virtual bool start();
    
    virtual bool connectServer(std::shared_ptr<TCPConnectItem> item);
    
    virtual void testPrint();
    
protected:
    virtual void onRecvMsgFromServer();
};

#endif /* TCPServerSwitch_hpp */
