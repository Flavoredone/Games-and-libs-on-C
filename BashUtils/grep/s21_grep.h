#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flags {
  char *reg;
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} Flags;

int parse(int argc, char *argv[], Flags *flags, int *file_index);
int regopen(Flags *flags, char *path);
void compilereg(int argc, char *argv[], Flags *flags, int file_index);
void grep21(regex_t *pattern, char *path, Flags *flags, int count_files);

#endif