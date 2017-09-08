//
//  ViewController.m
//  ServerManager
//
//  Created by AlexiChen on 2017/9/1.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#import "ViewController.h"
#include <sstream>
#include "TCPClient.hpp"
#include "TCPServerSwitch.hpp"



@implementation ViewController

- (void)dealloc
{
    
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView
{
    if (_tcpServer)
    {
        std::map<std::string, std::shared_ptr<TCPConnectItem> > map = _tcpServer->getConnectMap();
        return map.size();
    }
    return 0;
}

/* Notifications */


- (void)comboBoxSelectionDidChange:(NSNotification *)notification
{
    _typeIndex = _typeBox.indexOfSelectedItem;
    
    switch (_typeIndex)
    {
        case 0:
            _ipAddr.enabled = NO;
            _ipPort.enabled = NO;
            _tableView.enabled = YES;
            break;
            
        case 1:
            _ipAddr.enabled = NO;
            _ipPort.enabled = YES;
            _tableView.enabled = YES;
            break;
        case 2:
            _ipAddr.enabled = NO;
            _ipPort.enabled = YES;
            _tableView.enabled = NO;
            break;
            
            
        default:
            break;
    }
}

- (NSString *)getIPPortAtRow:(NSInteger)row
{
    std::map<std::string, std::shared_ptr<TCPConnectItem> > map = _tcpServer->getConnectMap();
    
    std::map<std::string, std::shared_ptr<TCPConnectItem> >::iterator it = map.begin();
    int c = 0;
    for (; it != map.end(); it++)
    {
        if ( c == row)
        {
            break;
        }
        c++;
    }
    
    std::ostringstream stream ;
    stream << it->second->mHost << ":" << it->second->mPort;
    NSString *hostport = [NSString stringWithUTF8String:stream.str().c_str()];
    return hostport;
}
//-(id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row {
//    
//    
//    
//}

- (CGFloat)tableView:(NSTableView *)tableView sizeToFitWidthOfColumn:(NSInteger)column NS_AVAILABLE_MAC(10_6)
{
    if (column == 1)
    {
        return 0;
    }
    return 200;
}

- (NSView *)tableView:(NSTableView *)tableView viewForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row {
    
    // Get a new ViewCell
    NSTableCellView *cellView = [tableView makeViewWithIdentifier:tableColumn.identifier owner:self];
    
    // Since this is a single-column table view, this would not be necessary.
    // But it's a good practice to do it in order by remember it when a table is multicolumn.
    if( [tableColumn.identifier isEqualToString:@"AutomaticTableColumnIdentifier.0"] )
    {
        cellView.textField.stringValue = [self getIPPortAtRow:row];
        return cellView;
    }
    else if ([tableColumn.identifier isEqualToString:@"AutomaticTableColumnIdentifier.1"])
    {
        return nil;
    }
    return cellView;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // Do any additional setup after loading the view.
    _tableView.delegate = self;
    _tableView.dataSource = self;
    
    _textView.editable = NO;
    
    _typeIndex = 0;
    [_typeBox selectItemAtIndex:_typeIndex];
    
    _msgText.enabled = NO;
    _sendMsg.enabled = NO;
}

- (IBAction)onStartServer:(NSButton *)sender
{
    std::shared_ptr<TCPConnectItem> itemptr(new TCPConnectItem());
    itemptr->mHost = std::string([_ipAddr.stringValue UTF8String]);
    itemptr->mPort = std::string([_ipPort.stringValue UTF8String]);
    
    if (!_tcpServer)
    {
        
        switch (_typeIndex)
        {
            case 0:
            {
                itemptr->mNick = "servermanager";
                std::shared_ptr<TCPServer> ser(new TCPServer(itemptr));
                _tcpServer = std::move(ser);
                break;
            }
            case 1:
            {
                itemptr->mNick = "serverswitch";
                std::shared_ptr<TCPServer> ser(new TCPServerSwitch(itemptr));
                _tcpServer = std::move(ser);
                break;
            }
            case 2:
            {
                itemptr->mNick = "client";
                std::shared_ptr<TCPServer> ser(new TCPClient());
                _tcpServer = std::move(ser);
            }   break;
                
                
            default:
                break;
        }

        std::shared_ptr<TCPServerMonitor> sermon(new TCPServerMonitor(self));
        _TCPObserver = std::move(sermon);
        _tcpServer->setListener(_TCPObserver);
        
    }
    
    bool succ = false;
    if (_typeIndex != 2)
    {
        succ = _tcpServer->start();
        _msgText.enabled = !succ;
        _sendMsg.enabled = !succ;
    }
    else
    {
        succ = _tcpServer->connectServer(itemptr);
        _msgText.enabled = succ;
        _sendMsg.enabled = succ;
    }
    
    
    sender.enabled = !succ;
    _ipAddr.enabled = NO;
    _ipPort.enabled = NO;
    _typeBox.enabled = NO;
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
    
    // Update the view, if already loaded.
}

- (void)appendTextMsg:(NSString *)text
{
    dispatch_block_t block = ^{
        
        NSLog(@"%@", text);
        if (_textView.string.length)
        {
            if ([text hasSuffix:@"\n"])
            {
                NSAttributedString* attr = [[NSAttributedString alloc] initWithString:text];
                [[_textView textStorage] appendAttributedString:attr];
            }
            else
            {
                NSAttributedString* attr = [[NSAttributedString alloc] initWithString:[@"\n" stringByAppendingString:text]];
                [[_textView textStorage] appendAttributedString:attr];
            }
            [_textView scrollRangeToVisible:NSMakeRange([[_textView string] length], 0)];
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

- (IBAction)onSendMsg:(NSButton *)sender
{
    
}

- (void)onTipInfo:(std::string)string
{
    NSString *s = [NSString stringWithUTF8String:string.c_str()];
    [self appendTextMsg:s];
}
- (void)onListenError:(int)err errorInfo:(std::string)string
{
    std::ostringstream stream;
    stream << "Listen Error : (" << err << "," << string << ")" ;
    NSString *s = [NSString stringWithUTF8String:stream.str().c_str()];
    [self appendTextMsg:s];
}

- (void)onConnectError:(int)err errorInfo:(std::string)string
{
    std::ostringstream stream;
    stream << "Connect Error : (" << err << "," << string << ")" ;
    NSString *s = [NSString stringWithUTF8String:stream.str().c_str()];
    [self appendTextMsg:s];
}

- (void)onRecvMsgFrom:(std::shared_ptr<TCPConnectItem> )item message:(std::string)msg
{
    if (item)
    {
        std::ostringstream stream;
        stream << "Recv From (" << item->mHost << ":" << item->mPort << ") :" << msg ;
        NSString *s = [NSString stringWithUTF8String:stream.str().c_str()];
        [self appendTextMsg:s];
    }
    
}

- (void)onAcceptConnect:(std::shared_ptr<TCPConnectItem> )item
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [_tableView reloadData];
    });
}

- (void)onAcceptConnectList:(std::list<std::shared_ptr<TCPConnectItem> >)itemList
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [_tableView reloadData];
    });
}



- (void)onExitConnect:(std::shared_ptr<TCPConnectItem> )item
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [_tableView reloadData];
    });
}


@end


TCPServerMonitor::TCPServerMonitor(ViewController *objc)
{
    mWeakObserverRef = objc;
}

//TCPServerMonitor::TCPServerMonitor()
//{
//    mWeakObserverRef = nil;
//}
//
//TCPServerMonitor::~TCPServerMonitor()
//{
//    mWeakObserverRef = nil;
//}

void TCPServerMonitor::onTipInfo(std::string info)
{
    if (mWeakObserverRef)
    {
        [mWeakObserverRef onTipInfo:info];
    }
}

void TCPServerMonitor::onLinterError(int err, std::string errorinfo)
{
    if (mWeakObserverRef)
    {
        [mWeakObserverRef onListenError:err errorInfo:errorinfo];
    }
}

void TCPServerMonitor::onConnectError(int err, std::string errinfo)
{
    if (mWeakObserverRef)
    {
        [mWeakObserverRef onConnectError:err errorInfo:errinfo];
    }
}
void TCPServerMonitor::onRecvMsgFrom(std::shared_ptr<TCPConnectItem> item, std::string msg)
{
    if (mWeakObserverRef)
    {
        [mWeakObserverRef onRecvMsgFrom:item message:msg];
    }
}

void TCPServerMonitor::onAcceptConnect(std::shared_ptr<TCPConnectItem> item)
{
    if (mWeakObserverRef)
    {
        [mWeakObserverRef onAcceptConnect:item];
    }
}

void TCPServerMonitor::onExitConnect(std::shared_ptr<TCPConnectItem> item)
{
    if (mWeakObserverRef)
    {
        [mWeakObserverRef onExitConnect:item];
    }
}

void TCPServerMonitor::onAcceptConnectList(std::list<std::shared_ptr<TCPConnectItem> > itemList)
{
    if (mWeakObserverRef)
    {
        [mWeakObserverRef onAcceptConnectList:itemList];
    }
}
