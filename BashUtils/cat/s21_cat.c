#include "s21_cat.h"

int main(int argc, char* argv[]) {
  Flags flags = {0, 0, 0, 0, 0, 0};
  int cnt = 0;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-')
      cnt++;
    else {
      cnt++;
      break;
    }
  }

  parse(cnt, argv, &flags);
  for (int i = cnt; i < argc; i++) cat21(argv[i], &flags);
  return 0;
}

void parse(int argc, char* argv[], Flags* flags) {
  char* fl = "benstvET";
  struct option gnuflags[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0},
  };
  int res = getopt_long(argc, argv, fl, gnuflags, NULL);

  while (res != -1) {
    switch (res) {
      case 'b':
        flags->b = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'T':
        flags->T = 1;
        break;
      case 'E':
        flags->E = 1;
        break;
      case 'e':
        flags->E = 1;
        flags->v = 1;
        break;
      case 't':
        flags->T = 1;
        flags->v = 1;
        break;
      default:
        exit(1);
    }
    res = getopt_long(argc, argv, fl, gnuflags, NULL);
  }
  if (flags->b && flags->n) flags->n = 0;
}

int cat21(char* filename, Flags* flags) {
  FILE* file = fopen(filename, "r");

  if (file == NULL) {
    fprintf(stderr, "cat: %s: No such file or directory\n", filename);
    return -1;
  } else {
    int cnt = 0, end = 0, prev = '\n';
    int cur = fgetc(file);

    while (cur != EOF) {
      if (flags->s && cur == '\n' && prev == '\n' && end) {
      } else {
        if (prev == '\n' && cur == '\n')
          end = 1;
        else
          end = 0;

        if (prev == '\n') {
          if ((flags->b && cur != '\n') || flags->n) {
            cnt++;
            printf("%6d\t", cnt);
          }
        }
        if ((cur >= 0 && cur <= 31) || cur == 127) {
          if (flags->E && cur == '\n') {
            printf("$");
          } else if ((flags->T && cur == '\t') ||
                     (flags->v && cur != '\n' && cur != '\t')) {
            printf("^");
            cur = (cur + 64) % 128;
          }
        }
        printf("%c", cur);
      }
      prev = cur;
      cur = fgetc(file);
    }
  }
  fclose(file);
  return 0;
}