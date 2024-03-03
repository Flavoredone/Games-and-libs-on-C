#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flags {
  int b;
  int n;
  int s;
  int v;
  int T;
  int E;
} Flags;

int cat21(char* file_name, Flags* flags);
void parse(int argc, char* argv[], Flags* flags);