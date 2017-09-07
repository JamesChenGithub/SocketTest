//
//  TCPServerManager.cpp
//  ServerManager
//
//  Created by AlexiChen on 2017/9/1.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#include "TCPServer.hpp"

#import <sys/socket.h>
#import <netinet/in.h>
#import <arpa/inet.h>
#import <string.h>
#import <ifaddrs.h>
#import <net/if.h>
#import <unistd.h>

TCPServer::TCPServer(std::shared_ptr<TCPConnectItem> item)
{
    std::unique_ptr<TCPConnectItem> ptr(new TCPConnectItem(*item));
    mHostItem = std::move(ptr);
}

void TCPServer::setHost(std::shared_ptr<TCPConnectItem> item)
{
    
}

TCPServer::TCPServer()
{
    std::cout << "TCPServer() constructor" << std::endl;
    mHostItem.reset();
    mListener.reset();
    
}

TCPServer::~TCPServer()
{
    std::cout << "TCPServer() desconstructor" << std::endl;
}


void TCPServer::setListener(std::shared_ptr<TCPServerListener> aListener)
{
    
    if (aListener.use_count() > 0)
    {
        mListener = aListener;
    }
    
}

bool TCPServer::start()
{
    if (!isVaildItem(mHostItem.get()))
    {
        onListenError(-1, "ip地址或端口有误");
        return false;
    }
    
    
    int socketID = socket(AF_INET, SOCK_STREAM, 0);
    if (socketID < 0)
    {
        onListenError(socketID, "创建socket失败");
        return false;
    }
    
    onTipInfo("创建Socket成功");
    
    mHostItem->mSocketID = socketID;
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(mHostItem->mPort.c_str()));
    addr.sin_addr.s_addr = inet_addr(mHostItem->mHost.c_str());
    
    int err = bind(mHostItem->mSocketID, (const struct sockaddr *)&addr, sizeof(addr));
    if (err != 0)
    {
        onListenError(socketID, "bind socket失败");
        return false;
    }
    
    onTipInfo("绑定Socket成功");
    
    int listencount = listen(mHostItem->mSocketID, 10);
    if (listencount != 0)
    {
        onListenError(socketID, "listen socket失败");
        return false;
    }
    
    onTipInfo("listen Socket成功");
    
    mAcceptThread = std::thread([this]{
        onTipInfo("开始接收链接");
        this->onAccept();
    });
    mAcceptThread.detach();
    return true;
}


bool TCPServer::connectServer(std::shared_ptr<TCPConnectItem> item)
{
    std::unique_ptr<TCPConnectItem> ptr(new TCPConnectItem(*item));
    mHostItem = std::move(ptr);
    
    if (!isVaildItem(mHostItem.get()))
    {
        onListenError(-1, "ip地址或端口有误");
        return false;
    }
    
    
    int socketID = socket(AF_INET, SOCK_STREAM, 0);
    if (socketID < 0)
    {
        onListenError(socketID, "创建socket失败");
        return false;
    }
    
    onTipInfo("创建Socket成功");
    
    mHostItem->mSocketID = socketID;
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(mHostItem->mPort.c_str()));
    addr.sin_addr.s_addr = inet_addr(mHostItem->mHost.c_str());
    
    int err = connect(mHostItem->mSocketID, (const struct sockaddr *)&addr, sizeof(addr));
    if (err != 0)
    {
        onListenError(socketID, "connect socket失败");
        return false;
    }
    
    onTipInfo("connect Socket成功");
    
    mAcceptThread = std::thread([this]{
        onTipInfo("开始接收链接");
        this->onRecvMsgFromServer();
    });
    mAcceptThread.detach();
    return true;
}

void TCPServer::testPrint()
{

    std::cout << "TCPServer::testPrint()" << std::endl;
}


const std::map<std::string, std::shared_ptr<TCPConnectItem>> TCPServer::getConnectMap() const
{
    return mConnMap;
}

bool TCPServer::isVaildItem(TCPConnectItem *item)
{
    const char *host = item->mHost.c_str();
    if (host)
    {
        in_addr_t addr = inet_addr(host);
        
        if (addr == INADDR_NONE)
        {
            std::cout << "IP地址错误" << std::endl;
            return false;
        }
        
        int port = atoi(item->mPort.c_str());
        
        if (!(port > 1024 && port < 65536))
        {
            std::cout << "端口号错误" << std::endl;
            return false;
        }
        
        return true;
    }
    
    return false;
    
}

void TCPServer::onTipInfo(std::string info)
{
    if (!mListener.expired())
    {
        std::shared_ptr<TCPServerListener> lis = mListener.lock();
        if (lis)
        {
            lis->onTipInfo(info);
        }
    }
    
}

void TCPServer::onListenError(int err, std::string errinfo)
{
    if (!mListener.expired())
    {
        std::shared_ptr<TCPServerListener> lis = mListener.lock();
        if (lis)
        {
            lis->onLinterError(err, errinfo);
        }
    }
}

void TCPServer::onConnectError(int err, std::string errinfo)
{
    if (!mListener.expired())
    {
        std::shared_ptr<TCPServerListener> lis = mListener.lock();
        if (lis)
        {
            lis->onConnectError(err, errinfo);
        }
    }
}

void TCPServer::onListenMsg(TCPConnectItem *item, std::string msg)
{
    if (!mListener.expired())
    {
        std::shared_ptr<TCPServerListener> lis = mListener.lock();
        if (lis)
        {
            lis->onRecvMsgFrom(item, msg);
        }
    }
}

void TCPServer::onAcceptConnect(TCPConnectItem *item)
{
    if (!mListener.expired())
    {
        std::shared_ptr<TCPServerListener> lis = mListener.lock();
        if (lis)
        {
            lis->onAcceptConnect(item);
        }
    }
}

void TCPServer::onExitConnect(TCPConnectItem *item)
{
    if (!mListener.expired())
    {
        std::shared_ptr<TCPServerListener> lis = mListener.lock();
        if (lis)
        {
            lis->onExitConnect(item);
        }
    }
}

#define kMaxCacheSize 2049

void TCPServer::onAccept()
{
    mRunning = true;
    while (mRunning)
    {
        struct sockaddr_in peeraddr;
        socklen_t addlen = sizeof(peeraddr);
        
        int peer = accept(mHostItem->mSocketID, (struct sockaddr *)&peeraddr, &addlen);
        
        if (peer != -1)
        {
            char *cip = inet_ntoa(peeraddr.sin_addr);
            int port = ntohs(peeraddr.sin_port);
            
            char buf[100];
            sprintf(buf, "%s:%d", cip, port);
            std::string ipid = std::string(buf);
            onTipInfo("接受"+ipid+"连接");
            
            TCPConnectItem *item = new TCPConnectItem();
            item->mHost = std::string(cip);
            bzero(buf, sizeof(buf));
            sprintf(buf, "%d", port);
            item->mPort = std::string(buf);
            item->mSocketID = peer;
            
            
            std::thread thread = std::thread([this, &item]{
                onTipInfo("new thread to recv msg");
                this->onRecvMsgFromClient(item);
            });
            
            thread.detach();
            
            std::shared_ptr<TCPConnectItem> ip(item);
            mConnMap.insert(std::pair<std::string, std::shared_ptr<TCPConnectItem>>(ipid, ip));
            
            onAcceptConnect(item);
            
            std::cout << "new thread to handle msg" << std::endl;
        }
    }
}

void TCPServer::onRecvMsg(TCPConnectItem *serverItem, std::string msg)
{
    if (!mListener.expired())
    {
        std::shared_ptr<TCPServerListener> lis = mListener.lock();
        if (lis)
        {
            lis->onRecvMsgFrom(serverItem, msg);
        }
    }
}


void TCPServer::onRecvMsgFromServer()
{
    mRunning = true;
    
    while (mRunning)
    {
        char buf[kMaxCacheSize];
        memset(&buf, 0, sizeof(buf));
        ssize_t size =  recv(mHostItem->mSocketID, &buf, sizeof(buf) - 1, 0);
        
        if (size <= 0)
        {
            
            break;
        }
        else
        {
            printf("recv from server : %s\n", buf);
            
            onRecvMsg(mHostItem.get(), std::string(buf));
        }
    }
    
    onExitConnect(mHostItem.get());
    close(mHostItem->mSocketID);
}



void TCPServer::onRecvMsgFromClient(TCPConnectItem *clientItem)
{
    if (!clientItem)
    {
        return;
    }
    
    bool exit = false;
    do
    {
        char buf[kMaxCacheSize];
        memset(&buf, 0, sizeof(buf));
        
        ssize_t size = recv(clientItem->mSocketID, buf, sizeof(buf) - 1, 0);
        
        if (size == -1 || size == 0)
        {
            std::cout << "connection exit" << std::endl;
            exit = true;
        }
        else
        {
            std::string recvStr = "recv client (" + clientItem->mHost + ":" + clientItem->mPort + ") msg : " + std::string(buf);
            
            std::cout << recvStr << std::endl;
            
            onListenMsg(clientItem, std::string(buf));
            
            exit = recvStr.find("exit") != std::string::npos;
        }
        
    } while(!exit);
    
    close(clientItem->mSocketID);
    mConnMap.erase(std::string(clientItem->mHost + ":" + clientItem->mPort));
}

