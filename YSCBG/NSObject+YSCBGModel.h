//
//  NSObject+YSCBGModel.h
//  YSCBGFMDB
//
//  Created by huangzhibiao on 17/2/28.
//  Copyright © 2017年 Biao. All rights reserved.
//  温馨提示，同步：线程阻塞；异步：线程非阻塞;
/**
YSCBGFMDB全新升级->>
完美支持:
int,long,signed,float,double,NSInteger,CGFloat,BOOL,NSString,NSMutableString,NSNumber,
NSArray,NSMutableArray,NSDictionary,NSMutableDictionary,NSMapTable,NSHashTable,NSData,
NSMutableData,UIImage,NSDate,NSURL,NSRange,CGRect,CGSize,CGPoint,自定义对象 等的存储.
 */
#import <Foundation/Foundation.h>
#import "YSCBGFMDBConfig.h"

@protocol YSCBGProtocol <NSObject>
//可选择操作
@optional
/**
 自定义 “联合主键” 函数, 如果需要自定义 “联合主键”,则在类中自己实现该函数.
 @return 返回值是 “联合主键” 的字段名(即相对应的变量名).
 注：当“联合主键”和“唯一约束”同时定义时，“联合主键”优先级大于“唯一约束”.
 */
+(NSArray* _Nonnull)YSCBG_unionPrimaryKeys;
/**
 自定义 “唯一约束” 函数,如果需要 “唯一约束”字段,则在类中自己实现该函数.
 @return 返回值是 “唯一约束” 的字段名(即相对应的变量名).
 */
+(NSArray* _Nonnull)YSCBG_uniqueKeys;
/**
 @return 返回不需要存储的属性.
 */
+(NSArray* _Nonnull)YSCBG_ignoreKeys;
/**
 *  数组中需要转换的模型类(‘字典转模型’ 或 ’模型转字典‘ 都需要实现该函数)
 *  @return 字典中的key是数组属性名，value是数组中存放模型的Class
 */
+(NSDictionary *_Nonnull)YSCBG_objectClassInArray;
/**
 如果模型中有自定义类变量,则实现该函数对应进行集合到模型的转换.
 字典转模型用.
 */
+(NSDictionary *_Nonnull)YSCBG_objectClassForCustom;
/**
 将模型中对应的自定义类变量转换为字典.
 模型转字典用.
 */
+(NSDictionary *_Nonnull)YSCBG_dictForCustomClass;
/**
替换变量的功能(及当字典的key和属性名不一样时，进行映射对应起来)
*/
+(NSDictionary *_Nonnull)YSCBG_replacedKeyFromPropertyName;
@end




@interface NSObject (YSCBGModel)<YSCBGProtocol>
/**
 本库自带的自动增长主键.
 */
@property(nonatomic,strong)NSNumber* _Nonnull YSCBG_id;
/**
 为了方便开发者，特此加入以下两个字段属性供开发者做参考.(自动记录数据的存入时间和更新时间)
 */
@property(nonatomic,copy)NSString* _Nonnull YSCBG_createTime;//数据创建时间(即存入数据库的时间)
@property(nonatomic,copy)NSString* _Nonnull YSCBG_updateTime;//数据最后那次更新的时间.
/**
 自定义表名
 */
@property(nonatomic,copy)NSString* _Nonnull YSCBG_tableName;

/**
 @tablename 此参数为nil时，判断以当前类名为表名的表是否存在; 此参数非nil时,判断以当前参数为表名的表是否存在.
 */
+(BOOL)YSCBG_isExistForTableName:(NSString* _Nullable)tablename;
/**
 同步存储.
 */
-(BOOL)YSCBG_save;
/**
 异步存储.
 */
-(void)YSCBG_saveAsync:(YSCBG_complete_B)complete;

/**
 同步存储或更新.
 当"唯一约束"或"主键"存在时，此接口会更新旧数据,没有则存储新数据.
 提示：“唯一约束”优先级高于"主键".
 */
-(BOOL)YSCBG_saveOrUpdate;
/**
 同上条件异步.
 */
-(void)YSCBG_saveOrUpdateAsync:(YSCBG_complete_B)complete;

/**
 同步 存储或更新 数组元素.
 @array 存放对象的数组.(数组中存放的是同一种类型的数据)
 当"唯一约束"或"主键"存在时，此接口会更新旧数据,没有则存储新数据.
 提示：“唯一约束”优先级高于"主键".
 */
+(BOOL)YSCBG_saveOrUpdateArray:(NSArray* _Nonnull)array;
/**
 同上条件异步.
 */
+(void)YSCBG_saveOrUpdateArrayAsync:(NSArray* _Nonnull)array complete:(YSCBG_complete_B)complete;

/**
 同步覆盖存储.
 覆盖掉原来的数据,只存储当前的数据.
 */
-(BOOL)YSCBG_cover;
/**
 同上条件异步.
 */
-(void)YSCBG_coverAsync:(YSCBG_complete_B)complete;

/**
 同步查询所有结果.
 @tablename 当此参数为nil时,查询以此类名为表名的数据，非nil时，查询以此参数为表名的数据.
 温馨提示: 当数据量巨大时,请用范围接口进行分页查询,避免查询出来的数据量过大导致程序崩溃.
 */
+(NSArray* _Nullable)YSCBG_findAll:(NSString* _Nullable)tablename;
/**
 同上条件异步.
 */
+(void)YSCBG_findAllAsync:(NSString* _Nullable)tablename complete:(YSCBG_complete_A)complete;

/**
 查找第一条数据
 @tablename 当此参数为nil时,查询以此类名为表名的数据，非nil时，查询以此参数为表名的数据.
 */
+(id _Nullable)YSCBG_firstObjet:(NSString* _Nullable)tablename;
/**
 查找最后一条数据
 @tablename 当此参数为nil时,查询以此类名为表名的数据，非nil时，查询以此参数为表名的数据.
 */
+(id _Nullable)YSCBG_lastObject:(NSString* _Nullable)tablename;
/**
 查询某一行数据
 @tablename 当此参数为nil时,查询以此类名为表名的数据，非nil时，查询以此参数为表名的数据.
 @row 从第1行开始算起.
 */
+(id _Nullable)YSCBG_object:(NSString* _Nullable)tablename row:(NSInteger)row;

/**
 同步查询所有结果.
 @tablename 当此参数为nil时,查询以此类名为表名的数据，非nil时，查询以此参数为表名的数据.
 @orderBy 要排序的key.
 @limit 每次查询限制的条数,0则无限制.
 @desc YES:降序，NO:升序.
 */
+(NSArray* _Nullable)YSCBG_find:(NSString* _Nullable)tablename limit:(NSInteger)limit orderBy:(NSString* _Nullable)orderBy desc:(BOOL)desc;
/**
 同上条件异步.
 */
+(void)YSCBG_findAsync:(NSString* _Nullable)tablename limit:(NSInteger)limit orderBy:(NSString* _Nullable)orderBy desc:(BOOL)desc complete:(YSCBG_complete_A)complete;
/**
 同步查询所有结果.
 @tablename 当此参数为nil时,查询以此类名为表名的数据，非nil时，查询以此参数为表名的数据.
 @orderBy 要排序的key.
 @range 查询的范围(从location开始的后面length条，localtion要大于0).
 @desc YES:降序，NO:升序.
 */
+(NSArray* _Nullable)YSCBG_find:(NSString* _Nullable)tablename range:(NSRange)range orderBy:(NSString* _Nullable)orderBy desc:(BOOL)desc;
/**
 同上条件异步.
 */
+(void)YSCBG_findAsync:(NSString* _Nullable)tablename range:(NSRange)range orderBy:(NSString* _Nullable)orderBy desc:(BOOL)desc complete:(YSCBG_complete_A)complete;
/**
 支持keyPath.
 @tablename 当此参数为nil时,查询以此类名为表名的数据，非nil时，查询以此参数为表名的数据.
 @where 条件参数，可以为nil,nil时查询所有数据.
 where使用规则请看demo或如下事例:
 1.查询name等于爸爸和age等于45,或者name等于马哥的数据.  此接口是为了方便开发者自由扩展更深层次的查询条件逻辑.
 where = [NSString stringWithFormat:@"where %@=%@ and %@=%@ or %@=%@",YSCBG_sqlKey(@"age"),YSCBG_sqlValue(@(45)),YSCBG_sqlKey(@"name"),YSCBG_sqlValue(@"爸爸"),YSCBG_sqlKey(@"name"),YSCBG_sqlValue(@"马哥")];
 2.查询user.student.human.body等于小芳 和 user1.name中包含fuck这个字符串的数据.
 where = [NSString stringWithFormat:@"where %@",YSCBG_keyPathValues(@[@"user.student.human.body",YSCBG_equal,@"小芳",@"user1.name",YSCBG_contains,@"fuck"])];
 3.查询user.student.human.body等于小芳,user1.name中包含fuck这个字符串 和 name等于爸爸的数据.
 where = [NSString stringWithFormat:@"where %@ and %@=%@",YSCBG_keyPathValues(@[@"user.student.human.body",YSCBG_equal,@"小芳",@"user1.name",YSCBG_contains,@"fuck"]),YSCBG_sqlKey(@"name"),YSCBG_sqlValue(@"爸爸")];
 */
+(NSArray* _Nullable)YSCBG_find:(NSString* _Nullable)tablename where:(NSString* _Nullable)where;
/**
 同上条件异步.
 */
+(void)YSCBG_findAsync:(NSString* _Nullable)tablename where:(NSString* _Nullable)where complete:(YSCBG_complete_A)complete;

/**
 查询某一时间段的数据.(存入时间或更新时间)
 @tablename 当此参数为nil时,查询以此类名为表名的数据，非nil时，查询以此参数为表名的数据.
 @dateTime 参数格式：
 2017 即查询2017年的数据
 2017-07 即查询2017年7月的数据
 2017-07-19 即查询2017年7月19日的数据
 2017-07-19 16 即查询2017年7月19日16时的数据
 2017-07-19 16:17 即查询2017年7月19日16时17分的数据
 2017-07-19 16:17:53 即查询2017年7月19日16时17分53秒的数据
 */
+(NSArray* _Nullable)YSCBG_find:(NSString* _Nullable)tablename type:(YSCBG_dataTimeType)type dateTime:(NSString* _Nonnull)dateTime;

/**
 支持keyPath.
 @where 条件参数,不能为nil.
 where使用规则请看demo或如下事例:
 1.将People类数据中user.student.human.body等于"小芳"的数据更新为当前对象的数据:
 where = [NSString stringWithFormat:@"where %@",YSCBG_keyPathValues(@[@"user.student.human.body",YSCBG_equal,@"小芳"])];
 2.将People类中name等于"马云爸爸"的数据更新为当前对象的数据:
 where = [NSString stringWithFormat:@"where %@=%@",YSCBG_sqlKey(@"name"),YSCBG_sqlValue(@"马云爸爸")];
 */
-(BOOL)YSCBG_updateWhere:(NSString* _Nonnull)where;
/**
 同上条件异步.
 */
-(void)YSCBG_updateAsyncWhere:(NSString* _Nonnull)where complete:(YSCBG_complete_B)complete;
/**
 此接口不支持keyPath.
 @tablename 当此参数为nil时,查询以此类名为表名的数据，非nil时，更新以此参数为表名的数据.
 @where 条件参数,不能为nil.
 where使用规则请看demo或如下事例:
 1.将People类中name等于"马云爸爸"的数据的name更新为"马化腾":
 where = [NSString stringWithFormat:@"set %@=%@ where %@=%@",YSCBG_sqlKey(@"name"),YSCBG_sqlValue(@"马化腾"),YSCBG_sqlKey(@"name"),YSCBG_sqlValue(@"马云爸爸")];
 */
+(BOOL)YSCBG_update:(NSString* _Nullable)tablename where:(NSString* _Nonnull)where;


/**
 支持keyPath.
 @tablename 当此参数为nil时,查询以此类名为表名的数据，非nil时，删除以此参数为表名的数据.
 @where 条件参数,可以为nil，nil时删除所有以tablename为表名的数据.
 where使用规则请看demo或如下事例:
 1.删除People类中name等于"美国队长"的数据.
 where = [NSString stringWithFormat:@"where %@=%@",YSCBG_sqlKey(@"name"),YSCBG_sqlValue(@"美国队长")];
 2.删除People类中user.student.human.body等于"小芳"的数据.
 where = [NSString stringWithFormat:@"where %@",YSCBG_keyPathValues(@[@"user.student.human.body",YSCBG_equal,@"小芳"])];
 3.删除People类中name等于"美国队长" 和 user.student.human.body等于"小芳"的数据.
 where = [NSString stringWithFormat:@"where %@=%@ and %@",YSCBG_sqlKey(@"name"),YSCBG_sqlValue(@"美国队长"),YSCBG_keyPathValues(@[@"user.student.human.body",YSCBG_equal,@"小芳"])];
 */
+(BOOL)YSCBG_delete:(NSString* _Nullable)tablename where:(NSString* _Nullable)where;
/**
 同上条件异步.
 */
+(void)YSCBG_deleteAsync:(NSString* _Nullable)tablename where:(NSString* _Nullable)where complete:(YSCBG_complete_B)complete;


/**
 删除某一行数据
 @tablename 当此参数为nil时,查询以此类名为表名的数据，非nil时，删除以此参数为表名的数据.
 @row 第几行，从第1行算起.
 */
+(BOOL)YSCBG_delete:(NSString* _Nullable)tablename row:(NSInteger)row;
/**
 删除第一条数据
 @tablename 当此参数为nil时,查询以此类名为表名的数据，非nil时，删除以此参数为表名的数据.
 */
+(BOOL)YSCBG_deleteFirstObject:(NSString* _Nullable)tablename;
/**
 删除最后一条数据
 @tablename 当此参数为nil时,查询以此类名为表名的数据，非nil时，删除以此参数为表名的数据.
 */
+(BOOL)YSCBG_deleteLastObject:(NSString* _Nullable)tablename;


/**
 同步清除所有数据.
 @tablename 当此参数为nil时,查询以此类名为表名的数据，非nil时，清除以此参数为表名的数据.
 */
+(BOOL)YSCBG_clear:(NSString* _Nullable)tablename;
/**
 异步清除所有数据.
 @tablename 当此参数为nil时,查询以此类名为表名的数据，非nil时，清除以此参数为表名的数据.
 */
+(void)YSCBG_clearAsync:(NSString* _Nullable)tablename complete:(YSCBG_complete_B)complete;


/**
 同步删除这个类的数据表.
 @tablename 当此参数为nil时,查询以此类名为表名的数据，非nil时，清除以此参数为表名的数据.
 */
+(BOOL)YSCBG_drop:(NSString* _Nullable)tablename;
/**
 异步删除这个类的数据表.
 @tablename 当此参数为nil时,查询以此类名为表名的数据，非nil时，清除以此参数为表名的数据.
 */
+(void)YSCBG_dropAsync:(NSString* _Nullable)tablename complete:(YSCBG_complete_B)complete;


/**
 查询该表中有多少条数据.
 @tablename 当此参数为nil时,查询以此类名为表名的数据条数，非nil时，查询以此参数为表名的数据条数.
 @where 条件参数,nil时查询所有以tablename为表名的数据条数.
 支持keyPath.
 使用规则请看demo或如下事例:
 1.查询People类中name等于"美国队长"的数据条数.
 where = [NSString stringWithFormat:@"where %@=%@",YSCBG_sqlKey(@"name"),YSCBG_sqlValue(@"美国队长")];
 2.查询People类中user.student.human.body等于"小芳"的数据条数.
 where = [NSString stringWithFormat:@"where %@",YSCBG_keyPathValues(@[@"user.student.human.body",YSCBG_equal,@"小芳"])];
 3.查询People类中name等于"美国队长" 和 user.student.human.body等于"小芳"的数据条数.
 where = [NSString stringWithFormat:@"where %@=%@ and %@",YSCBG_sqlKey(@"name"),YSCBG_sqlValue(@"美国队长"),YSCBG_keyPathValues(@[@"user.student.human.body",YSCBG_equal,@"小芳"])];
 */
+(NSInteger)YSCBG_count:(NSString* _Nullable)tablename where:(NSString* _Nullable)where;


/**
 直接调用sqliteb的原生函数计算sun,min,max,avg等.
 @tablename 当此参数为nil时,操作以此类名为表名的数据表，非nil时，操作以此参数为表名的数据表.
 @key -> 要操作的属性,不支持keyPath.
 @where -> 条件参数,支持keyPath.
 */
+(double)YSCBG_sqliteMethodWithTableName:(NSString* _Nullable)tablename type:(YSCBG_sqliteMethodType)methodType key:(NSString* _Nonnull)key where:(NSString* _Nullable)where;
/**
 获取数据表当前版本号.
 @tablename 当此参数为nil时,操作以此类名为表名的数据表，非nil时，操作以此参数为表名的数据表.
 */
+(NSInteger)YSCBG_version:(NSString* _Nullable)tablename;
/**
 刷新,当类"唯一约束"改变时,调用此接口刷新一下.
 同步刷新.
 @tablename 当此参数为nil时,操作以此类名为表名的数据表，非nil时，操作以此参数为表名的数据表.
 @version 版本号,从1开始,依次往后递增.
 说明: 本次更新版本号不得 低于或等于 上次的版本号,否则不会更新.
 */
+(YSCBG_dealState)YSCBG_update:(NSString* _Nullable)tablename version:(NSInteger)version;
/**
 同上条件异步.
 */
+(void)YSCBG_updateAsync:(NSString* _Nullable)tablename version:(NSInteger)version complete:(YSCBG_complete_I)complete;
/**
 刷新,当类"唯一约束"改变时,调用此接口刷新一下.
 同步刷新.
 @tablename 当此参数为nil时,操作以此类名为表名的数据表，非nil时，操作以此参数为表名的数据表.
 @version 版本号,从1开始,依次往后递增.
 @keyDict 拷贝的对应key集合,形式@{@"新Key1":@"旧Key1",@"新Key2":@"旧Key2"},即将本类以前的变量 “旧Key1” 的数据拷贝给现在本类的变量“新Key1”，其他依此推类.
 (特别提示: 这里只要写那些改变了的变量名就可以了,没有改变的不要写)，比如A以前有3个变量,分别为a,b,c；现在变成了a,b,d；那只要写@{@"d":@"c"}就可以了，即只写变化了的变量名映射集合.
 说明: 本次更新版本号不得 低于或等于 上次的版本号,否则不会更新.
 */
+(YSCBG_dealState)YSCBG_update:(NSString* _Nullable)tablename version:(NSInteger)version keyDict:(NSDictionary* const _Nonnull)keydict;
/**
 同上条件异步.
 */
+(void)YSCBG_updateAsync:(NSString* _Nullable)tablename version:(NSInteger)version keyDict:(NSDictionary* const _Nonnull)keydict complete:(YSCBG_complete_I)complete;
/**
 将某表的数据拷贝给另一个表.
 同步复制.
 @tablename 源表名,当此参数为nil时,操作以此类名为表名的数据表，非nil时，操作以此参数为表名的数据表.
 @destCla 目标表名.
 @keyDict 拷贝的对应key集合,形式@{@"srcKey1":@"destKey1",@"srcKey2":@"destKey2"},即将源类srcCla中的变量值拷贝给目标类destCla中的变量destKey1，srcKey2和destKey2同理对应,依此推类.
 @append YES: 不会覆盖destCla的原数据,在其末尾继续添加；NO: 覆盖掉destCla原数据,即将原数据删掉,然后将新数据拷贝过来.
 */
+(YSCBG_dealState)YSCBG_copy:(NSString* _Nullable)tablename toTable:(NSString* _Nonnull)destTable keyDict:(NSDictionary* const _Nonnull)keydict append:(BOOL)append;
/**
 同上条件异步.
 */
+(void)YSCBG_copyAsync:(NSString* _Nullable)tablename toTable:(NSString* _Nonnull)destTable keyDict:(NSDictionary* const _Nonnull)keydict append:(BOOL)append complete:(YSCBG_complete_I)complete;

/**
 注册数据库表变化监听.
 @tablename 表名称，当此参数为nil时，监听以当前类名为表名的数据表，当此参数非nil时，监听以此参数为表名的数据表。
 @identify 唯一标识，,此字符串唯一,不可重复,移除监听的时候使用此字符串移除.
 @return YES: 注册监听成功; NO: 注册监听失败.
 */
+(BOOL)YSCBG_registerChangeForTableName:(NSString* _Nullable)tablename identify:(NSString* _Nonnull)identify block:(YSCBG_changeBlock)block;
/**
 移除数据库表变化监听.
 @tablename 表名称，当此参数为nil时，监听以当前类名为表名的数据表，当此参数非nil时，监听以此参数为表名的数据表。
 @identify 唯一标识，,此字符串唯一,不可重复,移除监听的时候使用此字符串移除.
 @return YES: 移除监听成功; NO: 移除监听失败.
 */
+(BOOL)YSCBG_removeChangeForTableName:(NSString* _Nullable)tablename identify:(NSString* _Nonnull)identify;

#pragma mark 下面附加字典转模型API,简单好用,在只需要字典转模型功能的情况下,可以不必要再引入MJExtension那么多文件,造成代码冗余,缩减安装包.
/**
 字典转模型.
 @keyValues 字典(NSDictionary)或json格式字符.
 说明:如果模型中有数组且存放的是自定义的类(NSString等系统自带的类型就不必要了),那就实现objectClassInArray这个函数返回一个字典,key是数组名称,value是自定的类Class,用法跟MJExtension一样.
 */
+(id _Nonnull)YSCBG_objectWithKeyValues:(id const _Nonnull)keyValues;
+(id _Nonnull)YSCBG_objectWithDictionary:(NSDictionary* const _Nonnull)dictionary;
/**
 直接传数组批量处理;
 注:array中的元素是字典,否则出错.
 */
+(NSArray* _Nonnull)YSCBG_objectArrayWithKeyValuesArray:(NSArray* const _Nonnull)array;
/**
 模型转字典.
 @ignoredKeys 忽略掉模型中的哪些key(即模型变量)不要转,nil时全部转成字典.
 */
-(NSMutableDictionary* _Nonnull)YSCBG_keyValuesIgnoredKeys:(NSArray* _Nullable)ignoredKeys;

#warning mark 过期方法(能正常使用,但不建议使用)
/**
 判断这个类的数据表是否已经存在.
 */
+(BOOL)YSCBG_isExist YSCBGFMDBDeprecated("此方法已过期(能正常使用,但不建议使用)，使用YSCBG_isExistForTableName:替代.");
/**
 同步存入对象数组.
 @array 存放对象的数组.(数组中存放的是同一种类型的数据)
 */
+(BOOL)YSCBG_saveArray:(NSArray* _Nonnull)array YSCBGFMDBDeprecated("此方法已过期(能正常使用,但不建议使用)，请使用YSCBG_saveOrUpdateArray替代");
/**
 同上条件异步.
 */
+(void)YSCBG_saveArrayAsync:(NSArray* _Nonnull)array complete:(YSCBG_complete_B)complete YSCBGFMDBDeprecated("此方法已过期(能正常使用,但不建议使用)，请使用YSCBG_saveOrUpdateArray替代");

/**
 同步更新对象数组.
 @array 存放对象的数组.(数组中存放的是同一种类型的数据).
 当类中定义了"唯一约束" 或 "主键"有值时,使用此API才有意义.
 提示：“唯一约束”优先级高于"主键".
 */
+(BOOL)YSCBG_updateArray:(NSArray* _Nonnull)array YSCBGFMDBDeprecated("此方法已过期(能正常使用,但不建议使用)，请使用YSCBG_saveOrUpdateArray替代");
/**
 同上条件异步.
 */
+(void)YSCBG_updateArrayAsync:(NSArray* _Nonnull)array complete:(YSCBG_complete_B)complete YSCBGFMDBDeprecated("此方法已过期(能正常使用,但不建议使用)，请使用YSCBG_saveOrUpdateArray替代");
/**
 同步存入对象数组.
 @array 存放对象的数组.(数组中存放的是同一种类型的数据)
 */
+(BOOL)YSCBG_saveArray:(NSArray* _Nonnull)array IgnoreKeys:(NSArray* const _Nullable)ignoreKeys YSCBGFMDBDeprecated("此方法已过期(能正常使用,但不建议使用)，在模型的.m文件中实现YSCBG_ignoreKeys函数即可");
/**
 异步存入对象数组.
 @array 存放对象的数组.(数组中存放的是同一种类型的数据)
 */
+(void)YSCBG_saveArrayAsync:(NSArray* _Nonnull)array IgnoreKeys:(NSArray* const _Nullable)ignoreKeys complete:(YSCBG_complete_B)complete YSCBGFMDBDeprecated("此方法已过期(能正常使用,但不建议使用)，在模型的.m文件中实现YSCBG_ignoreKeys函数即可");
/**
 同步存储.
 @ignoreKeys 忽略掉模型中的哪些key(即模型变量)不要存储.
 */
-(BOOL)YSCBG_saveIgnoredKeys:(NSArray* const _Nonnull)ignoredKeys YSCBGFMDBDeprecated("此方法已过期(能正常使用,但不建议使用)，在模型的.m文件中实现YSCBG_ignoreKeys函数即可");
/**
 异步存储.
 @ignoreKeys 忽略掉模型中的哪些key(即模型变量)不要存储.
 */
-(void)YSCBG_saveAsyncIgnoreKeys:(NSArray* const _Nonnull)ignoredKeys complete:(YSCBG_complete_B)complete YSCBGFMDBDeprecated("此方法已过期(能正常使用,但不建议使用)，在模型的.m文件中实现YSCBG_ignoreKeys函数即可");
/**
 同步覆盖存储.
 覆盖掉原来的数据,只存储当前的数据.
 @ignoreKeys 忽略掉模型中的哪些key(即模型变量)不要存储.
 */
-(BOOL)YSCBG_coverIgnoredKeys:(NSArray* const _Nonnull)ignoredKeys YSCBGFMDBDeprecated("此方法已过期(能正常使用,但不建议使用)，在模型的.m文件中实现YSCBG_ignoreKeys函数即可");
/**
 异步覆盖存储.
 覆盖掉原来的数据,只存储当前的数据.
 @ignoreKeys 忽略掉模型中的哪些key(即模型变量)不要存储.
 */
-(void)YSCBG_coverAsyncIgnoredKeys:(NSArray* const _Nonnull)ignoredKeys complete:(YSCBG_complete_B)complete YSCBGFMDBDeprecated("此方法已过期(能正常使用,但不建议使用)，在模型的.m文件中实现YSCBG_ignoreKeys函数即可");
/**
 同步更新数据.
 @where 条件数组，形式@[@"name",@"=",@"标哥",@"age",@"=>",@(25)],即更新name=标哥,age=>25的数据.
 可以为nil,nil时更新所有数据;
 @ignoreKeys 忽略哪些key不用更新.
 不支持keypath的key,即嵌套的自定义类, 形式如@[@"user.name",@"=",@"习大大"]暂不支持(有专门的keyPath更新接口).
 */
-(BOOL)YSCBG_updateWhere:(NSArray* _Nullable)where ignoreKeys:(NSArray* const _Nullable)ignoreKeys YSCBGFMDBDeprecated("此方法已过期(能正常使用,但不建议使用)，在模型的.m文件中实现YSCBG_ignoreKeys函数即可");
/**
 @format 传入sql条件参数,语句来进行更新,方便开发者自由扩展.
 支持keyPath.
 使用规则请看demo或如下事例:
 1.将People类数据中user.student.human.body等于"小芳"的数据更新为当前对象的数据(忽略name不要更新).
 NSString* conditions = [NSString stringWithFormat:@"where %@",YSCBG_keyPathValues(@[@"user.student.human.body",YSCBG_equal,@"小芳"])];
 [p YSCBG_updateFormatSqlConditions:conditions IgnoreKeys:@[@"name"]];
 2.将People类中name等于"马云爸爸"的数据更新为当前对象的数据.
 NSString* conditions = [NSString stringWithFormat:@"where %@=%@",YSCBG_sqlKey(@"name"),YSCBG_sqlValue(@"马云爸爸")])];
 [p YSCBG_updateFormatSqlConditions:conditions IgnoreKeys:nil];
 @ignoreKeys 忽略哪些key不用更新.
 */
-(BOOL)YSCBG_updateFormatSqlConditions:(NSString* _Nonnull)conditions IgnoreKeys:(NSArray* const _Nullable)ignoreKeys YSCBGFMDBDeprecated("此方法已过期(能正常使用,但不建议使用)，在模型的.m文件中实现YSCBG_ignoreKeys函数即可");
@end

#pragma mark 直接存储数组.
@interface NSArray (YSCBGModel)
/**
 存储数组.
 @name 唯一标识名称.
 **/
-(BOOL)YSCBG_saveArrayWithName:(NSString* const _Nonnull)name;
/**
 添加数组元素.
 @name 唯一标识名称.
 @object 要添加的元素.
 */
+(BOOL)YSCBG_addObjectWithName:(NSString* const _Nonnull)name object:(id const _Nonnull)object;
/**
 获取数组元素数量.
 @name 唯一标识名称.
 */
+(NSInteger)YSCBG_countWithName:(NSString* const _Nonnull)name;
/**
 查询整个数组
 */
+(NSArray* _Nullable)YSCBG_arrayWithName:(NSString* const _Nonnull)name;
/**
 获取数组某个位置的元素.
 @name 唯一标识名称.
 @index 数组元素位置.
 */
+(id _Nullable)YSCBG_objectWithName:(NSString* const _Nonnull)name Index:(NSInteger)index;
/**
 更新数组某个位置的元素.
 @name 唯一标识名称.
 @index 数组元素位置.
 */
+(BOOL)YSCBG_updateObjectWithName:(NSString* const _Nonnull)name Object:(id _Nonnull)object Index:(NSInteger)index;
/**
 删除数组的某个元素.
 @name 唯一标识名称.
 @index 数组元素位置.
 */
+(BOOL)YSCBG_deleteObjectWithName:(NSString* const _Nonnull)name Index:(NSInteger)index;
/**
 清空数组元素.
 @name 唯一标识名称.
 */
+(BOOL)YSCBG_clearArrayWithName:(NSString* const _Nonnull)name;
@end

#pragma mark 直接存储字典.
@interface NSDictionary (YSCBGModel)
/**
 存储字典.
 */
-(BOOL)YSCBG_saveDictionary;
/**
 添加字典元素.
 */
+(BOOL)YSCBG_setValue:(id const _Nonnull)value forKey:(NSString* const _Nonnull)key;
/**
 更新字典元素.
 */
+(BOOL)YSCBG_updateValue:(id const _Nonnull)value forKey:(NSString* const _Nonnull)key;
/**
 获取字典元素.
 */
+(id _Nullable)YSCBG_valueForKey:(NSString* const _Nonnull)key;
/**
 遍历字典元素.
 */
+(void)YSCBG_enumerateKeysAndObjectsUsingBlock:(void (^ _Nonnull)(NSString* _Nonnull key, id _Nonnull value,BOOL * _Nullable stop))block;
/**
 移除字典某个元素.
 */
+(BOOL)YSCBG_removeValueForKey:(NSString* const _Nonnull)key;
/**
 清空字典.
 */
+(BOOL)YSCBG_clearDictionary;
@end
