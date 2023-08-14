/*
   Copyright 2016 Aldo J. Nunez

   Licensed under the Apache License, Version 2.0.
   See the LICENSE text file for details.
*/

#pragma once

// Allegro
#include <allegro5/allegro5.h>
// #include <allegro5/allegro_windows.h>

// C
// #include <crtdbg.h>
#include <stdio.h>

// C++
#include <cmath>
#include <memory>
#include <array>

// This project
#include "Global.h"

errno_t fopen_s(FILE **f, const char *name, const char *mode) {
    errno_t ret = 0;
    assert(f);
    *f = fopen(name, mode);
    /* Can't be sure about 1-to-1 mapping of errno and MS' errno_t */
    if (!*f)
        ret = errno;
    return ret;
}

#include "Util.h"

#undef GetObject

template < typename T, size_t N >
size_t _countof( T ( & arr )[ N ] )
{
    return std::extent< T[ N ] >::value;
}

int _stricmp(const char *string1, const char *string2)
{
    return strcasecmp(string1, string2);
}
