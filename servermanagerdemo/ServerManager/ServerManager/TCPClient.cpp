//
//  TCPClient.cpp
//  ServerManager
//
//  Created by AlexiChen on 2017/9/5.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#include "TCPClient.hpp"


TCPClient::~TCPClient()
{
    
}


TCPClient::TCPClient()
{
    
}
TCPClient::TCPClient(std::shared_ptr<TCPConnectItem> item):TCPServer(item)
{
    
}




void TCPClient::testPrint()
{
    TCPServer::testPrint();
    std::cout << "TCPClient::testPrint()" << std::endl;
    
//    std::count << (uint64_t)(&TCPServer::testPrint()) <<  (uint64_t)(&TCPClient::testPrint()) << std::endl;
}
