//
//  OCDesBridge.h
//  DES_GUI
//
//  Created by 李文坤 on 2017/5/13.
//  Copyright © 2017年 李文坤. All rights reserved.
//
#import <Foundation/Foundation.h>

@interface OCDesBridge : NSObject

+ (void)encrypt: (NSString*)inputFilePath :(NSString*)outputFilePath :(NSString*)hex_key;
+ (void)decrypt: (NSString*)inputFilePath :(NSString*)outputFilePath :(NSString*)hex_key;

@end

