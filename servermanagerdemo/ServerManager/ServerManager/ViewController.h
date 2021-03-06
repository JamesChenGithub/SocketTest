//
//  ViewController.h
//  ServerManager
//
//  Created by AlexiChen on 2017/9/1.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "TCPServer.hpp"

class TCPServerMonitor;

@interface ViewController : NSViewController<NSTableViewDelegate, NSTableViewDataSource, NSComboBoxDelegate>
{
    std::shared_ptr<TCPServer> _tcpServer;
    std::shared_ptr<TCPServerMonitor> _TCPObserver;
    
    NSInteger _typeIndex;
}

@property (weak) IBOutlet NSTextField *nickName;
@property (weak) IBOutlet NSTextField *ipAddr;


@property (weak) IBOutlet NSTextField *ipPort;
@property (weak) IBOutlet NSTextView *textView;
@property (weak) IBOutlet NSTableView *tableView;

@property (weak) IBOutlet NSComboBox *typeBox;

@property (weak) IBOutlet NSTextField *msgText;
@property (weak) IBOutlet NSButton *sendMsg;
@end



class TCPServerMonitor : public TCPServerListener
{
private:
    __weak ViewController *mWeakObserverRef;
    
public:
    ~TCPServerMonitor()
    {
        mWeakObserverRef = nil;
    }
    TCPServerMonitor()
    {
        mWeakObserverRef = nil;
    }
    
    
    TCPServerMonitor(ViewController *viewObj);
    
    virtual void onTipInfo(std::string info);
    
    virtual void onLinterError(int err, std::string errorinfo);
    
    virtual void onConnectError(int err, std::string errinfo);
    
    virtual void onRecvMsgFrom(std::shared_ptr<TCPConnectItem> item, std::string msg);
    
    virtual void onAcceptConnect(std::shared_ptr<TCPConnectItem> item);
    
    virtual void onAcceptConnectList(std::list<std::shared_ptr<TCPConnectItem> > itemList);
    
    virtual void onExitConnect(std::shared_ptr<TCPConnectItem> item);
};

