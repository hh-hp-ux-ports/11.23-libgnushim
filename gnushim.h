/* gnushim.h — libgnushim v2: declarations for the libc functions HP-UX 11.23 lacks, supplied by
 * the static libgnushim.a (gnulib 2020-vintage, both ABIs). Every function here was VERIFIED
 * missing by link-probe on rx2620 2026-07-22 (probe/probe.sh in the libgnushim repo).
 * Link: -L/opt/gnu/lib[/hpux64] -lgnushim.
 * NOTE: some symbols may be exported by libgnushim.a under gnulib's rpl_ prefix on this platform;
 * build_rx2620.sh nm-verifies the plain names and fails loudly if they are not present. */
#ifndef GNUSHIM_H
#define GNUSHIM_H
#include <sys/types.h>   /* ssize_t, size_t */
#include <stdio.h>       /* FILE */
#include <stdarg.h>      /* va_list */
#include <time.h>        /* struct timespec */
#ifdef __cplusplus
extern "C" {
#endif

/* environment */
int setenv(const char *name, const char *value, int overwrite);
int unsetenv(const char *name);

/* stdio line input */
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
ssize_t getdelim(char **lineptr, size_t *n, int delimiter, FILE *stream);

/* strings */
size_t strnlen(const char *s, size_t maxlen);
char *strndup(const char *s, size_t n);
char *strcasestr(const char *haystack, const char *needle);
char *stpcpy(char *dest, const char *src);
char *stpncpy(char *dest, const char *src, size_t n);
int strverscmp(const char *a, const char *b);
void *mempcpy(void *dest, const void *src, size_t n);

/* printf allocation */
int asprintf(char **strp, const char *fmt, ...);
int vasprintf(char **strp, const char *fmt, va_list ap);

/* files */
int mkstemps(char *template_, int suffixlen);
int futimens(int fd, const struct timespec times[2]);
char *canonicalize_file_name(const char *name);

/* GNU getopt_long */
#ifndef no_argument
#define no_argument       0
#define required_argument 1
#define optional_argument 2
struct option { const char *name; int has_arg; int *flag; int val; };
#endif
int getopt_long(int argc, char *const argv[], const char *optstring,
                const struct option *longopts, int *longindex);

#ifdef __cplusplus
}
#endif
#endif /* GNUSHIM_H */
