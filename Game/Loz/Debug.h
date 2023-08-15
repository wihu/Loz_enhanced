#pragma once

#include <assert.h>
#include <allegro5/base.h>


// extern "C" {

AL_FUNC(bool, trace_prefix, (char const *channel, int level,
   char const *file, int line, char const *function));
   
AL_PRINTFUNC(void, trace_suffix, (const char *msg, ...), 1, 2);

#define DEBUG_CHANNEL(x) \
    static char const *_debug_channel = x;
   
#define TRACE_CHANNEL_LEVEL(channel, level)                        \
    !trace_prefix(channel, level, __FILE__, __LINE__, __func__)         \
    ? (void)0 : trace_suffix

#define TRACE_LEVEL(x)   TRACE_CHANNEL_LEVEL(_debug_channel, x)
#define LOG_DEBUG        TRACE_LEVEL(0)
#define LOG_INFO         TRACE_LEVEL(1)
#define LOG_WARN         TRACE_LEVEL(2)
#define LOG_ERROR        TRACE_LEVEL(3)

void shutdown_logging(void);

// }
