//
//  YSCDatabaseQueue.m
//  YSCdb
//
//  Created by August Mueller on 6/22/11.
//  Copyright 2011 Flying Meat Inc. All rights reserved.
//

#import "YSCDatabaseQueue.h"
#import "YSCDatabase.h"

#if YSCDB_SQLITE_STANDALONE
#import <sqlite3/sqlite3.h>
#else
#import <sqlite3.h>
#endif

typedef NS_ENUM(NSInteger, YSCDBTransaction) {
    YSCDBTransactionExclusive,
    YSCDBTransactionDeferred,
    YSCDBTransactionImmediate,
};

/*
 
 Note: we call [self retain]; before using dispatch_sync, just incase 
 YSCDatabaseQueue is released on another thread and we're in the middle of doing
 something in dispatch_sync
 
 */

/*
 * A key used to associate the YSCDatabaseQueue object with the dispatch_queue_t it uses.
 * This in turn is used for deadlock detection by seeing if inDatabase: is called on
 * the queue's dispatch queue, which should not happen and causes a deadlock.
 */
static const void * const kDispatchQueueSpecificKey = &kDispatchQueueSpecificKey;

@interface YSCDatabaseQueue () {
    dispatch_queue_t    _queue;
    YSCDatabase          *_db;
}
@end

@implementation YSCDatabaseQueue

+ (instancetype)databaseQueueWithPath:(NSString *)aPath {
    YSCDatabaseQueue *q = [[self alloc] initWithPath:aPath];
    
    YSCDBAutorelease(q);
    
    return q;
}

+ (instancetype)databaseQueueWithURL:(NSURL *)url {
    return [self databaseQueueWithPath:url.path];
}

+ (instancetype)databaseQueueWithPath:(NSString *)aPath flags:(int)openFlags {
    YSCDatabaseQueue *q = [[self alloc] initWithPath:aPath flags:openFlags];
    
    YSCDBAutorelease(q);
    
    return q;
}

+ (instancetype)databaseQueueWithURL:(NSURL *)url flags:(int)openFlags {
    return [self databaseQueueWithPath:url.path flags:openFlags];
}

+ (Class)databaseClass {
    return [YSCDatabase class];
}

- (instancetype)initWithURL:(NSURL *)url flags:(int)openFlags vfs:(NSString *)vfsName {
    return [self initWithPath:url.path flags:openFlags vfs:vfsName];
}

- (instancetype)initWithPath:(NSString*)aPath flags:(int)openFlags vfs:(NSString *)vfsName {
    self = [super init];
    
    if (self != nil) {
        
        _db = [[[self class] databaseClass] databaseWithPath:aPath];
        YSCDBRetain(_db);
        
#if SQLITE_VERSION_NUMBER >= 3005000
        BOOL success = [_db openWithFlags:openFlags vfs:vfsName];
#else
        BOOL success = [_db open];
#endif
        if (!success) {
            NSLog(@"Could not create database queue for path %@", aPath);
            YSCDBRelease(self);
            return 0x00;
        }
        
        _path = YSCDBReturnRetained(aPath);
        
        _queue = dispatch_queue_create([[NSString stringWithFormat:@"YSCdb.%@", self] UTF8String], NULL);
        dispatch_queue_set_specific(_queue, kDispatchQueueSpecificKey, (__bridge void *)self, NULL);
        _openFlags = openFlags;
        _vfsName = [vfsName copy];
    }
    
    return self;
}

- (instancetype)initWithPath:(NSString *)aPath flags:(int)openFlags {
    return [self initWithPath:aPath flags:openFlags vfs:nil];
}

- (instancetype)initWithURL:(NSURL *)url flags:(int)openFlags {
    return [self initWithPath:url.path flags:openFlags vfs:nil];
}

- (instancetype)initWithURL:(NSURL *)url {
    return [self initWithPath:url.path];
}

- (instancetype)initWithPath:(NSString *)aPath {
    // default flags for sqlite3_open
    return [self initWithPath:aPath flags:SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE vfs:nil];
}

- (instancetype)init {
    return [self initWithPath:nil];
}

- (void)dealloc {
    YSCDBRelease(_db);
    YSCDBRelease(_path);
    YSCDBRelease(_vfsName);
    
    if (_queue) {
        YSCDBDispatchQueueRelease(_queue);
        _queue = 0x00;
    }
#if ! __has_feature(objc_arc)
    [super dealloc];
#endif
}

- (void)close {
    YSCDBRetain(self);
    dispatch_sync(_queue, ^() {
        [self->_db close];
        YSCDBRelease(_db);
        self->_db = 0x00;
    });
    YSCDBRelease(self);
}

- (void)interrupt {
    [[self database] interrupt];
}

- (YSCDatabase*)database {
    if (![_db isOpen]) {
        if (!_db) {
           _db = YSCDBReturnRetained([[[self class] databaseClass] databaseWithPath:_path]);
        }
        
#if SQLITE_VERSION_NUMBER >= 3005000
        BOOL success = [_db openWithFlags:_openFlags vfs:_vfsName];
#else
        BOOL success = [_db open];
#endif
        if (!success) {
            NSLog(@"YSCDatabaseQueue could not reopen database for path %@", _path);
            YSCDBRelease(_db);
            _db  = 0x00;
            return 0x00;
        }
    }
    
    return _db;
}

- (void)inDatabase:(void (^)(YSCDatabase *db))block {
#ifndef NDEBUG
    /* Get the currently executing queue (which should probably be nil, but in theory could be another DB queue
     * and then check it against self to make sure we're not about to deadlock. */
    YSCDatabaseQueue *currentSyncQueue = (__bridge id)dispatch_get_specific(kDispatchQueueSpecificKey);
    assert(currentSyncQueue != self && "inDatabase: was called reentrantly on the same queue, which would lead to a deadlock");
#endif
    
    YSCDBRetain(self);
    
    dispatch_sync(_queue, ^() {
        
        YSCDatabase *db = [self database];
        
        block(db);
        
        if ([db hasOpenResultSets]) {
            NSLog(@"Warning: there is at least one open result set around after performing [YSCDatabaseQueue inDatabase:]");
            
#if defined(DEBUG) && DEBUG
            NSSet *openSetCopy = YSCDBReturnAutoreleased([[db valueForKey:@"_openResultSets"] copy]);
            for (NSValue *rsInWrappedInATastyValueMeal in openSetCopy) {
                YSCResultSet *rs = (YSCResultSet *)[rsInWrappedInATastyValueMeal pointerValue];
                NSLog(@"query: '%@'", [rs query]);
            }
#endif
        }
    });
    
    YSCDBRelease(self);
}

- (void)beginTransaction:(YSCDBTransaction)transaction withBlock:(void (^)(YSCDatabase *db, BOOL *rollback))block {
    YSCDBRetain(self);
    dispatch_sync(_queue, ^() { 
        
        BOOL shouldRollback = NO;

        switch (transaction) {
            case YSCDBTransactionExclusive:
                [[self database] beginTransaction];
                break;
            case YSCDBTransactionDeferred:
                [[self database] beginDeferredTransaction];
                break;
            case YSCDBTransactionImmediate:
                [[self database] beginImmediateTransaction];
                break;
        }
        
        block([self database], &shouldRollback);
        
        if (shouldRollback) {
            [[self database] rollback];
        }
        else {
            [[self database] commit];
        }
    });
    
    YSCDBRelease(self);
}

- (void)inTransaction:(void (^)(YSCDatabase *db, BOOL *rollback))block {
    [self beginTransaction:YSCDBTransactionExclusive withBlock:block];
}

- (void)inDeferredTransaction:(void (^)(YSCDatabase *db, BOOL *rollback))block {
    [self beginTransaction:YSCDBTransactionDeferred withBlock:block];
}

- (void)inExclusiveTransaction:(void (^)(YSCDatabase *db, BOOL *rollback))block {
    [self beginTransaction:YSCDBTransactionExclusive withBlock:block];
}

- (void)inImmediateTransaction:(void (^)(YSCDatabase * _Nonnull, BOOL * _Nonnull))block {
    [self beginTransaction:YSCDBTransactionImmediate withBlock:block];
}

- (NSError*)inSavePoint:(void (^)(YSCDatabase *db, BOOL *rollback))block {
#if SQLITE_VERSION_NUMBER >= 3007000
    static unsigned long savePointIdx = 0;
    __block NSError *err = 0x00;
    YSCDBRetain(self);
    dispatch_sync(_queue, ^() { 
        
        NSString *name = [NSString stringWithFormat:@"savePoint%ld", savePointIdx++];
        
        BOOL shouldRollback = NO;
        
        if ([[self database] startSavePointWithName:name error:&err]) {
            
            block([self database], &shouldRollback);
            
            if (shouldRollback) {
                // We need to rollback and release this savepoint to remove it
                [[self database] rollbackToSavePointWithName:name error:&err];
            }
            [[self database] releaseSavePointWithName:name error:&err];
            
        }
    });
    YSCDBRelease(self);
    return err;
#else
    NSString *errorMessage = NSLocalizedString(@"Save point functions require SQLite 3.7", nil);
    if (self.logsErrors) NSLog(@"%@", errorMessage);
    return [NSError errorWithDomain:@"YSCDatabase" code:0 userInfo:@{NSLocalizedDescriptionKey : errorMessage}];
#endif
}

- (BOOL)checkpoint:(YSCDBCheckpointMode)mode error:(NSError * __autoreleasing *)error
{
    return [self checkpoint:mode name:nil logFrameCount:NULL checkpointCount:NULL error:error];
}

- (BOOL)checkpoint:(YSCDBCheckpointMode)mode name:(NSString *)name error:(NSError * __autoreleasing *)error
{
    return [self checkpoint:mode name:name logFrameCount:NULL checkpointCount:NULL error:error];
}

- (BOOL)checkpoint:(YSCDBCheckpointMode)mode name:(NSString *)name logFrameCount:(int * _Nullable)logFrameCount checkpointCount:(int * _Nullable)checkpointCount error:(NSError * __autoreleasing _Nullable * _Nullable)error
{
    __block BOOL result;
    __block NSError *blockError;
    
    YSCDBRetain(self);
    dispatch_sync(_queue, ^() {
        result = [self.database checkpoint:mode name:name logFrameCount:NULL checkpointCount:NULL error:&blockError];
    });
    YSCDBRelease(self);
    
    if (error) {
        *error = blockError;
    }
    return result;
}

@end
