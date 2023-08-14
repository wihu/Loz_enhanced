#include "Helpers.h"

#include <assert.h>
#include <sys/errno.h>
#include <type_traits>
#include <strings.h>

errno_t fopen_s(FILE **f, const char *name, const char *mode) {
    errno_t ret = 0;
    assert(f);
    *f = fopen(name, mode);
    /* Can't be sure about 1-to-1 mapping of errno and MS' errno_t */
    if (!*f)
        ret = errno;
    return ret;
}

// template < typename T, size_t N >
// size_t _countof( T ( & arr )[ N ] )
// {
//     return std::extent< T[ N ] >::value;
// }

int _stricmp(const char *string1, const char *string2)
{
    return strcasecmp(string1, string2);
}

// template <size_t size>
// int sprintf_s(char (&buffer)[size], const char *format, ...)
// {
//     //TODO: implement.
//     return 0;
// }

int _mkdir(const char *dirname)
{
    //TODO: implement.
    return 0;
}
