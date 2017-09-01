//
//  TCPServerManager.cpp
//  ServerManager
//
//  Created by AlexiChen on 2017/9/1.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#include "TCPServerManager.hpp"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <net/if.h>

TCPServer::TCPServer(TCPConnectItem item):TCPServer()
{
    mHostItem.mHost = item.mHost;
    mHostItem.mPort = item.mPort;
    mHostItem.mNick = item.mNick;
    mHostItem.mSocketID = item.mSocketID;
}

TCPServer::TCPServer()
{
    
}

TCPServer::~TCPServer()
{
    
}


bool TCPServer::start()
{
    if (!isVaildItem(mHostItem))
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
    
    mHostItem.mSocketID = socketID;
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(mHostItem.mPort.c_str()));
    addr.sin_addr.s_addr = inet_addr(mHostItem.mHost.c_str());
    
    int err = bind(mHostItem.mSocketID, (const struct sockaddr *)&addr, sizeof(addr));
    
    if (err != 0)
    {
        onListenError(socketID, "bind socket失败");
        return false;
    }
    
    int listencount = listen(mHostItem.mSocketID, 10);
    if (listencount != 0)
    {
        onListenError(socketID, "listen socket失败");
        return false;
    }
    
    mAcceptThread = std::thread([this]{
        this->onAccept();
    });
    
    return true;
}

bool TCPServer::shutdown()
{
    return true;
}

bool TCPServer::isVaildItem(const TCPConnectItem item)
{
    const char *host = item.mHost.c_str();
    if (host)
    {
        in_addr_t addr = inet_addr(host);
        
        if (addr == INADDR_NONE)
        {
            std::cout << "IP地址错误" << std::endl;
            return false;
        }
        
        int port = atoi(item.mPort.c_str());
        
        if (!(port > 1024 && port < 65536))
        {
            std::cout << "端口号错误" << std::endl;
            return false;
        }
        
        return true;
    }
    
    return false;
    
}

void TCPServer::onListenError(int err, std::string errinfo)
{
    auto p = mListener.lock();
    if (p)
    {
        p->onLinterError(err, errinfo);
    }
}

#define kMaxCacheSize 2049

void TCPServer::onAccept()
{
    while (mRunning)
    {
        struct sockaddr_in peeraddr;
        socklen_t addlen = sizeof(peeraddr);
        
        int peer = accept(mHostItem.mSocketID, (struct sockaddr *)&peeraddr, &addlen);
        
        if (peer != -1)
        {
            char *cip = inet_ntoa(peeraddr.sin_addr);
            int port = ntohs(peeraddr.sin_port);
            
            char buf[100];
            sprintf(buf, "%s:%d", cip, port);
            std::string ipid = std::string(buf);
            
            TCPConnectItem *item = new TCPConnectItem();
            item->mHost = std::string(cip);
            bzero(buf, sizeof(buf));
            sprintf(buf, "%d", port);
            item->mPort = std::string(buf);
            item->mSocketID = peer;
            
            
            std::thread recvthread = std::thread(&TCPServer::onRecvMsg, item);
            recvthread.join();
        }
    }
}


void TCPServer::onRecvMsg(TCPConnectItem *clientItem)
{
    if (clientItem)
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
            [self appendTextMsg:[NSString stringWithFormat:@"client(%@:%d) disconnect", cipstr, client_port]];
            exit = YES;
        }
        else
        {
            NSString *recvStr = [NSString stringWithUTF8String:buf];
            NSLog(@"%@", recvStr);
            
            NSString *log = [NSString stringWithFormat:@"recv client(%@:%d)  : %@", cipstr, client_port, recvStr];
            [self appendTextMsg:log];
            exit = [recvStr containsString:@"exit"];
            
            
            @synchronized (_acceptSocketIDs)
            {
                if (_acceptSocketIDs.count == 2)
                {
                    for (NSNumber *socid in _acceptSocketIDs)
                    {
                        if (socid.intValue != peerfd)
                        {
                            [self dispatchToDest:socid.intValue message:log];
                            break;
                        }
                    }
                }
            }
        }
        
    } while(!exit);
    
    [self appendTextMsg:[NSString stringWithFormat:@"%@:%d exit", cipstr, client_port]];
    [_acceptSocketIDs removeObject:@(peerfd)];
    close(peerfd);
    
    delete  clientItem;
    clientitem = NULL;
}

