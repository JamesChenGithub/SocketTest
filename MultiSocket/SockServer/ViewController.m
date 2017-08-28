//
//  ViewController.m
//  SockServer
//
//  Created by AlexiChen on 2017/8/25.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#import "ViewController.h"
#import "IPTool.h"

#import <sys/socket.h>
#import <netinet/in.h>
#import <arpa/inet.h>
#import <string.h>
#import <ifaddrs.h>
#import <net/if.h>


// 0: server 1:client
#define kClientType 1

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _host = [[IPPort alloc] init];
    
    // 获取IP地址
    
    NSString *ip = @"0.0.0.";
    NSString *getIP = [IPTool getIPAddress:YES];
    if (getIP.length > 0)
    {
        ip = getIP;
    }
    
    ip = @"127.0.0.1";
    [_ipAdd setStringValue:ip];
    _ipAdd.enabled = NO;
    
    int por = 3333;//1024 + arc4random() % 5000;
    NSString *port = [NSString stringWithFormat:@"%d", por];
    [_port setStringValue:port];
    _port.enabled = NO;
    
    
    _host.ipAddr = ip;
    _host.port = port;
    _host.nick = _nick.stringValue;
    
    _textView.editable = NO;
    
    if (kClientType == 0)
    {
        _nick.stringValue = @"Server";
        [_startButton setTitle:@"启动"];
    }
    else
    {
        _nick.stringValue = @"Client";
        [_startButton setTitle:@"连接"];
    }
    
    _sendButton.enabled = NO;
    
    _acceptSocketIDs = [NSMutableArray array];
    // Do any additional setup after loading the view.
}

- (void)appendTextMsg:(NSString *)text
{
    dispatch_block_t block = ^{
        if (_textView.string.length)
        {
            
            if ([text hasSuffix:@"\n"])
            {
                NSString *newtxt = [[text componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]] componentsJoinedByString:@""];
                
                _textView.string = [_textView.string stringByAppendingString:[@"\n" stringByAppendingString:newtxt]];
            }
            else
            {
                
                _textView.string = [_textView.string stringByAppendingString:[@"\n" stringByAppendingString:text]];
            }
        }
        else
        {
            _textView.string = text;
        }
    };
    
    if ([NSThread isMainThread])
    {
        block();
    }
    else
    {
        dispatch_async(dispatch_get_main_queue(), block);
    }
}

- (IBAction)startTCP:(NSButton *)sender
{
    if (kClientType == 0)
    {
        if (_running)
        {
            _running = NO;
        }
        else
        {
            sender.enabled = NO;
            [self appendTextMsg:@"开始启动TCP Server"];
            _tcpThread = [[NSThread alloc] initWithTarget:self selector:@selector(startTCPServer) object:nil];
            _tcpThread.name = @"TCPServer";
            [_tcpThread start];
            
            
        }
        
        sender.enabled = NO;
    }
    else
    {
        if (_running)
        {
            _running = NO;
        }
        else
        {
            [self appendTextMsg:@"开始启动TCP Client"];
            if (!_tcpThread)
            {
                _tcpThread = [[NSThread alloc] initWithTarget:self selector:@selector(startTCPClient) object:nil];
                _tcpThread.name = @"TCPServer";
                [_tcpThread start];
            }
            
        }
        
        sender.enabled = NO;
        
    }
}


- (void)startTCPClient
{
    int err = 0;
    int socketid = socket(AF_INET, SOCK_STREAM, 0);
    
    if (socketid == -1)
    {
        [self appendTextMsg:@"socket create fail"];
    }
    else
    {
        //        struct sockaddr_in addr;
        //        memset(&addr, 0, sizeof(addr));
        //        addr.sin_len = sizeof(addr);
        //        addr.sin_family = AF_INET;
        //        addr.sin_addr.s_addr = INADDR_ANY;
        //        err = bind(socketid, (const struct sockaddr *)&addr, sizeof(addr));
        if (err == 0)
        {
            [self appendTextMsg:@"socket bind succ"];
            
            struct sockaddr_in peeraddr;
            memset(&peeraddr, 0, sizeof(peeraddr));
            peeraddr.sin_len = sizeof(peeraddr);
            peeraddr.sin_family = AF_INET;
            peeraddr.sin_port = htons(_port.intValue);
            peeraddr.sin_addr.s_addr = inet_addr([_ipAdd.stringValue UTF8String]);
            
            
            err = connect(socketid, (const struct sockaddr *)&peeraddr, sizeof(peeraddr));
            
            if (err == 0)
            {
                _running = YES;
                [self appendTextMsg:@"socket connect succ"];
                
                _socketId = socketid;
                
                dispatch_async(dispatch_get_main_queue(), ^{
                    _sendButton.enabled = YES;
                });
                
                
                while (_running)
                {
                    char buf[1024];
                    memset(&buf, 0, sizeof(buf));
                    recv(socketid, &buf, sizeof(buf), 0);
                    
                    printf("recv from server : %s\n", buf);
                    
                    NSString *recv = [NSString stringWithUTF8String:buf];
                    
                    [self appendTextMsg:[NSString stringWithFormat:@"recv from server : %@", recv]];
                    
                    
                }
            }
            else
            {
                [self appendTextMsg:@"socket connect fail"];
                
            }
        }
        else
        {
            [self appendTextMsg:@"socket bind fail"];
            
        }
    }
}

- (void)startTCPServer
{
    int socketid = socket(AF_INET, SOCK_STREAM, 0);
    if (socketid == -1)
    {
        [self appendTextMsg:@"socket create fail"];
        dispatch_async(dispatch_get_main_queue(), ^{
            _startButton.enabled = YES;
        });
        return;
    }
    else
    {
        [self appendTextMsg:@"socket create succ"];
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port.intValue);
    addr.sin_addr.s_addr = inet_addr([_ipAdd.stringValue UTF8String]);//0.0.0.0,本机任意地址（可能有多个网卡）
    
    int err = bind(socketid, (const struct sockaddr *)&addr, sizeof(addr));
    if (err != 0)
    {
        NSString *error = [NSString stringWithFormat:@"socket bind failed : %d", err];
        [self appendTextMsg:error];
        dispatch_async(dispatch_get_main_queue(), ^{
            _startButton.enabled = YES;
        });
        
        return;
    }
    else
    {
        [self appendTextMsg:@"socket bind succ"];
    }
    
    
    int liserr = listen(socketid, 10);
    if (liserr != 0)
    {
        NSString *error = [NSString stringWithFormat:@"socket listen failed : %d", err];
        [self appendTextMsg:error];
        dispatch_async(dispatch_get_main_queue(), ^{
            _startButton.enabled = YES;
        });
        
        return;
    }
    else
    {
        [self appendTextMsg:@"socket listen succ"];
    }
    
    _running = YES;
    [self appendTextMsg:@"socket accepting"];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [_startButton setTitle:@"停止"];
    });
    
    while (_running)
    {
        struct sockaddr_in peerAddr;
        
        socklen_t addrlen = sizeof(peerAddr);
        int peerfd = accept(socketid, (struct sockaddr *)&peerAddr, &addrlen);
        
        
        if (peerfd != -1)
        {
            dispatch_async(dispatch_get_main_queue(), ^{
                _sendButton.enabled = YES;
            });
            
            char *client_ip = inet_ntoa(peerAddr.sin_addr);
            int client_port = ntohs(peerAddr.sin_port);
            
            NSString *cipstr = [NSString stringWithUTF8String:client_ip];
            [self appendTextMsg:[NSString stringWithFormat:@"accept %@:%d ", cipstr, client_port]];
            
            [_acceptSocketIDs addObject:@(peerfd)];
            
            [NSThread detachNewThreadWithBlock:^{
                BOOL exit = NO;
                do
                {
                    char buf[1025];
                    memset(&buf, 0, sizeof(buf));
                    ssize_t count;
                    size_t len = sizeof(buf);
                    
                    count = recv(peerfd, buf, 1024, 0);
                    
                    NSString *recvStr = [NSString stringWithUTF8String:buf];
                    
                    [self appendTextMsg:[NSString stringWithFormat:@"recv client(%@:%d)  : %@", cipstr, client_port, recvStr]];
                    
                    exit = [recvStr containsString:@"exit"];
                    
                    
                } while(!exit);
                
                [self appendTextMsg:[NSString stringWithFormat:@"%@:%d exit", cipstr, client_port]];
                [_acceptSocketIDs removeObject:@(peerfd)];
                close(peerfd);
            }];
            
            
        }
    }
}

- (IBAction)onSendMsg:(NSButton *)sender
{
    NSString *text = _sendMsg.stringValue;
    if (text.length)
    {
        
        if (kClientType)
        {
            ssize_t len = send(_socketId, [text UTF8String], 1024, 0);
            if (len > 0)
            {
                [self appendTextMsg:[NSString stringWithFormat:@"client send succ  : %@", text]];
            }
            else
            {
                [self appendTextMsg:[NSString stringWithFormat:@"client send fail  : %@", text]];
            }
        }
        else
        {
            
            for (NSNumber *socid in _acceptSocketIDs)
            {
                ssize_t len = send([socid intValue], [text UTF8String], 1024, 0);
                if (len > 0)
                {
                    [self appendTextMsg:[NSString stringWithFormat:@"server send succ  : %@", text]];
                }
                else
                {
                    [self appendTextMsg:[NSString stringWithFormat:@"server send fail  : %@", text]];
                }
            }
        }
        
    }
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
    
    // Update the view, if already loaded.
}


@end
