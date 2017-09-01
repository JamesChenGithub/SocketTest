//
//  IPPort.h
//  SockServer
//
//  Created by AlexiChen on 2017/8/25.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface IPPort : NSObject

@property (nonatomic, copy) NSString *ipAddrPort;
@property (nonatomic, copy) NSString *nick;

@end



typedef NS_ENUM(int, MsgConType){
    EC2C,
    EGroup,
    ESystem,
    EService
};


typedef NS_ENUM(int, MsgType) {
    ECMD,
    EMsg
};




@interface HostMsg : NSObject

@property (nonatomic, copy) NSString *from;
@property (nonatomic, copy) NSString *to;
@property (nonatomic, assign) int convType;
@property (nonatomic, assign) int msgType;
@property (nonatomic, assign) int subMsgType;
@property (nonatomic, copy) NSString *msg;
- (NSData *)toJsonData;
@end


typedef NS_ENUM(uint32, HostMsgDataKey) {
    EData_Start = 0xFEFEFEFE,
    EData_End = 0xEFEFEFEF
};

@interface HostMsgData : NSObject

@property (nonatomic, assign) uint32 head;
@property (nonatomic, assign) uint32 len;
@property (nonatomic, strong) HostMsg *msg;
@property (nonatomic, assign) uint32 end;

- (NSData *)toData;

+ (instancetype)parseFrom:(NSData *)netData;

@end


