// KMiscTools.cpp

#include "KMiscTools.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#if defined(__ANDROID__)
// Android-specific includes if needed
#else
#include <mach-o/dyld.h>
#endif

char KMiscTools::currentAppPath[MAX_PATH];
char KMiscTools::currentFilePath[MAX_PATH];
char KMiscTools::currentTempPath[MAX_PATH];
char KMiscTools::currentDocHomePath[MAX_PATH];
bool KMiscTools::_inPackage;
char KMiscTools::currentPath[MAX_PATH*2];

static uint64_t startTimeNs = 0;

static inline uint64_t getTimeNs() {
#if defined(__ANDROID__)
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
#else
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
        return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
    }
    return 0;
#endif
}

unsigned long KMiscTools::getMilliseconds(void)
{
#if defined(__ANDROID__)
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
        return (unsigned long)((ts.tv_sec * 1000UL) + (ts.tv_nsec / 1000000UL));
    }
    return 0;
#else
    uint64_t now = getTimeNs();
    if(startTimeNs == 0) {
        startTimeNs = now;
    }
    uint64_t diff = now - startTimeNs;
    return (unsigned long)(diff / 1000000UL);
#endif
}

long KMiscTools::getSeed( void )
{
#if defined(__ANDROID__)
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
        return (long)ts.tv_nsec;
    }
    return 0;
#else
    uint64_t now = getTimeNs();
    return (long)(now & 0xFFFFFFFF);
#endif
}

void KMiscTools::getSerial( char *s )
{
    memset(s, 0, 45);
}

void KMiscTools::initMiscTools( void )
{
    _inPackage = true;

    // Initialize timing
    startTimeNs = getTimeNs();

    memset(currentAppPath, 0, MAX_PATH);
    getAppHome(currentAppPath, MAX_PATH);

    memset(currentFilePath, 0, MAX_PATH);
    getUserHome(currentFilePath, MAX_PATH);

    memset(currentTempPath, 0, MAX_PATH);
    getTempHome(currentTempPath, MAX_PATH);

    memset(currentDocHomePath, 0, MAX_PATH);
    getDocHome(currentDocHomePath, MAX_PATH);
    
    printf("currentAppPath: %s\n", currentAppPath);
    printf("currentFilePath: %s\n", currentFilePath);
    printf("currentTempPath: %s\n", currentTempPath);
    printf("currentDocHomePath: %s\n", currentDocHomePath);
}

char * KMiscTools::makeHomeDocFilePath(const char *filename)
{
#if defined(__ANDROID__)
    return (char *)filename;
#else
    long index;
    char *builtPath = currentPath;
    memset(builtPath, 0, MAX_PATH*2);
    if ( filename[0] == '\\' )
        filename++ ;
    if( filename[0] == '/' ) {
        snprintf(builtPath, MAX_PATH, "%s", filename);
    }
    else {
        snprintf(builtPath, MAX_PATH, "%s/%s", currentDocHomePath, filename);
    }
    long i = (long)strlen(builtPath);
    for ( index = 0 ; index < i ; index++ ) {
        if ( builtPath[index] == '\\' )
            builtPath[index] = '/';
    }
    return    builtPath;
#endif
}

char * KMiscTools::makeTempFilePath(const char *filename)
{
#if defined(__ANDROID__)
    return (char *)filename;
#else
    long index;
    char *builtPath = currentPath;
    memset(builtPath, 0, MAX_PATH*2);
    if ( filename[0] == '\\' )
        filename++ ;
    if( filename[0] == '/' ) {
        snprintf(builtPath, MAX_PATH, "%s", filename);
    }
    else {
        snprintf(builtPath, MAX_PATH, "%s/%s", currentTempPath, filename);
    }
    long i = (long)strlen(builtPath);
    for ( index = 0 ; index < i ; index++ ) {
        if ( builtPath[index] == '\\' )
            builtPath[index] = '/';
    }
    return    builtPath;
#endif
}

char * KMiscTools::makeFilePath(const char *filename)
{
#if defined(__ANDROID__)
    return (char *)filename;
#else
    long index;
    char *builtPath = KMiscTools::currentPath;
    memset(builtPath, 0, MAX_PATH*2);
    if ( filename[0] == '\\' )
        filename++ ;
    if( filename[0] == '/' ) {
        snprintf(builtPath, MAX_PATH, "%s", filename);
    }
    else {
        snprintf(builtPath, MAX_PATH, "%s/%s", KMiscTools::currentAppPath, filename);
    }
    long i = (long)strlen(builtPath);
    for ( index = 0 ; index < i ; index++ ) {
        if ( builtPath[index] == '\\' )
            builtPath[index] = '/';
    }
    return    builtPath;
#endif
}

int KMiscTools::getCurrentDir(char * path, unsigned int maxLen)
{
#if defined(__ANDROID__)
    // On Android, current directory concept may differ, just return 0 or implement if needed.
    return 0;
#else
    if (getcwd(path, maxLen) != NULL) {
        return (int)strlen(path);
    }
    return 0;
#endif
}

int KMiscTools::getUserHome(char * path, unsigned int maxLen)
{
#if defined(__ANDROID__)
    // Android: Typically /data/data/<package> is app home, but we don't have direct access here
    // Return empty or some default.
    if (maxLen > 0) {
        path[0] = '\0';
    }
    return 0;
#else
    const char *home = getenv("HOME");
    if (!home) {
        // If HOME not set, fallback to something
        home = "/";
    }
    size_t len = strlen(home);
    if (len >= maxLen) len = maxLen - 1;
    memcpy(path, home, len);
    path[len] = '\0';
    return (int)len;
#endif
}

int KMiscTools::getDocHome(char * path, unsigned int maxLen)
{
#if defined(__ANDROID__)
    if (maxLen > 0) {
        path[0] = '\0';
    }
    return 0;
#else
    char userHome[MAX_PATH];
    getUserHome(userHome, MAX_PATH);
    int len = snprintf(path, maxLen, "%s/Documents", userHome);
    if (len < 0 || (unsigned int)len >= maxLen) {
        return 0;
    }
    return len;
#endif
}

int KMiscTools::getTempHome(char * path, unsigned int maxLen)
{
#if defined(__ANDROID__)
    if (maxLen > 0) {
        path[0] = '\0';
    }
    return 0;
#else
    const char *tmp = getenv("TMPDIR");
    if (!tmp) {
        tmp = "/tmp";
    }
    size_t len = strlen(tmp);
    if (len >= maxLen) len = maxLen - 1;
    memcpy(path, tmp, len);
    path[len] = '\0';
    return (int)len;
#endif
}

int KMiscTools::getAppHome(char * path, unsigned int maxLen)
{
#if defined(__ANDROID__)
    if (maxLen > 0) {
        path[0] = '\0';
    }
    return 0;
#else
    uint32_t size = (uint32_t)maxLen;
    if (_NSGetExecutablePath(path, &size) == 0) {
        // path might look like:
        // "/private/var/containers/Bundle/Application/UUID/MyApp.app/MyApp"
        // Strip off the executable name:
        char *lastSlash = strrchr(path, '/');
        if (lastSlash) {
            *lastSlash = '\0';
        }
        return (int)strlen(path);
    } else {
        // Buffer too small or some error occurred
        if (maxLen > 0) {
            path[0] = '\0';
        }
        return 0;
    }
#endif
}

unsigned short KMiscTools::pathSeparator(void)
{
    return '/';
}

void KMiscTools::appendPath(char * path, char * component)
{
    unsigned int len = (unsigned int)strlen(path);
    if (path[len] != pathSeparator()) {
        path[len] = (char) pathSeparator();
        path[len+1] = 0;
    }
    char * p = component;
    if (*p == pathSeparator()) p++;
    strcat(path, p);
}

char * KMiscTools::concatePath(char * path1, char * path2)
{
    size_t len1 = strlen(path1);
    size_t len2 = strlen(path2);
    char * resultPath = (char *) malloc(len1 + len2 + 2);
    strcpy(resultPath, path1);
    appendPath(resultPath, path2);
    return resultPath;
}
