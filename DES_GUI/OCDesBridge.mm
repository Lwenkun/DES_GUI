//
//  OCDesBridge.m
//  DES_GUI
//
//  Created by 李文坤 on 2017/5/14.
//  Copyright © 2017年 李文坤. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "OCDesBridge.h"
#import "des.hpp"


@implementation OCDesBridge

+ (void)encrypt: (NSString*)inputFilePath :(NSString*)outputFilePath :(NSString*)hex_key {
    des().des_encrypt([inputFilePath UTF8String], [outputFilePath UTF8String], [hex_key UTF8String]);
}

+ (void)decrypt: (NSString*)inputFilePath :(NSString*)outputFilePath :(NSString*)hex_key {
    des().des_decrypt([inputFilePath UTF8String], [outputFilePath UTF8String], [hex_key UTF8String]);
}

@end
