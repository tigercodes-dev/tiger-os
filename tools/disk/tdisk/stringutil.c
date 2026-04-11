#include "stringutil.h"
#include <ctype.h>

int stricmp(const char* a, const char* b) {
    for (; *a && *b; a++, b++) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            break;
        }
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}
