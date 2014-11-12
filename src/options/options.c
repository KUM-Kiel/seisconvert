#include "options.h"

#include <stdio.h>
#include <stdlib.h>

#define NEED_ARGUMENT_LONG  "Option '--%s' needs an argument."
#define NEED_ARGUMENT_SHORT "Option '-%c' needs an argument."
#define UNRECOGNISED_LONG   "Unrecognised option '--%s'."
#define UNRECOGNISED_SHORT  "Unrecognised option '-%c'."
#define UNEXPECTED_ARGUMENT "Option '--%s' expects no argument."

static int str_equal(char *a, char *b)
{
  if (!a || !b) return 0;
  while (*a && *b && *a == *b) {++a; ++b;}
  return *a == *b;
}

static option_t *lookup_short(option_t **o, char n)
{
  while (*o) {
    if ((*o)->shortopt == n) {
      return *o;
    }
    ++o;
  }
  fprintf(stderr, UNRECOGNISED_SHORT "\n", n);
  exit(1);
  return 0;
}

static option_t *lookup_long(option_t **o, char *n)
{
  while (*o) {
    if ((*o)->longopt && str_equal((*o)->longopt, n)) {
      return *o;
    }
    ++o;
  }
  fprintf(stderr, UNRECOGNISED_LONG "\n", n);
  exit(1);
  return 0;
}

static char *find_and_null_eq(char *s)
{
  while (*s) {
    if (*s == '=') {
      *s = 0;
      return s + 1;
    }
    ++s;
  }
  return 0;
}

#define HAS_PARAMETER(o) ((o)->type & OPTION_PARAMETER)
#define HAS_DEFAULT(o) ((o)->type & OPTION_DEFAULT)
#define HAS_CALLBACK(o) ((o)->type & OPTION_CALLBACK)

static void apply_option(option_t *o, char *arg)
{
  if (!arg && HAS_DEFAULT(o)) {
    arg = o->arg_default;
  }
  if (HAS_CALLBACK(o)) {
    o->callback(arg);
  } else if (HAS_PARAMETER(o)) {
    if (o->arg_adr) {
      *(o->arg_adr) = arg;
    }
  } else {
    if (o->flag_adr) {
      *(o->flag_adr) = o->flag_value;
    }
  }
}

int parse_options(int *argc, char ***argv, option_t **options)
{
  int c = 1, i = 1, j;
  char x;
  char *a, *v;
  char **_argv = *argv;
  option_t *o = 0;
  int s_l = 0;

  while (i < *argc) {
    if (_argv[i][0] == '-' && _argv[i][1]) {
      if (o) {
        if (HAS_DEFAULT(o)) {
          apply_option(o, 0);
          o = 0;
        } else {
          if (s_l) {
            fprintf(stderr, NEED_ARGUMENT_LONG "\n", o->longopt);
          } else {
            fprintf(stderr, NEED_ARGUMENT_SHORT "\n", o->shortopt);
          }
          exit(1);
        }
      }
      if (_argv[i][1] == '-' && !_argv[i][2]) {
        /* End of options */
        ++i;
        while (i < *argc) {
          _argv[c] = _argv[i];
          ++c;
          ++i;
        }
      } else if (_argv[i][1] == '-') {
        /* Longopt */
        s_l = 1;
        a = _argv[i] + 2;
        v = find_and_null_eq(a);
        o = lookup_long(options, a);
        if (v || !HAS_PARAMETER(o)) {
          if (v && !HAS_PARAMETER(o)) {
            fprintf(stderr, UNEXPECTED_ARGUMENT "\n", o->longopt);
            exit(1);
          }
          apply_option(o, v);
          o = 0;
        }
      } else {
        /* Shortopt */
        s_l = 0;
        j = 1;
        while ((x = _argv[i][j])) {
          o = lookup_short(options, x);
          if (!HAS_PARAMETER(o)) {
            apply_option(o, 0);
            o = 0;
          } else {
            if (_argv[i][j + 1]) {
              apply_option(o, _argv[i] + j + 1);
              o = 0;
            }
            break;
          }
          ++j;
        }
      }
    } else {
      if (o) {
        apply_option(o, _argv[i]);
        o = 0;
      } else {
        _argv[c] = _argv[i];
        ++c;
      }
    }
    ++i;
  }
  if (o) {
    if (HAS_DEFAULT(o)) {
      apply_option(o, 0);
      o = 0;
    } else {
      if (s_l) {
        fprintf(stderr, NEED_ARGUMENT_LONG "\n", o->longopt);
      } else {
        fprintf(stderr, NEED_ARGUMENT_SHORT "\n", o->shortopt);
      }
      exit(1);
    }
  }

  _argv[c] = 0;
  *argc = c;

  return 0;
}
