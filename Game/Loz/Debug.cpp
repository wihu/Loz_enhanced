#include <stdio.h>

#include <allegro5/allegro.h>
#include <vector>
#include <string>

/* tracing */
typedef struct TRACE_INFO
{
    bool trace_virgin;
    FILE *trace_file;
    bool need_close;

    /* 0: debug, 1: info, 2: warn, 3: error */
    int level;
    /* 1: line number, 2: function name, 4: timestamp */
    int flags;
    /* List of channels to log. NULL to log all channels. */
    std::vector<std::string> channels;
    std::vector<std::string> excluded;
    /* Whether settings have been read from allegro5.cfg or not. */
    bool configured;
} TRACE_INFO;

static TRACE_INFO trace_info =
    {
        true,
        NULL,
        true,
        0,
        7,
        {},
        {},
        false};

static char static_trace_buffer[2048];

void _configure_logging(void)
{
    ALLEGRO_CONFIG *config;
    char const *v;
    bool got_all = false;

    config = al_get_system_config();
    v = al_get_config_value(config, "trace", "channels");
    if (v)
    {
        ALLEGRO_USTR_INFO uinfo;
        const ALLEGRO_USTR *u = al_ref_cstr(&uinfo, v);
        int pos = 0;

        while (pos >= 0)
        {
            int comma = al_ustr_find_chr(u, pos, ',');
            int first;
            ALLEGRO_USTR *u2, **iter;
            if (comma == -1)
                u2 = al_ustr_dup_substr(u, pos, al_ustr_length(u));
            else
                u2 = al_ustr_dup_substr(u, pos, comma);
            al_ustr_trim_ws(u2);
            first = al_ustr_get(u2, 0);

            if (first == '-')
            {
                al_ustr_remove_chr(u2, 0);
                trace_info.channels.push_back(std::string(al_cstr(u2)));
            }
            else
            {
                if (first == '+')
                    al_ustr_remove_chr(u2, 0);
                trace_info.channels.push_back(std::string(al_cstr(u2)));
                if (!strcmp(al_cstr(u2), "all"))
                    got_all = true;
            }
            pos = comma;
            al_ustr_get_next(u, &pos);
        }

        if (got_all)
        {
            trace_info.channels.clear();
        }
    }

#ifdef DEBUGMODE
    trace_info.level = 0;
#else
    trace_info.level = 9999;
#endif

    v = al_get_config_value(config, "trace", "level");
    if (v)
    {
        if (!strcmp(v, "error"))
            trace_info.level = 3;
        else if (!strcmp(v, "warn"))
            trace_info.level = 2;
        else if (!strcmp(v, "info"))
            trace_info.level = 1;
        else if (!strcmp(v, "debug"))
            trace_info.level = 0;
        else if (!strcmp(v, "none"))
            trace_info.level = 9999;
    }

    v = al_get_config_value(config, "trace", "timestamps");
    if (!v || strcmp(v, "0"))
        trace_info.flags |= 4;
    else
        trace_info.flags &= ~4;

    v = al_get_config_value(config, "trace", "functions");
    if (!v || strcmp(v, "0"))
        trace_info.flags |= 2;
    else
        trace_info.flags &= ~2;

    v = al_get_config_value(config, "trace", "lines");
    if (!v || strcmp(v, "0"))
        trace_info.flags |= 1;
    else
        trace_info.flags &= ~1;

    trace_info.configured = true;
}

static void open_trace_file(void)
{
    if (trace_info.trace_virgin)
    {
        const char *s = getenv("ALLEGRO_TRACE");

        if (s)
        {
            if (!strcmp(s, "-"))
            {
                trace_info.trace_file = stdout;
                trace_info.need_close = false;
            }
            else
            {
                trace_info.trace_file = fopen(s, "w");
            }
        }
        else
#if defined(ALLEGRO_IPHONE) || defined(ALLEGRO_ANDROID) || defined(__EMSCRIPTEN__)
            /* iPhone and Android don't like us writing files, so we'll be doing
             * something else there by default. */
            trace_info.trace_file = NULL;
#else
            trace_info.trace_file = fopen("allegro.log", "w");
#endif

        trace_info.trace_virgin = false;
    }
}

static void do_trace(const char *msg, ...)
{
    va_list ap;
    int s = strlen(static_trace_buffer);
    va_start(ap, msg);
    vsnprintf(static_trace_buffer + s, sizeof(static_trace_buffer) - s,
              msg, ap);
    va_end(ap);
}

bool trace_prefix(char const *channel, int level,
                  char const *file, int line, char const *function)
{
    if (!trace_info.configured)
    {
        _configure_logging();
    }

    if (level < trace_info.level)
        return false;

    const std::vector<std::string> &v = trace_info.channels;
    if (v.empty())
        goto channel_included;

    if (std::find(v.begin(), v.end(), std::string(channel)) != v.end())
    {
        goto channel_included;
    }

    return false;

channel_included:

    const std::vector<std::string> &e = trace_info.excluded;
    if (!e.empty())
    {
        if (std::find(e.begin(), e.end(), std::string(channel)) != e.end())
        {
            return false;
        }
    }

    open_trace_file();

    do_trace("%-8s ", channel);
    if (level == 0)
        do_trace("D ");
    if (level == 1)
        do_trace("I ");
    if (level == 2)
        do_trace("W ");
    if (level == 3)
        do_trace("E ");

    const char *name = strrchr(file, '/');

    if (trace_info.flags & 1)
    {
        do_trace("%20s:%-4d ", name ? name + 1 : file, line);
    }
    if (trace_info.flags & 2)
    {
        do_trace("%-32s ", function);
    }
    if (trace_info.flags & 4)
    {
        double t = 0;
        if (al_is_system_installed())
            t = al_get_time();
        do_trace("[%10.5f] ", t);
    }

    return true;
}

void trace_suffix(const char *msg, ...)
{
    int olderr = errno;
    va_list ap;
    int s = strlen(static_trace_buffer);
    va_start(ap, msg);
    vsnprintf(static_trace_buffer + s, sizeof(static_trace_buffer) - s,
              msg, ap);
    va_end(ap);

    /* We're intentially still writing to a file if it's set even with the
     * additional logging options above. */
    if (trace_info.trace_file)
    {
        fprintf(trace_info.trace_file, "%s", static_trace_buffer);
        fflush(trace_info.trace_file);
    }

    static_trace_buffer[0] = '\0';
    errno = olderr;
}

void shutdown_logging(void)
{
    if (trace_info.configured)
    {
        trace_info.channels.clear();
        trace_info.excluded.clear();

        trace_info.configured = false;
    }

    if (trace_info.trace_file && trace_info.need_close)
    {
        fclose(trace_info.trace_file);
    }

    trace_info.trace_file = NULL;
    trace_info.trace_virgin = true;
}
