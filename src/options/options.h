#pragma once

typedef struct {
  int type;
  char shortopt;
  char *longopt;
  int *flag_adr;
  int flag_value;
  char **arg_adr;
  char *arg_default;
  void (*callback)(char *arg);
} option_t;

#define OPTION_FLAG 0
#define OPTION_PARAMETER 1
#define OPTION_DEFAULT 2
#define OPTION_CALLBACK 4

#define OPTIONS(...) ((option_t *[]){__VA_ARGS__, 0})
#define FLAG(s, l, f, v) (&((option_t){OPTION_FLAG, s, l, &(f), v, 0, 0, 0}))
#define FLAG_CALLBACK(s, l, c) (&((option_t){OPTION_FLAG | OPTION_CALLBACK, s, l, 0, 0, 0, 0, c}))
#define PARAMETER(s, l, p) (&((option_t){OPTION_PARAMETER, s, l, 0, 0, &(p), 0, 0}))
#define PARAMETER_CALLBACK(s, l, c) (&((option_t){OPTION_PARAMETER | OPTION_CALLBACK, s, l, 0, 0, 0, 0, c}))
#define PARAMETER_DEFAULT(s, l, p, v) (&((option_t){OPTION_PARAMETER | OPTION_DEFAULT, s, l, 0, 0, &(p), v, 0}))
#define PARAMETER_DEFAULT_CALLBACK(s, l, v, c) (&((option_t){OPTION_PARAMETER | OPTION_DEFAULT | OPTION_CALLBACK, s, l, 0, 0, 0, v, c}))

extern int parse_options(int *argc, char ***argv, option_t **options);
