//
//  YSCBGModelInfo.m
//  YSCBGFMDB
//
//  Created by huangzhibiao on 17/2/22.
//  Copyright © 2017年 Biao. All rights reserved.
//

#import "YSCBGModelInfo.h"
#import "YSCBGTool.h"
#import "YSCBGFMDBConfig.h"

@implementation YSCBGModelInfo

+(NSArray<YSCBGModelInfo*>*)modelInfoWithObject:(id)object{
    NSMutableArray* modelInfos = [NSMutableArray array];
    NSArray* keyAndTypes = [YSCBGTool getClassIvarList:[object class] Object:object onlyKey:NO];
    for(NSString* keyAndType in keyAndTypes){
        NSArray* keyTypes = [keyAndType componentsSeparatedByString:@"*"];
        NSString* propertyName = keyTypes[0];
        NSString* propertyType = keyTypes[1];
        
        YSCBGModelInfo* info = [YSCBGModelInfo new];
        //设置属性名
        [info setValue:propertyName forKey:@"propertyName"];
        //设置属性类型
        [info setValue:propertyType forKey:@"propertyType"];
        //设置列名(YSCBG_ + 属性名),加YSCBG_是为了防止和数据库关键字发生冲突.
        [info setValue:[NSString stringWithFormat:@"%@%@",YSCBG,propertyName] forKey:@"sqlColumnName"];
        //设置列属性
        NSString* sqlType = [YSCBGTool getSqlType:propertyType];
        [info setValue:sqlType forKey:@"sqlColumnType"];
            
        id propertyValue;
        id sqlValue;
        //crateTime和updateTime两个额外字段单独处理.
        if([propertyName isEqualToString:YSCBG_createTimeKey] ||
           [propertyName isEqualToString:YSCBG_updateTimeKey]){
            propertyValue = [YSCBGTool stringWithDate:[NSDate new]];
        }else{
            propertyValue = [object valueForKey:propertyName];
        }
        
        if(propertyValue){
            //设置属性值
            [info setValue:propertyValue forKey:@"propertyValue"];
            sqlValue = [YSCBGTool getSqlValue:propertyValue type:propertyType encode:YES];
            //非系统类型特殊处理sqlValue
            if(![YSCBGTool isKindOfSystemType:propertyType]){
                //设置将要存储到数据库的值
                sqlValue = [NSString stringWithFormat:@"%@%@%@",propertyType,YSCBG_CUSTOM_TYPE_SEPARATOR,sqlValue];
            }
            [info setValue:sqlValue forKey:@"sqlColumnValue"];
            [modelInfos addObject:info];
        }
        
    }
    NSAssert(modelInfos.count,@"对象变量数据为空,不能存储!");
    return modelInfos;
}


@end
