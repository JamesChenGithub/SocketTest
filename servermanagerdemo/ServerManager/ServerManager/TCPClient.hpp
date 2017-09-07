//
//  TCPClient.hpp
//  ServerManager
//
//  Created by AlexiChen on 2017/9/5.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#ifndef TCPClient_hpp
#define TCPClient_hpp

#include <stdio.h>
#include "TCPServer.hpp"

class TCPClient : public TCPServer
{
public:
    ~TCPClient();
    TCPClient();
    TCPClient(std::shared_ptr<TCPConnectItem> item);
public:
    
    void testPrint();
    
};

#endif /* TCPClient_hpp */
