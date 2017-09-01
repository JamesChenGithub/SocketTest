//
//  IPPort.m
//  SockServer
//
//  Created by AlexiChen on 2017/8/25.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#import "IPPort.h"

#include "NSObject+Json.h"

@implementation IPPort

@end


@implementation HostMsg

- (instancetype)init
{
    if (self = [super init])
    {
        _convType = EC2C;
        _msgType = EMsg;
        _subMsgType = 0;
    }
    return self;
}

- (NSData *)toJsonData
{
    NSDictionary *dic = [self serializeToJsonObject];
    if([NSJSONSerialization isValidJSONObject:dic])
    {
        NSError *error = nil;
        NSData *data = [NSJSONSerialization dataWithJSONObject:dic options:NSJSONWritingPrettyPrinted error:&error];
        if(error)
        {
            return nil;
        }
        
        return data;
    }
    
    return nil;
}


@end

@implementation HostMsgData

- (instancetype)init
{
    if (self = [super init])
    {
        _head = EData_Start;
        _end = EData_End;
    }
    return self;
}

- (NSData *)toData
{
    NSData *msgData = [_msg toJsonData];
    if (msgData.length)
    {
        _len = (uint32)msgData.length;
        NSMutableData *toData = [NSMutableData dataWithLength:_len + 3 * sizeof(uint32)];
        void *byte = (void *)toData.bytes;
        memcpy(byte, (void *)&_head, sizeof(uint32));
        memcpy(byte + sizeof(uint32), (void *)&_len, sizeof(uint32));
        memcpy(byte +  2 * sizeof(uint32), msgData.bytes, _len);
        memcpy(byte +  2 * sizeof(uint32) + _len, ( void *)&_end, sizeof(uint32));
        
        NSLog(@"%@", toData);
        return toData;
    }
    return nil;
}

+ (instancetype)parseFrom:(NSData *)netData
{
    return nil;
}
@end
