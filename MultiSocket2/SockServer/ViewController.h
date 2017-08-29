//
//  ViewController.h
//  SockServer
//
//  Created by AlexiChen on 2017/8/25.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "IPPort.h"

@interface ViewController : NSViewController
{
    IPPort *_host;
    
    BOOL _running;
    NSThread *_tcpThread;
    
    int _socketId;
    
    NSMutableArray *_acceptSocketIDs;
}

@property (weak) IBOutlet NSTextField *ipAdd;

@property (weak) IBOutlet NSTextField *port;

@property (weak) IBOutlet NSTextField *nick;

@property (weak) IBOutlet NSButton *startButton;

@property (weak) IBOutlet NSTextView *textView;

@property (weak) IBOutlet NSTextView *sendMsg;

@property (weak) IBOutlet NSButton *sendButton;

@end

