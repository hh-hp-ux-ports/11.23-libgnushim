/* aliases.c — plain-name wrappers for symbols this gnulib vintage exports under its rpl_ prefix.
 * build_rx2620.sh nm-scans libgnu.a per ABI and defines NEED_ALIAS_<NAME> for exactly the symbols
 * that came out prefixed; everything here is an exact-signature passthrough (no globals involved —
 * getopt, which has global state, is handled by our own getopt_long.c instead). */
#include <sys/types.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#ifdef NEED_ALIAS_GETLINE
extern ssize_t rpl_getline(char **lineptr, size_t *n, FILE *stream);
ssize_t getline(char **lineptr, size_t *n, FILE *stream)
{ return rpl_getline(lineptr, n, stream); }
#endif
#ifdef NEED_ALIAS_GETDELIM
extern ssize_t rpl_getdelim(char **lineptr, size_t *n, int delim, FILE *stream);
ssize_t getdelim(char **lineptr, size_t *n, int delim, FILE *stream)
{ return rpl_getdelim(lineptr, n, delim, stream); }
#endif
#ifdef NEED_ALIAS_STRNLEN
extern size_t rpl_strnlen(const char *s, size_t maxlen);
size_t strnlen(const char *s, size_t maxlen) { return rpl_strnlen(s, maxlen); }
#endif
#ifdef NEED_ALIAS_STRNDUP
extern char *rpl_strndup(const char *s, size_t n);
char *strndup(const char *s, size_t n) { return rpl_strndup(s, n); }
#endif
#ifdef NEED_ALIAS_STRCASESTR
extern char *rpl_strcasestr(const char *h, const char *n);
char *strcasestr(const char *h, const char *n) { return rpl_strcasestr(h, n); }
#endif
#ifdef NEED_ALIAS_STPCPY
extern char *rpl_stpcpy(char *d, const char *s);
char *stpcpy(char *d, const char *s) { return rpl_stpcpy(d, s); }
#endif
#ifdef NEED_ALIAS_STPNCPY
extern char *rpl_stpncpy(char *d, const char *s, size_t n);
char *stpncpy(char *d, const char *s, size_t n) { return rpl_stpncpy(d, s, n); }
#endif
#ifdef NEED_ALIAS_STRVERSCMP
extern int rpl_strverscmp(const char *a, const char *b);
int strverscmp(const char *a, const char *b) { return rpl_strverscmp(a, b); }
#endif
#ifdef NEED_ALIAS_MEMPCPY
extern void *rpl_mempcpy(void *d, const void *s, size_t n);
void *mempcpy(void *d, const void *s, size_t n) { return rpl_mempcpy(d, s, n); }
#endif
#ifdef NEED_ALIAS_ASPRINTF
extern int rpl_vasprintf(char **strp, const char *fmt, va_list ap);
int asprintf(char **strp, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = rpl_vasprintf(strp, fmt, ap); va_end(ap); return r; }
#endif
#ifdef NEED_ALIAS_VASPRINTF
extern int rpl_vasprintf(char **strp, const char *fmt, va_list ap);
int vasprintf(char **strp, const char *fmt, va_list ap) { return rpl_vasprintf(strp, fmt, ap); }
#endif
#ifdef NEED_ALIAS_MKSTEMPS
extern int rpl_mkstemps(char *template_, int suffixlen);
int mkstemps(char *template_, int suffixlen) { return rpl_mkstemps(template_, suffixlen); }
#endif
#ifdef NEED_ALIAS_FUTIMENS
extern int rpl_futimens(int fd, const struct timespec times[2]);
int futimens(int fd, const struct timespec times[2]) { return rpl_futimens(fd, times); }
#endif
#ifdef NEED_ALIAS_CANONICALIZE_FILE_NAME
extern char *rpl_canonicalize_file_name(const char *name);
char *canonicalize_file_name(const char *name) { return rpl_canonicalize_file_name(name); }
#endif
#ifdef NEED_ALIAS_SETENV
extern int rpl_setenv(const char *name, const char *value, int overwrite);
int setenv(const char *name, const char *value, int overwrite) { return rpl_setenv(name, value, overwrite); }
#endif
#ifdef NEED_ALIAS_UNSETENV
extern int rpl_unsetenv(const char *name);
int unsetenv(const char *name) { return rpl_unsetenv(name); }
#endif
