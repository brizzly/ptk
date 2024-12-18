
#import "NSKeyedUnarchiver+CatchExceptions.h"
#import <objc/runtime.h>

@implementation NSKeyedUnarchiver (CatchExceptions)

+ (id)unarchiveObjectWithData:(NSData*)data
                  exception_p:(NSException**)exception_p {
    id object = nil ;

    @try {
        // Note: Since methods were swapped, this is invoking the original method
        object = [NSKeyedUnarchiver
replacement_unarchiveObjectWithData:data] ;
    }
    @catch (NSException* exception) {
        if (exception_p) {
            *exception_p = exception ;
        }
    }
    @finally{
    }

    return object ;
}

+ (void)load {
	// Swap the implementations of +unarchiveObjectWithData: and +replacement_unarchiveObjectWithData:.
	// When the +unarchiveObjectWithData: message is sent to the NSKeyedUnarchiver class object,
	// +replacement_unarchiveObjectWithData: will be invoked instead.  Conversely,
	// +replacement_unarchiveObjectWithData: will invoke +unarchiveObjectWithData:.
	Method originalMethod = class_getClassMethod(self,
												 @selector(unarchiveObjectWithData:)) ;
	Method replacedMethod = class_getClassMethod(self,
												 @selector(replacement_unarchiveObjectWithData:)) ;
	method_exchangeImplementations(originalMethod, replacedMethod) ;
}

+ (id)replacement_unarchiveObjectWithData:(NSData*)data {
    return [self unarchiveObjectWithData:data
                              exception_p:NULL] ;
}

@end