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
    std::unique_ptr<TCPConnectItem>  mHostItem;
    
    std::weak_ptr<TCPServerListener> mListener;
    
    std::thread mAcceptThread;
    
    std::map<std::string, std::shared_ptr<TCPConnectItem>> mConnMap;
    
public:
    ~TCPServer();

    TCPServer();
    TCPServer(std::shared_ptr<TCPConnectItem> item);
    
    void setHost(std::shared_ptr<TCPConnectItem> item);
    void setListener(std::shared_ptr<TCPServerListener> aListener);
    
    
    virtual bool start();
    virtual bool connectServer(std::shared_ptr<TCPConnectItem> item);
    
    const std::map<std::string, std::shared_ptr<TCPConnectItem>> getConnectMap() const;
    
    virtual void testPrint();
    
    
    
protected:
    bool isVaildItem(TCPConnectItem *item);
    
protected:
    void onTipInfo(std::string info);
    void onListenError(int err, std::string errinfo);
    void onConnectError(int err, std::string errinfo);
    void onListenMsg(TCPConnectItem *item, std::string msg);
    virtual void onRecvMsg(TCPConnectItem *item, std::string msg);
    void onAcceptConnect(TCPConnectItem *item);
    
    void onExitConnect(TCPConnectItem *item);
protected:
    void onAccept();
    void onRecvMsgFromServer();
    void onRecvMsgFromClient(TCPConnectItem *clientItem);
    
};

#endif /* TCPServerManager_hpp */
