//
//  NSCache+YSCBGCache.m
//  YSCBGFMDB
//
//  Created by biao on 2017/10/17.
//  Copyright © 2017年 Biao. All rights reserved.
//

#import "NSCache+YSCBGCache.h"

static NSCache* keyCaches;
@implementation NSCache (YSCBGCache)

+(instancetype)YSCBG_cache{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        keyCaches = [NSCache new];
    });
    return keyCaches;
}

@end
