/* getopt_long.c — minimal getopt_long for HP-UX 11.23 (no system implementation).
 * Supports: --name, --name=value, --name value, long-option flag/val semantics, and delegates
 * single-dash options to the system getopt(3). No abbreviation matching (htop doesn't need it). */
#include "gnushim.h"
#include <string.h>
#include <stdio.h>

int getopt_long(int argc, char * const argv[], const char *optstring,
                const struct option *longopts, int *longindex)
{
  char *arg, *eq;
  size_t len;
  int i;

  if (optind >= argc) return -1;
  arg = argv[optind];
  if (arg[0] != '-' || arg[1] != '-')
    return getopt(argc, argv, optstring);
  if (arg[2] == '\0') { optind++; return -1; }   /* bare "--" ends parsing */

  arg += 2;
  eq = strchr(arg, '=');
  len = eq ? (size_t)(eq - arg) : strlen(arg);

  for (i = 0; longopts[i].name != NULL; i++) {
    if (strlen(longopts[i].name) == len && strncmp(arg, longopts[i].name, len) == 0) {
      optind++;
      if (longindex) *longindex = i;
      optarg = NULL;
      if (longopts[i].has_arg == required_argument) {
        if (eq) optarg = eq + 1;
        else if (optind < argc) optarg = argv[optind++];
        else {
          if (opterr) fprintf(stderr, "%s: option '--%s' requires an argument\n", argv[0], longopts[i].name);
          return '?';
        }
      } else if (longopts[i].has_arg == optional_argument) {
        if (eq) optarg = eq + 1;
      } else if (eq) {
        if (opterr) fprintf(stderr, "%s: option '--%s' takes no argument\n", argv[0], longopts[i].name);
        return '?';
      }
      if (longopts[i].flag) { *longopts[i].flag = longopts[i].val; return 0; }
      return longopts[i].val;
    }
  }
  if (opterr) fprintf(stderr, "%s: unrecognized option '--%.*s'\n", argv[0], (int)len, arg);
  optind++;
  return '?';
}
