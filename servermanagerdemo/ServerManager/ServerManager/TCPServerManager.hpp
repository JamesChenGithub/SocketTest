//
//  TCPServerManager.hpp
//  ServerManager
//
//  Created by AlexiChen on 2017/9/1.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#ifndef TCPServerManager_hpp
#define TCPServerManager_hpp

#include <iostream>
#include <thread>
#include <map>
#include "Common.hpp"

class TCPServer
{
protected:
    bool   mRunning;
    TCPConnectItem mHostItem;
    
    std::weak_ptr<TCPServerListener> mListener;
    
    std::thread mAcceptThread;
    std::map<std::string, TCPConnectItem> mConnMap;
    
private:
    ~TCPServer();
    TCPServer();
public:
    TCPServer(const TCPConnectItem item);
    
    
    
    
    bool start();
    
    bool shutdown();
    
protected:
    bool isVaildItem(const TCPConnectItem item);
    
protected:
    void onListenError(int err, std::string errinfo);
    
protected:
    void onAccept();
    void onRecvMsg(TCPConnectItem *clientItem);
    
};

#endif /* TCPServerManager_hpp */
