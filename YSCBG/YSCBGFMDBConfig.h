//
//  YSCBGFMDBConfig.h
//  YSCBGFMDB
//
//  Created by biao on 2017/7/19.
//  Copyright © 2017年 Biao. All rights reserved.
//

#ifndef YSCBGFMDBConfig_h
#define YSCBGFMDBConfig_h

// 过期方法注释
#define YSCBGFMDBDeprecated(instead) NS_DEPRECATED(2_0, 2_0, 2_0, 2_0, instead)

#define YSCBG_primaryKey @"YSCBG_id"
#define YSCBG_createTimeKey @"YSCBG_createTime"
#define YSCBG_updateTimeKey @"YSCBG_updateTime"

//keyPath查询用的关系，YSCBG_equal:等于的关系；YSCBG_contains：包含的关系.
#define YSCBG_equal @"Relation_Equal"
#define YSCBG_contains @"Relation_Contains"

#define YSCBG_complete_B void(^_Nullable)(BOOL isSuccess)
#define YSCBG_complete_I void(^_Nullable)(YSCBG_dealState result)
#define YSCBG_complete_A void(^_Nullable)(NSArray* _Nullable array)
#define YSCBG_changeBlock void(^_Nullable)(YSCBG_changeState result)

typedef NS_ENUM(NSInteger,YSCBG_changeState){//数据改变状态
    YSCBG_insert,//插入
    YSCBG_update,//更新
    YSCBG_delete,//删除
    YSCBG_drop//删表
};

typedef NS_ENUM(NSInteger,YSCBG_dealState){//处理状态
    YSCBG_error = -1,//处理失败
    YSCBG_incomplete = 0,//处理不完整
    YSCBG_complete = 1//处理完整
};

typedef NS_ENUM(NSInteger,YSCBG_sqliteMethodType){//sqlite数据库原生方法枚举
    YSCBG_min,//求最小值
    YSCBG_max,//求最大值
    YSCBG_sum,//求总和值
    YSCBG_avg//求平均值
};

typedef NS_ENUM(NSInteger,YSCBG_dataTimeType){
    YSCBG_createTime,//存储时间
    YSCBG_updateTime,//更新时间
};

/**
 封装处理传入数据库的key和value.
 */
extern NSString* _Nonnull YSCBG_sqlKey(NSString* _Nonnull key);
/**
 转换OC对象成数据库数据.
 */
extern NSString* _Nonnull YSCBG_sqlValue(id _Nonnull value);
/**
 根据keyPath和Value的数组, 封装成数据库语句，来操作库.
 */
extern NSString* _Nonnull YSCBG_keyPathValues(NSArray* _Nonnull keyPathValues);
/**
 直接执行sql语句;
 @tablename nil时以cla类名为表名.
 @cla 要操作的类,nil时返回的结果是字典.
 提示：字段名要增加YSCBG_前缀
 */
extern id _Nullable YSCBG_executeSql(NSString* _Nonnull sql,NSString* _Nullable tablename,__unsafe_unretained _Nullable Class cla);
/**
 自定义数据库名称.
 */
extern void YSCBG_setSqliteName(NSString*_Nonnull sqliteName);
/**
 删除数据库文件
 */
extern BOOL YSCBG_deleteSqlite(NSString*_Nonnull sqliteName);
/**
 设置操作过程中不可关闭数据库(即closeDB函数无效).
 默认是NO.
 */
extern void YSCBG_setDisableCloseDB(BOOL disableCloseDB);
/**
 设置调试模式
 @debug YES:打印调试信息, NO:不打印调试信息.
 */
extern void YSCBG_setDebug(BOOL debug);

/**
 事务操作.
 返回YES提交事务, 返回NO回滚事务.
 */
extern void YSCBG_inTransaction(BOOL (^ _Nonnull block)(void));

/**
 清除缓存
 */
extern void YSCBG_cleanCache(void);

#endif /* YSCBGFMDBConfig_h */
