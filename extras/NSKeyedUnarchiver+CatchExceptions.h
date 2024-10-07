#import <Foundation/Foundation.h>

/*!
  @brief    Improvements to NSKeyedUnarchiver

  @details  Method +unarchiveObjectWithData: has been replaced so that
  instead of raising an exception and crashing if given a corrupt
archive,
  it just returns nil.  Also, another method has been added which
  returns the exception.
*/
@interface NSKeyedUnarchiver (CatchExceptions)

/*!
  @brief    Like unarchiveObjectWithData:, except it returns the
  exception by reference.

  @param    exception_p  Pointer which will, upon return, if an
  exception occurred and said pointer is not NULL, point to said
  NSException.
  */
+ (id)unarchiveObjectWithData:(NSData*)data
                  exception_p:(NSException**)exception_p ;

@end