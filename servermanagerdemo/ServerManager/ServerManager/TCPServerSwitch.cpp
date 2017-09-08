//
//  TCPServerSwitch.cpp
//  ServerManager
//
//  Created by AlexiChen on 2017/9/5.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#include "TCPServerSwitch.hpp"
#include "TCPUser.pb.h"
#import <sys/socket.h>
#import <netinet/in.h>
#import <arpa/inet.h>
#import <string.h>
#import <ifaddrs.h>
#import <net/if.h>
#import <unistd.h>

TCPServerSwitch::~TCPServerSwitch()
{
    std::cout << "TCPServerSwitch() desconstructor" << std::endl;
}


TCPServerSwitch::TCPServerSwitch()
{
    std::cout << "TCPServerSwitch() constructor" << std::endl;
}

TCPServerSwitch::TCPServerSwitch(std::shared_ptr<TCPConnectItem>  item):TCPServer(item)
{
    
}

bool TCPServerSwitch::start()
{
    bool succ = TCPServer::start();
    
    // todo 连接serverManager
    
    std::shared_ptr<TCPConnectItem>  itemptr(new TCPConnectItem());
    itemptr->mHost = std::string("127.0.0.1");
    itemptr->mPort = std::string("6666");
    this->connectServer(itemptr);
    return succ;

}

bool TCPServerSwitch::connectServer(std::shared_ptr<TCPConnectItem>  item)
{
    if (!isVaildItem(item))
    {
        onListenError(-1, "ip地址或端口有误");
        return false;
    }
    
    std::unique_ptr<TCPConnectItem> ptr(new TCPConnectItem(*item));
    mServerMgrItem = std::move(ptr);
    
    int socketID = socket(AF_INET, SOCK_STREAM, 0);
    if (socketID < 0)
    {
        onListenError(socketID, "创建socket失败");
        return false;
    }
    
    onTipInfo("创建Socket成功");
    
    mServerMgrItem->mSocketID = socketID;
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(mServerMgrItem->mPort.c_str()));
    addr.sin_addr.s_addr = inet_addr(mServerMgrItem->mHost.c_str());
    
    int err = connect(mServerMgrItem->mSocketID, (const struct sockaddr *)&addr, sizeof(addr));
    if (err != 0)
    {
        onListenError(socketID, "connect socket失败");
        return false;
    }
    
    onTipInfo("connect Socket成功");
    
    mRecvThread = std::thread([this]{
        onTipInfo("开始接收ServerManger数据");
        this->onRecvMsgFromServer();
    });
    mRecvThread.detach();
    return true;
}

void TCPServerSwitch::onRecvMsgFromServer()
{
    mRunning = true;
    
    while (mRunning)
    {
        const int kMaxCacheSize = 2049;
        char buf[kMaxCacheSize];
        memset(&buf, 0, sizeof(buf));
        ssize_t size =  recv(mServerMgrItem->mSocketID, &buf, sizeof(buf) - 1, 0);
        
        if (size <= 0)
        {
            break;
        }
        else
        {
            printf("recv from server : %s\n", buf);
            
            TCPMsg *msg = new TCPMsg();
            bool succ = msg->ParseFromArray(buf, (int)size);
            if (succ)
            {
                if (msg->has_msgtype())
                {
                    TCPMsg_TCPMsgType type = msg->msgtype();
                    if (type == TCPMsg_TCPMsgType_EMsg_Msg)
                    {
                        // get FromUser
                        TCPUser *fromuser = msg->mutable_fromuser();
                        
                        
                        std::shared_ptr<TCPConnectItem>  item(new TCPConnectItem());
                        item->mHost = fromuser->hostip();
                        item->mPort = fromuser->hostport();
                        item->mNick = fromuser->nick();
                        
                        TCPMsgContent *msgContent = msg->mutable_msgcontent();
                        
//                        TCPUser *toUSer = msgContent->mutable_touser();
                        
                        std::string msgInfo = msgContent->msgcontent();
                        
//                        onRecvMsg(mServerMgrItem, std::string(msgInfo));
                        
                    }
                    else if (type == TCPMsg_TCPMsgType_EMsg_Cmd)
                    {
                        // get FromUser
//                        TCPUser *fromuser = msg->mutable_fromuser();
                        TCPCmdContent *cmd = msg->mutable_cmdcontent();
                        TCPCmdContent_TCPMsgType cmdType = cmd->cmdtype();
                        switch (cmdType)
                        {
                            case TCPCmdContent_TCPMsgType_ECmd_Login:
                            {
                                // 收到转上线用户通知,加到用户列表
//                                TCPUser *toUSer = cmd->mutable_touser();
                                
//                                ::google::protobuf::RepeatedPtrField< ::TCPUser > *userList = cmd->mutable_userlist();
                                
                                break;
                            }
                            case TCPCmdContent_TCPMsgType_ECmd_Sync:
                            {
                                // 节点回复上线用户列表，添加到自身列表中
//                                TCPUser *toUSer = cmd->mutable_touser();
                                
//                                ::google::protobuf::RepeatedPtrField< ::TCPUser > *userList = cmd->mutable_userlist();
                                break;
                            }
                            case TCPCmdContent_TCPMsgType_ECmd_Exit:
                            {
                                // 收到转上线用户通知,移除用户列表
                                break;
                            }
                            default:
                                break;
                        }
                        
                        

                    }
                }
            }
            
            delete msg;
            msg = NULL;
            
            
        }
    }
    

    std::shared_ptr<TCPConnectItem>  nit(new TCPConnectItem(*mServerMgrItem));
    close(mServerMgrItem->mSocketID);
    onExitConnect(nit);
}



void TCPServerSwitch::testPrint()
{
    TCPServer::testPrint();
    std::cout << "TCPServerSwitch::testPrint()" << std::endl;
}
