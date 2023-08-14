#pragma once

#include "Geom.h"

#include <stdint.h>
#include <stdio.h>
#include <sys/_types/_errno_t.h>

inline Point MakePoint(uint8_t x, uint8_t y)
{
    return {static_cast<short>(x), static_cast<short>(y)};
}

errno_t fopen_s(FILE **f, const char *name, const char *mode);

template < typename T, size_t N >
constexpr size_t _countof( T ( & arr )[ N ] ) noexcept
{
    return N;
}

int _stricmp(const char *string1, const char *string2);

// template <size_t size>
// int sprintf_s(char (&buffer)[size], const char *format, ...);

int _mkdir(const char *dirname);
