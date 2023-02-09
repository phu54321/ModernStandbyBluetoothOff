#include <cstdio>
#include <windows.h>

void logA(const char *format, ...) {
    char buf[1024];
    wvsprintfA(buf, format, ((char *) &format) + sizeof(void *));
    OutputDebugStringA(buf);
    puts(buf);
}
