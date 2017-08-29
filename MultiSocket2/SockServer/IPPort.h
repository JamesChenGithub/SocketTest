//
//  IPPort.h
//  SockServer
//
//  Created by AlexiChen on 2017/8/25.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface IPPort : NSObject

@property (nonatomic, copy) NSString *ipAddr;
@property (nonatomic, assign) NSString *port;
@property (nonatomic, copy) NSString *nick;
@property (nonatomic, copy) NSString *message;

@end
