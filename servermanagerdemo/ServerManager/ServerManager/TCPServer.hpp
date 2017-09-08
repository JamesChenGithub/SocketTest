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
#include <list>
#include "Common.hpp"

class TCPServer
{
protected:
    bool   mRunning;                                // 是否在运行
    std::shared_ptr<TCPConnectItem>  mHostItem;     // 本机记录
    
    std::weak_ptr<TCPServerListener> mListener;     // 监听者
    
    std::thread mAcceptThread;                      // 监听线程
    
    std::map<std::string, std::shared_ptr<TCPConnectItem>> mConnMap;   // 连接表
    
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
    bool isVaildItem(std::shared_ptr<TCPConnectItem> item);
    
protected:
    void onTipInfo(std::string info);
    void onListenError(int err, std::string errinfo);
    void onConnectError(int err, std::string errinfo);
    void onListenMsg(std::shared_ptr<TCPConnectItem> item, std::string msg);
    virtual void onRecvMsg(std::shared_ptr<TCPConnectItem> item, std::string msg);
    void onAcceptConnect(std::shared_ptr<TCPConnectItem> item);
    void onAcceptConnectList(std::list<std::shared_ptr<TCPConnectItem> > itemList);
    
    void onExitConnect(std::shared_ptr<TCPConnectItem> item);
protected:
    void onAccept();
    void onRecvMsgFromServer();
    void onRecvMsgFromClient(std::shared_ptr<TCPConnectItem> item);
    
    
protected:
    virtual void onNotifyNickName(std::shared_ptr<TCPConnectItem> clientItem);
    virtual void onNotifyLogin(std::shared_ptr<TCPConnectItem> clientItem);
    virtual void onNotifySync(std::shared_ptr<TCPConnectItem> clientItem);
    virtual void onNotifyExit(std::shared_ptr<TCPConnectItem> clientItem);
    virtual void onNotifyServerSwitchExit(std::shared_ptr<TCPConnectItem> clientItem);
    
    virtual void onSwitchMsg(char *buf, std::shared_ptr<TCPConnectItem> fromUser, std::shared_ptr<TCPConnectItem> toUser);
    
};

#endif /* TCPServerManager_hpp */
