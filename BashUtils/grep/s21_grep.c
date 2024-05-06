#include "s21_grep.h"

int main(int argc, char *argv[]) {
  Flags flags = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  flags.reg = calloc(1, sizeof(char));
  int fi = 0;

  if (parse(argc, argv, &flags, &fi))
    compilereg(argc, argv, &flags, fi);
  free(flags.reg);
  return 0;
}

int parse(int argc, char *argv[], Flags *flags, int *fi) {
  int res, nofl = 1, len = 0;
  char *fl = "e:ivclnhsf:o";

  while ((res = getopt_long(argc, argv, fl, NULL, NULL)) != -1 && nofl) {
    switch (res) {
    case 'e':
      flags->e = 1;
      len = strlen(optarg);
      flags->reg = realloc(flags->reg, strlen(flags->reg) + len + 2);
      strcat(flags->reg, optarg);
      strcat(flags->reg, "|");

      break;
    case 'i':
      flags->i = 1;
      break;
    case 'c':
      flags->c = 1;
      break;
    case 'l':
      flags->l = 1;
      break;
    case 'n':
      flags->n = 1;
      break;
    case 'v':
      flags->v = 1;
      break;
    case 'h':
      flags->h = 1;
      break;
    case 's':
      flags->s = 1;
      break;
    case 'f':
      flags->f = 1;
      nofl = regopen(flags, optarg);
      break;
    case 'o':
      flags->o = 1;
      break;
    default:
      nofl = 0;
    }
  }
  if (flags->c || flags->l)
    flags->n = 0;
  if (flags->v)
    flags->o = 0;
  *fi = optind;
  return nofl;
}

int regopen(Flags *flags, char *path) {
  FILE *file = fopen(path, "r");
  int err = 1;
  char *str = NULL;
  size_t len_string = 0;

  if (file) {
    while ((getline(&str, &len_string, file)) != -1) {
      int len = strlen(str);
      if (str[len - 1] == '\n')
        str[len - 1] = '\0';
      flags->reg = realloc(flags->reg, strlen(flags->reg) + len + 2);
      strcat(flags->reg, optarg);
      strcat(flags->reg, "|");
    }
    free(str);
    fclose(file);
  } else if (!flags->s) {
    fprintf(stderr, "grep: %s: No such file or directory\n", path);
    err = 0;
  }
  return err;
}

void compilereg(int argc, char *argv[], Flags *flags, int fi) {
  regex_t regex;
  int len = strlen(argv[fi]);

  if (!flags->e && !flags->f) {
    flags->reg = realloc(flags->reg, len + 1);
    strcat(flags->reg, argv[fi]);
    fi++;
  } else
    flags->reg[strlen(flags->reg) - 1] = '\0';

  int count_files = argc - fi;
  int tmp = flags->i ? REG_EXTENDED + REG_ICASE : REG_EXTENDED;

  if (!regcomp(&regex, flags->reg, tmp)) {
    for (int i = fi; i < argc; i++) {
      grep21(&regex, argv[i], flags, count_files);
    }
  }
  regfree(&regex);
}

void grep21(regex_t *regex, char *path, Flags *flags, int count_files) {
  FILE *file = fopen(path, "r");
  char *str = NULL;
  size_t len_string = 0;

  if (file) {
    int line = 1, cfl = 0, end = 0, lfl = 0;
    while ((getline(&str, &len_string, file)) != -1 && !lfl) {
      regmatch_t pmatch[2];
      int reg = regexec(regex, str, 2, pmatch, 0);
      int eo = (int)pmatch[0].rm_eo;
      int so = (int)pmatch[0].rm_so;

      if ((reg == 0 && !flags->v) || (reg == REG_NOMATCH && flags->v)) {
        if (flags->l)
          lfl = 1;

        else if (!flags->c) {
          if (count_files > 1 && !flags->h)
            printf("%s:", path);

          if (flags->n)
            printf("%d:", line);

          if (flags->o) {
            printf("%.*s\n", eo - so, &str[so]);
          } else {
            printf("%s", str);
            end = str[strlen(str) - 1] == '\n' ? 0 : 1;
          }
        }
        if (flags->c)
          cfl++;
      }
      line++;
    }
    if (flags->c) {
      if (count_files > 1 && !flags->h)
        printf("%s:%d\n", path, cfl);
      else
        printf("%d\n", cfl);
    }
    if (flags->l && lfl == 1)
      printf("%s\n", path);

    if (end)
      printf("\n");

    free(str);
    fclose(file);

  } else if (!flags->s)
    fprintf(stderr, "grep: %s: No such file or directory\n", path);
}