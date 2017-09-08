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
#include <sstream>

#include "TCPUser.pb.h"

TCPServer::TCPServer(std::shared_ptr<TCPConnectItem> item)
{
    if (isVaildItem(item))
    {
        std::unique_ptr<TCPConnectItem> ptr(new TCPConnectItem(*item));
        ptr->mServKey = ptr->getKey();
        mHostItem = std::move(ptr);
    }
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
    if (!isVaildItem(std::make_shared<TCPConnectItem>(*mHostItem)))
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
    
    if (!isVaildItem(item))
    {
        onListenError(-1, "ip地址或端口有误");
        return false;
    }
    
    mHostItem = item;
    
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
        onTipInfo("开始接收ServerSwitch数据");
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

bool TCPServer::isVaildItem(std::shared_ptr<TCPConnectItem>item)
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

void TCPServer::onListenMsg(std::shared_ptr<TCPConnectItem> item, std::string msg)
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

void TCPServer::onAcceptConnect(std::shared_ptr<TCPConnectItem>item)
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

void TCPServer::onAcceptConnectList(std::list<std::shared_ptr<TCPConnectItem>> itemList)
{
    if (!mListener.expired())
    {
        std::shared_ptr<TCPServerListener> lis = mListener.lock();
        if (lis)
        {
            lis->onAcceptConnectList(itemList);
        }
    }
    
}
void TCPServer::onExitConnect(std::shared_ptr<TCPConnectItem>item)
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
            
            std::shared_ptr<TCPConnectItem> item (new TCPConnectItem());
            item->mHost = std::string(cip);
            bzero(buf, sizeof(buf));
            sprintf(buf, "%d", port);
            item->mPort = std::string(buf);
            item->mSocketID = peer;
            item->mNick = "ServerSwitch" + std::to_string((int)mConnMap.size());
            item->mServKey = mHostItem->getKey();
            
            std::thread thread = std::thread([this, &item] {
                onTipInfo("new thread to recv msg");
                this->onRecvMsgFromClient(item);
            });
            
            thread.detach();
            mConnMap.insert(std::pair<std::string, std::shared_ptr<TCPConnectItem>>(ipid, item));
            
            // 给其分配nickName
            onNotifyNickName(item);
            
            if (mConnMap.size() > 1)
            {
                // 通知其他人
                onNotifyLogin(item);
                
                // 同步用户列表不列表
                onNotifySync(item);
            }
            
            onAcceptConnect(item);
            
            std::cout << "new thread to handle msg" << std::endl;
        }
    }
}

void TCPServer::onRecvMsg(std::shared_ptr<TCPConnectItem> serverItem, std::string msg)
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
            
            onRecvMsg(mHostItem, std::string(buf));
        }
    }
    
    close(mHostItem->mSocketID);
    onExitConnect(mHostItem);
}



void TCPServer::onRecvMsgFromClient(std::shared_ptr<TCPConnectItem> clientItem)
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
            
            // 当前与之前的serverswitch断开
            // 通知其他用户 serverswitch + serverswitch下的client断开
            onNotifyServerSwitchExit(clientItem);
        }
        else
        {
            
            TCPMsg *msg = new TCPMsg();
            bool succ = msg->ParseFromArray(buf, (int)size);
            
            if (succ)
            {
                TCPMsg_TCPMsgType msgType = msg->msgtype();
                switch (msgType)
                {
                    case TCPMsg_TCPMsgType_EMsg_Msg:
                    {
                        // 转发消息
                        TCPUser *fromUser = msg->mutable_fromuser();
                        
                        TCPMsgContent *content = msg->mutable_msgcontent();
                        
                        TCPUser *toUser = content->mutable_touser();
                        
                        std::map<std::string, std::shared_ptr<TCPConnectItem>>::iterator it ;
                        for (it = mConnMap.begin(); it != mConnMap.end(); it++)
                        {
                            if (toUser->servkey() == it->second->getKey())
                            {
                                break;
                            }
                        }
                        
                        if (it != mConnMap.end())
                        {
                            // 向接口机转发
                            send(it->second->mSocketID, buf, size, 0);
                            
                            std::ostringstream stream;
                            if (len > 0)
                            {
                                stream << "向" << it->second->getKey() << "转发" << fromUser->mutable_hostip() << ":" << fromUser->mutable_hostport() << "给" << toUser->mutable_hostip() << ":" << toUser->mutable_hostport() <<"的消息成功" << std::endl;
                            }
                            else
                            {
                                stream << "向" << it->second->getKey() << "转发" << fromUser->mutable_hostip() << ":" << fromUser->mutable_hostport() << "给" << toUser->mutable_hostip() << ":" << toUser->mutable_hostport() <<"的消息失败" << std::endl;
                            }
                            
                            onTipInfo(stream.str());
                        }
                        
                        break;
                    }
                    case TCPMsg_TCPMsgType_EMsg_Cmd:
                    {
                        // 转发消息
                        TCPUser *fromUser = msg->mutable_fromuser();
                        
                        TCPCmdContent *cmd = msg->mutable_cmdcontent();
                        
                        TCPUser *toUser = cmd->mutable_touser();
                        
                        TCPCmdContent_TCPMsgType type = cmd->cmdtype();
                        switch (type)
                        {
                            case TCPCmdContent_TCPMsgType_ECmd_Login:
                            case TCPCmdContent_TCPMsgType_ECmd_Exit:
                            {
                                // 说明是switch转发过来的
                                // 端需要同步给其他的
                                
                                std::map<std::string, std::shared_ptr<TCPConnectItem>>::iterator it ;
                                for (it = mConnMap.begin(); it != mConnMap.end(); it++)
                                {
                                    if (it->second->servkey() == mHostItem->getKey() && it->second->getKey() != clientItem->getKey())
                                    {
                                        // 向其转发
                                        send(it->second->mSocketID, buf, size, 0);
                                        
                                        std::ostringstream stream;
                                        if (len > 0)
                                        {
                                            stream << "向" << it->second->getKey() << "转发登录消息成功" << std::endl;
                                        }
                                        else
                                        {
                                            stream << "向" << it->second->getKey() << "转发登录消息失败" << std::endl;
                                        }
                                        
                                        onTipInfo(stream.str());
                                    }
                                }
                                
                                
                                break;
                            }
                            case TCPCmdContent_TCPMsgType_ECmd_Sync:
                            {
                                // 不可能收到
                                break;
                            }
                            
                            
                            case TCPCmdContent_TCPMsgType_ECmd_Nick:
                            {
                                // 不可能收到
                            }
                                
                                break;
                                
                            default:
                                break;
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
            else
            {
                std::string recvStr = "recv client (" + clientItem->mHost + ":" + clientItem->mPort + ") msg : " + std::string(buf);
                
                std::cout << recvStr << std::endl;
                
                onListenMsg(clientItem, std::string(buf));
            }
            
            delete msg;
            msg = NULL;
        }
        
    } while(!exit);
    
    close(clientItem->mSocketID);
    
    // 清理当前列表
    
    
}


void TCPServer::onSwitchMsg(char *buf, std::shared_ptr<TCPConnectItem> fromUser, std::shared_ptr<TCPConnectItem> toUser)
{
    
}

void TCPServer::onNotifyLogin(std::shared_ptr<TCPConnectItem>clientItem)
{
    // 通知已连接上的用户，item登录上了
    if (clientItem && mConnMap.size() > 1)
    {
        TCPMsg *msg = new TCPMsg();
        
        TCPUser *fromUser = new TCPUser();
        fromUser->set_hostip(mHostItem->mHost);
        fromUser->set_hostport(mHostItem->mPort);
        fromUser->set_nick(mHostItem->mNick);
        fromUser->set_servkey(mHostItem->mServKey);
        
        msg->set_allocated_fromuser(fromUser);
        msg->set_msgtype(TCPMsg_TCPMsgType_EMsg_Cmd);
        
        const std::string key = clientItem->getKey();
        std::map<std::string, std::shared_ptr<TCPConnectItem>>::iterator it ;
        for (it = mConnMap.begin(); it != mConnMap.end(); it++)
        {
            if (it->first != key)
            {
                TCPCmdContent *cmdContent = new TCPCmdContent;
                TCPUser *toUser = new TCPUser();
                toUser->set_hostip(it->second->mHost);
                toUser->set_hostport(it->second->mPort);
                toUser->set_nick(it->second->mNick);
                toUser->set_servkey(it->second->mServKey);
                cmdContent->set_allocated_touser(toUser);
                
                
                cmdContent->set_cmdtype(TCPCmdContent_TCPMsgType_ECmd_Login);
                
                TCPUser *paramUser = new TCPUser();
                paramUser->set_hostip(clientItem->mHost);
                paramUser->set_hostport(clientItem->mPort);
                paramUser->set_nick(clientItem->mNick);
                paramUser->set_servkey(clientItem->mServKey);
                
                ::google::protobuf::RepeatedPtrField< ::TCPUser > userList = cmdContent->userlist();
                // TODO:发消息
                userList.AddAllocated(paramUser);
                
                msg->set_allocated_cmdcontent(cmdContent);
                
                std::string str = msg->SerializeAsString();
                // 发送数据
                ssize_t len = send(it->second->mSocketID, (const void *)str.c_str(),  msg->ByteSize(), 0);
                std::ostringstream stream;
                if (len > 0)
                {
                    stream << "向" << it->second->getKey() << "转发" << clientItem->getKey() <<"上线消息成功" << std::endl;
                }
                else
                {
                    stream << "向" << it->second->getKey() << "转发" << clientItem->getKey() <<"上线消息失败" << std::endl;
                }
                
                onTipInfo(stream.str());
            }
        }
        delete msg;
        msg = NULL;
    }
    
}

void TCPServer::onNotifyNickName(std::shared_ptr<TCPConnectItem>clientItem)
{
    // 通知已连接上的用户，item登录上了
    if (clientItem && isVaildItem(clientItem))
    {
        TCPMsg *msg = new TCPMsg();
        
        TCPUser *fromUser = new TCPUser();
        fromUser->set_hostip(mHostItem->mHost);
        fromUser->set_hostport(mHostItem->mPort);
        fromUser->set_nick(mHostItem->mNick);
        
        msg->set_allocated_fromuser(fromUser);
        msg->set_msgtype(TCPMsg_TCPMsgType_EMsg_Cmd);
        
        TCPCmdContent *cmdContent = new TCPCmdContent;
        TCPUser *toUser = new TCPUser();
        toUser->set_hostip(clientItem->mHost);
        toUser->set_hostport(clientItem->mPort);
        toUser->set_nick(clientItem->mNick);
        cmdContent->set_allocated_touser(toUser);
        
        cmdContent->set_cmdtype(TCPCmdContent_TCPMsgType_ECmd_Nick);
        
        cmdContent->set_cmdparam(clientItem->mNick);
        
        msg->set_allocated_cmdcontent(cmdContent);
        
        std::string str = msg->SerializeAsString();
        // 发送数据
        ssize_t len = send(clientItem->mSocketID, (const void *)str.c_str(),  msg->ByteSize(), 0);
        std::ostringstream stream;
        if (len > 0)
        {
            stream << "给" << clientItem->getKey() <<"起Nick名：" << clientItem->mNick << "成功" << std::endl;
        }
        else
        {
            stream << "给" << clientItem->getKey() <<"起Nick名：" << clientItem->mNick << "失败" << std::endl;
        }
        
        onTipInfo(stream.str());
        
        delete msg;
        msg = NULL;
    }
}


void TCPServer::onNotifySync(std::shared_ptr<TCPConnectItem>clientItem)
{
    // 通知已连接上的用户，item登录上了
    if (clientItem && mConnMap.size() > 1)
    {
        TCPMsg *msg = new TCPMsg();
        
        TCPUser *fromUser = new TCPUser();
        fromUser->set_hostip(mHostItem->mHost);
        fromUser->set_hostport(mHostItem->mPort);
        fromUser->set_nick(mHostItem->mNick);
        fromUser->set_servkey(mHostItem->mServKey);
        
        msg->set_allocated_fromuser(fromUser);
        msg->set_msgtype(TCPMsg_TCPMsgType_EMsg_Cmd);
        
        
        TCPCmdContent *cmdContent = new TCPCmdContent;
        TCPUser *toUser = new TCPUser();
        toUser->set_hostip(clientItem->mHost);
        toUser->set_hostport(clientItem->mPort);
        toUser->set_nick(clientItem->mNick);
        toUser->set_servkey(clientItem->mServKey);
        cmdContent->set_allocated_touser(toUser);
        
        cmdContent->set_cmdtype(TCPCmdContent_TCPMsgType_ECmd_Sync);
        
        ::google::protobuf::RepeatedPtrField< ::TCPUser > userList = cmdContent->userlist();
        
        const std::string key = clientItem->getKey();
        std::map<std::string, std::shared_ptr<TCPConnectItem>>::iterator it;
        for (it = mConnMap.begin(); it != mConnMap.end(); it++)
        {
            if (it->first != key)
            {
                TCPUser *paramUser = new TCPUser();
                paramUser->set_hostip(it->second->mHost);
                paramUser->set_hostport(it->second->mPort);
                paramUser->set_nick(it->second->mNick);
                paramUser->set_servkey(it->second->mServKey);
                
                // TODO:发消息
                userList.AddAllocated(paramUser);
            }
        }
        
        
        msg->set_allocated_cmdcontent(cmdContent);
        
        std::string str = msg->SerializeAsString();
        // 发送数据
        ssize_t len = send(clientItem->mSocketID, (const void *)str.c_str(),  msg->ByteSize(), 0);
        std::ostringstream stream;
        if (len > 0)
        {
            stream << "向" << clientItem->getKey() << "同步用户列表成功" << std::endl;
        }
        else
        {
            stream << "向" << clientItem->getKey() << "同步用户列表失败" << std::endl;
        }
        
        onTipInfo(stream.str());
        
        
        delete msg;
        msg = NULL;
    }
}

void TCPServer::onNotifyExit(std::shared_ptr<TCPConnectItem>clientItem)
{
    
}

void TCPServer::onNotifyServerSwitchExit(std::shared_ptr<TCPConnectItem> clientItem)
{
    const std::string hostKey = mHostItem->mServKey;
    const std::string discswkey = clientItem->mServKey;
    if (clientItem && clientItem->mServKey == mHostItem->getKey())
    {
        mConnMap.erase(clientItem->getKey());
        // 说明是转发机断开了
        // 找到其他转发机
        
        // 找出所有断开的机子
        
        
        std::map<std::string, std::shared_ptr<TCPConnectItem>>::iterator it;
        
        for (it = mConnMap.begin(); it != mConnMap.end(); it++)
        {
            if (it->second->mServKey == clientItem->getKey())
            {
                it = mConnMap.erase(it);
                std::cout << it->second->getKey() << "下线" << std::endl;
            }
        }
        
        
        
        for (it = mConnMap.begin(); it != mConnMap.end(); it++)
        {
            // 向其他接口机同步列表
            if (it->second->mServKey == mHostItem->getKey())
            {
                onNotifySync(it->second);
            }
        }
    }
}

