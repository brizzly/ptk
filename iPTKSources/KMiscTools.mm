#include "KMiscTools.h"
#include "Foundation/Foundation.h"
#include <assert.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>
#include <UIKit/UIKit.h>

// isInternetReachable
#import <SystemConfiguration/SystemConfiguration.h>
#import <netinet/in.h>
#import <netinet6/in6.h>

#import <Foundation/Foundation.h>
#import <Security/Security.h>


struct utsname KMiscTools::systemInfo;

char	KMiscTools::currentAppPath[MAX_PATH];
char	KMiscTools::currentFilePath[MAX_PATH];
char	KMiscTools::currentTempPath[MAX_PATH];
char	KMiscTools::currentDocHomePath[MAX_PATH];
bool	KMiscTools::_inPackage;

char	KMiscTools::currentPath[MAX_PATH*2];

uint64_t 	t0;		// Start time for getMillisecond()
static mach_timebase_info_data_t timebase;


//#################################################################################	
static inline unsigned int getCString(NSString * str, char * cstr, unsigned int bufSize)
{
    const char * utf8 = [str UTF8String];
	unsigned int size = [str length];
	if ( size > bufSize )
		size = bufSize-1;
    memcpy(cstr, utf8, size);
	return size;
}

//#################################################################################	
unsigned long KMiscTools::getMilliseconds( void )
{
	return (unsigned long)(((float)(mach_absolute_time() - t0)) * ((float)timebase.numer) / ((float)timebase.denom) / 1000000.0f);
}

//#################################################################################	
long KMiscTools::getSeed( void )
{
	return (long)mach_absolute_time();
}

//#################################################################################	
void KMiscTools::getSerial( char *s )
{
	memset(s, 0, 45);
//	NSString *uniqueIdentifier;
	/*
	UIDevice *myCurrentDevice = [UIDevice currentDevice];
	uniqueIdentifier = [myCurrentDevice uniqueIdentifier];
	getCString(uniqueIdentifier, s, 40);
	//[myCurrentDevice release];
	//[uniqueIdentifier release];
	 */
}


//#################################################################################	
void KMiscTools::initMiscTools( void ) 
{
	//if(logfile) { logfile->logDebug("initMiscTools"); }

	_inPackage = true;
	
	t0 = mach_absolute_time();			// Init hires timer
	mach_timebase_info(&timebase);	// Get mach timbase info to get real time

	memset(currentAppPath, 0, MAX_PATH);
	getAppHome(currentAppPath, MAX_PATH);

	memset(currentFilePath, 0, MAX_PATH);
	getUserHome(currentFilePath, MAX_PATH);
	
	memset(currentTempPath, 0, MAX_PATH);
	getTempHome(currentTempPath, MAX_PATH);
	
	memset(currentDocHomePath, 0, MAX_PATH);
	getDocHome(currentDocHomePath, MAX_PATH);
}


//#################################################################################	

char * KMiscTools::makeHomeDocFilePath(const char *filename)
{
	long		i =0 ;
	long		index ;
	char *builtPath = currentPath; //new char [MAX_PATH*2]; //[MAX_PATH*];
	memset(builtPath, 0, MAX_PATH*2);
	if ( filename[0] == '\\' )	
		filename++ ;
	if( filename[0] == '/' ) {
		snprintf( builtPath, MAX_PATH, "%s", filename );		
	}
	else {	
		snprintf(builtPath, MAX_PATH, "%s/%s", currentDocHomePath, filename);
	}
	//pour mac, conversion de fichier en :
	i = strlen(builtPath ) ;
	for ( index = 0 ; index <  i ; index++ ) 
	{
		if ( builtPath[index] == '\\' )		
			builtPath[index] = '/' ;			
	}	
	return	builtPath;	
}

char * KMiscTools::makeTempFilePath(const	char *filename)
{
	long		i =0 ;
	long		index ;
	char *builtPath = currentPath; //new char [MAX_PATH*2]; //[MAX_PATH*];
	memset(builtPath, 0, MAX_PATH*2);
	if ( filename[0] == '\\' )	
		filename++ ;
	if( filename[0] == '/' ) {
		snprintf( builtPath, MAX_PATH, "%s", filename );		
	}
	else {	
		snprintf(builtPath, MAX_PATH, "%s/%s", currentTempPath, filename);
	}
	//pour mac, conversion de fichier en :
	i = strlen(builtPath ) ;
	for ( index = 0 ; index <  i ; index++ ) 
	{
		if ( builtPath[index] == '\\' )		
			builtPath[index] = '/' ;			
	}	
	return	builtPath;
}

char * KMiscTools::makeFilePath(const	char *filename)
{
	long		i =0 ;
	long		index ;
	char *builtPath = currentPath; //new char [MAX_PATH*2]; //[MAX_PATH*];
	memset(builtPath, 0, MAX_PATH*2);
	if ( filename[0] == '\\' )	
		filename++ ;
	if( filename[0] == '/' ) {
		snprintf( builtPath, MAX_PATH, "%s", filename );		
	}
	else {	
		snprintf(builtPath, MAX_PATH, "%s/%s", currentAppPath, filename);
	}
	//pour mac, conversion de fichier en :
	i = strlen(builtPath ) ;
	for ( index = 0 ; index <  i ; index++ ) 
	{
		if ( builtPath[index] == '\\' )		
			builtPath[index] = '/' ;			
	}	
	return	builtPath;
}



//#################################################################################	
int KMiscTools::getCurrentDir(char * path, unsigned int maxLen)
{
   NSString * cur = [[NSFileManager defaultManager] currentDirectoryPath];
   return getCString(cur, path, maxLen);    
}



//#################################################################################	
int KMiscTools::getUserHome(char * path, unsigned int maxLen)
{
    NSString * home = NSHomeDirectory();
	return getCString(home, path, maxLen);
}



//#################################################################################	
int KMiscTools::getDocHome(char * path, unsigned int maxLen)
{
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); //creates paths so that you can pull the app's path from it
		NSString *doc = [paths objectAtIndex:0]; //gets the applications directory on the users iPhone
   // NSString * doc = NSDocumentDirectory();
	return getCString(doc, path, maxLen);
}



//#################################################################################	
int KMiscTools::getTempHome(char * path, unsigned int maxLen)
{
    NSString * home = NSTemporaryDirectory();
	return getCString(home, path, maxLen);
}



//#################################################################################	
int KMiscTools::getAppHome(char * path, unsigned int maxLen)
{
    NSBundle * mb = [NSBundle mainBundle];
	NSString * mbp = [mb bundlePath];
	return getCString(mbp, path, maxLen);
}


//#################################################################################	
unsigned short KMiscTools::pathSeparator(void)
{
    return '/';
}


//#################################################################################	
void KMiscTools::appendPath(char * path, char * component)
{
    unsigned int len = strlen(path);
	if ( path[len] != pathSeparator() )
	{
		path[len] = (char) pathSeparator();
		path[len+1] = 0;
	}
    char * p = component;
	if ( *p == pathSeparator() ) p++;
	strcat(path, p);
}


//#################################################################################	
char * KMiscTools::concatePath(char * path1, char * path2)
{
     size_t len1 = strlen(path1);
	 size_t len2 = strlen(path2);
     char * resultPath = (char * ) malloc( len1 + len2 + 1 );
	 strcpy(resultPath, path1);
	 
	 appendPath(resultPath, path2);
	 
	 return resultPath;	 
}

/*
bool KMiscTools::isIphone6()
{
	if(UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone )
	{
		CGRect f = [[UIScreen mainScreen] bounds];
		if(f.size.width > 320) {
			return true;
		}
	}
	return false;
}
*/

bool KMiscTools:: iPad()
{
    UIUserInterfaceIdiom idiom = [[UIDevice currentDevice] userInterfaceIdiom];
    //NSLog(@"%ld", idiom);
    if (idiom == UIUserInterfaceIdiomPad) {
        return true;
    }
	return false;
}

bool KMiscTools::iPadPro()
{
	//#define iPadPro12 (UIDevice.currentDevice.userInterfaceIdiom == UIUserInterfaceIdiomPad && UIScreen.mainScreen.nativeBounds.size.height == 2732)
	int h = UIScreen.mainScreen.nativeBounds.size.height;
	if(h == 1334 || h == 2732) {
		return true;
	}
//	CGRect f = [[UIScreen mainScreen] bounds];
//	if(f.size.width > 2048) {
//		return true;
//	}
	return false;
}

bool KMiscTools::iPadMini()
{
	NSString *machName = nil;

	if (uname(&systemInfo) < 0) {
	} else {
		machName = [NSString stringWithCString:systemInfo.machine
								  encoding:NSUTF8StringEncoding];
	}
	if (machName == nil) {
		return false;
	}
	if (    [machName isEqualToString:@"iPad2,5"]
		|| [machName isEqualToString:@"iPad2,6"]
		|| [machName isEqualToString:@"iPad2,7"])
	{
		return true;
	}
	return false;
}

float KMiscTools::getScale()
{
	float scale = 1;
	if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) {
		scale = [[UIScreen mainScreen] scale];
	}
	return scale;
}

int KMiscTools::screenDPI()
{
	UIScreen * mainscr = [UIScreen mainScreen];
	int w = mainscr.nativeBounds.size.width;
	int h = mainscr.nativeBounds.size.height;
	
	if(iPadMini() == true) {
		return 163;
	}
	float scale = 1;
	if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) {
		scale = [[UIScreen mainScreen] scale];
	}
	float dpi;
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		dpi = 132 * scale;
	}
	else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
	{
		if (w == 1080 && h == 1920) // 5.5" Retina display detected (iPhone 6+ 7+ or 8+)
		{
			return 401*2;
		}
		
		if(h == 2436)
		{
			return 401*2; // iPhone X
		}
		
		dpi = 163 * scale;
		
	}
	else {
		dpi = 160 * scale;
	}
	return (int)dpi;
}

void KMiscTools::getUserLanguage(char * lang)
{
	NSString * currentLanguage = [[NSLocale preferredLanguages] objectAtIndex:0];
	NSLog(@"lang = %@", currentLanguage);
	getCString(currentLanguage, lang, 16);
	
	/*
	NSUserDefaults	*	defaults		= [NSUserDefaults standardUserDefaults];
	NSArray			*	languages		= [defaults objectForKey:@"AppleLanguages"];
	NSString		*	currentLanguage2 = [languages objectAtIndex:0];
	NSLog(@"lang = %@", currentLanguage2);
	
	NSString		*	currentLanguage3 = [[[NSBundle mainBundle] preferredLocalizations] objectAtIndex:0];
	NSLog(@"lang = %@", currentLanguage3);
	
	NSString		*	currentLanguage4 = [[NSLocale preferredLanguages] objectAtIndex:0];
	NSLog(@"lang = %@", currentLanguage4);
	
	NSLocale		*	locale			= [NSLocale currentLocale];
	NSString		*	currentLanguage5 = [locale objectForKey:NSLocaleLanguageCode];
	NSLog(@"lang = %@", currentLanguage5);*/
}

bool KMiscTools::isInternetReachable()
{
	struct sockaddr_in zeroAddress;
	bzero(&zeroAddress, sizeof(zeroAddress));
	zeroAddress.sin_len = sizeof(zeroAddress);
	zeroAddress.sin_family = AF_INET;
	
	SCNetworkReachabilityRef reachability = SCNetworkReachabilityCreateWithAddress(kCFAllocatorDefault, (const struct sockaddr*)&zeroAddress);
	SCNetworkReachabilityFlags flags;
	
	if(reachability == NULL)
		return false;
	
	if (!(SCNetworkReachabilityGetFlags(reachability, &flags)))
		return false;
	
	if ((flags & kSCNetworkReachabilityFlagsReachable) == 0)
		// if target host is not reachable
		return false;
	
	
	BOOL isReachable = false;
	
	
	if ((flags & kSCNetworkReachabilityFlagsConnectionRequired) == 0)
	{
		// if target host is reachable and no connection is required
		//  then we'll assume (for now) that your on Wi-Fi
		isReachable = true;
	}
	
	
	if ((((flags & kSCNetworkReachabilityFlagsConnectionOnDemand ) != 0) ||
		 (flags & kSCNetworkReachabilityFlagsConnectionOnTraffic) != 0))
	{
		// ... and the connection is on-demand (or on-traffic) if the
		//     calling application is using the CFSocketStream or higher APIs
		
		if ((flags & kSCNetworkReachabilityFlagsInterventionRequired) == 0)
		{
			// ... and no [user] intervention is needed
			isReachable = true;
		}
	}
	
	if ((flags & kSCNetworkReachabilityFlagsIsWWAN) == kSCNetworkReachabilityFlagsIsWWAN)
	{
		// ... but WWAN connections are OK if the calling application
		//     is using the CFNetwork (CFSocketStream?) APIs.
		isReachable = true;
	}
	
	return isReachable;
}

int KMiscTools::getUserID(char * path)
{
	CFUUIDRef cfuuid = CFUUIDCreate(NULL);
	NSString *uuid =  (__bridge_transfer NSString *)CFUUIDCreateString(NULL, cfuuid);
	CFRelease(cfuuid);
	return getCString(uuid, path, 512);
	//return uuid;
	
	/*
	NSString *uuid;
	KeychainItemWrapper *keychain = [[KeychainItemWrapper alloc] initWithIdentifier:@"AC_APP_UUID" accessGroup:nil];
	NSString *keychainUUID = [keychain objectForKey:(__bridge id)(kSecAttrAccount)];
	NSString *appVersion = [NSString stringWithFormat:@"%@",@"1.0"];
	[keychain setObject:appVersion forKey:(__bridge id)(kSecAttrDescription)];
	if (keychainUUID==nil||[keychainUUID isKindOfClass:[NSNull class]]||keychainUUID.length==0) {
		uuid = [[NSUUID UUID] UUIDString];
		[keychain setObject:uuid forKey:(__bridge id)(kSecAttrAccount)];
	}else{
		uuid = [keychain objectForKey:(__bridge id)(kSecAttrAccount)];
	}*/
}

#define USER_KEY					@"com.jadegame.ptk.user"	// iCloud Container Key
#define KEYCHAIN_ACCOUNT_IDENTIFIER @"com.jadegame.ptk"			// KeyChain key

/*
int KMiscTools::createUserID2(char * uid)
{
    // Keychain query dictionary
    NSDictionary *query = @{
        (__bridge id)kSecClass:          (__bridge id)kSecClassGenericPassword,
        (__bridge id)kSecAttrService:    @"YourServiceName", // Replace with USER_KEY
        (__bridge id)kSecAttrAccount:    @"YourAccountName", // Replace with KEYCHAIN_ACCOUNT_IDENTIFIER
        (__bridge id)kSecReturnData:     @YES,
    };

    CFTypeRef result = NULL;
    OSStatus status = SecItemCopyMatching((__bridge CFDictionaryRef)query, &result);

    NSString *userID = nil;
    if (status == errSecSuccess) {
        // Successfully retrieved the userID from the keychain
        NSData *data = (__bridge_transfer NSData *)result;
        userID = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    } else if (status == errSecItemNotFound) {
        // Item not found; you may choose to create and store a new userID here
        userID = [[[NSUUID UUID] UUIDString] copy];
        
        // Store the new userID in the keychain
        NSData *userIDData = [userID dataUsingEncoding:NSUTF8StringEncoding];
        NSDictionary *addQuery = @{
            (__bridge id)kSecClass:       (__bridge id)kSecClassGenericPassword,
            (__bridge id)kSecAttrService: USER_KEY,
            (__bridge id)kSecAttrAccount: KEYCHAIN_ACCOUNT_IDENTIFIER,
            (__bridge id)kSecValueData:   userIDData,
        };
        status = SecItemAdd((__bridge CFDictionaryRef)addQuery, NULL);
        if (status != errSecSuccess) {
            // Handle the error appropriately
            return -1;
        }
    } else {
        // Handle other potential errors (e.g., keychain access denied)
        return -1;
    }

    // Continue with your existing logic
    NSUUID *userUUID = [[NSUUID alloc] initWithUUIDString:userID];
    if (userUUID == nil) {
        // Handle invalid UUID string
        return -1;
    }

    NSString *userID_uuid = [userUUID UUIDString];
    if ([userID_uuid isEqualToString:@"EDE99CD2-D513-4ED7-874F-E09E6E8276A5"]) {
        userID_uuid = @"4D70ADAD-37AD-47E0-A2E2-505080DD19AA";
    }

    BOOL success = [userID_uuid getCString:uid maxLength:512 encoding:NSUTF8StringEncoding];
    return success ? 0 : -1;
}*/

int KMiscTools::createUserID(char * uid)
{
    return 0;
    /*
	NSString *userID = [SAMKeychain passwordForService:USER_KEY account:KEYCHAIN_ACCOUNT_IDENTIFIER];
	if (userID) {
		NSUUID * userUUID = [[NSUUID UUID] initWithUUIDString:userID];
		NSString * userID_uuid = [userUUID UUIDString];
		if([userID_uuid isEqualToString:@"EDE99CD2-D513-4ED7-874F-E09E6E8276A5"] == YES) {
			userID_uuid = @"4D70ADAD-37AD-47E0-A2E2-505080DD19AA";
		}
		return getCString(userID_uuid, uid, 512);
	}
	
    
	// check iCloud
	userID = [[NSUbiquitousKeyValueStore defaultStore] stringForKey:USER_KEY];
	//if (!userID)
	if (!userID || [userID isEqualToString:@""] == YES)
	{
		// none in iCloud, create one
		NSUUID *newUUID = [NSUUID UUID];
		userID = [newUUID UUIDString];
		// save to iCloud
		[[NSUbiquitousKeyValueStore defaultStore] setString:userID forKey:USER_KEY];
	}
	
	// store the user ID locally
	[SAMKeychain setPassword:userID forService:USER_KEY account:KEYCHAIN_ACCOUNT_IDENTIFIER];
	NSUUID * userUUID = [[NSUUID UUID] initWithUUIDString:userID];
	NSString * userID_uuid = [userUUID UUIDString];
	if([userID_uuid isEqualToString:@"EDE99CD2-D513-4ED7-874F-E09E6E8276A5"] == YES) {
		userID_uuid = @"4D70ADAD-37AD-47E0-A2E2-505080DD19AA";
	}
	return getCString(userID_uuid, uid, 512);*/
}



