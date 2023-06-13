#include "s21_cat.h"

void ascii(char read, t_flags *flags) {
  int flag = 0;
  if ((read >= 0 && read <= 8) || (read >= 11 && read <= 31)) {
    printf("%c%c", '^', read + 64);
    flag = 1;
  }
  if (read == 127) {
    printf("%c%c", '^', '?');
    flag = 1;
  }
  if (flags->T == 1 && flags->t == 0 && read == '\t') {
    printf("%s", "^I");
    flag = 1;
  }
  if (flags->E == 1 && flags->e == 0 && read == '\n') {
    printf("%c", '$');
  }
  if (flag == 0) {
    printf("%c", read);
  }
}

void myprintf(char read, t_flags *flags) {
  int flag = 0;
  if (flags->v == 1 && flags->e == 0 && flags->t == 0) {
    ascii(read, flags);
    flag = 1;
  }

  if (flags->e == 1) {
    if (read == '\n') {
      printf("%c", '$');
    }
    if (flags->t == 0) {
      ascii(read, flags);
    }
    flag = 1;
  }

  if (flags->t == 1) {
    if (read == '\t')
      printf("%s", "^I");
    else
      ascii(read, flags);
    flag = 1;
  }

  if ((flags->T == 1 || flags->E == 1) && flags->t == 0 && flags->e == 0 &&
      flags->v == 0) {
    if (flags->E == 1)
      if (read == '\n') {
        printf("%c", '$');
      }
    int p = 0;
    if (flags->T == 1) {
      if (read == '\t') {
        p = 1;
        printf("%s", "^I");
      }
    }
    if (p == 0) {
      printf("%c", read);
    }
    flag = 1;
  }

  if (flag == 0) {
    printf("%c", read);
  }
}

size_t parse_flags(int argc, char *argv[], t_flags *flags) {
  char c = 0;
  size_t i = 0;
  int option_index = 0;
  static struct option long_options[] = {
      {"number", 0, 0, 1},
      {"number-nonblank", 0, 0, 2},
      {"squeeze-blank", 0, 0, 3},
  };
  while (c != -1) {
    c = getopt_long(argc, argv, "vsbteEnvT", long_options, &option_index);
    if (c == 's' || c == 3) flags->s = 2;
    if (c == 'b' || c == 2) flags->b = 1;
    if (c == 'n' || c == 1) flags->n = 1;
    if (c == 'E') flags->E = 1;
    if (c == 'T') flags->T = 1;
    if (c == 'e') flags->e = 1;
    if (c == 't') flags->t = 1;
    if (c == 'v') flags->v = 1;
  }
  return i;
}

int print_flag_b(int n) {
  printf("%6d\t", n);
  n++;
  return n;
}

int print_flag_n(int n) {
  printf("%6d\t", n);
  n++;
  return n;
}

void print_file(FILE *fp, t_flags *flags) {
  int n = 1;
  int flagsb = 0;
  int flagsn = 0;
  char read;
  while ((read = fgetc(fp)) != EOF) {
    if (flags->s == 1 && read == '\n') {
      continue;
    }
    if ((flags->s == 3 || flags->s == 2) && read == '\n') {
      flags->s--;
    }
    if (flags->b == 1 && read != '\n' && flagsb == 0) {
      n = print_flag_b(n);
      flagsb += 1;
    }
    if (flags->b == 1 && flagsb == 1 && read == '\n') {
      flagsb = 0;
    }
    if (flags->n == 1 && flags->b == 0 && flagsn == 0) {
      n = print_flag_n(n);
      flagsn++;
    }
    if (read != '\n' && flags->s != 0) flags->s = 3;
    if (flags->n == 1 && flags->b == 0 && flagsn == 2) {
      n = print_flag_n(n);
      flagsn = 1;
    }

    if (flags->n == 1 && flags->b == 0 && read == '\n' && flagsn == 1) {
      flagsn++;
    }
    myprintf(read, flags);
  }
}

void file_open(char *argv[], t_flags *flags) {
  while (argv[optind]) {
    char *name = argv[optind];
    FILE *fp;
    if ((fp = fopen(name, "r")) == NULL) {
      printf("cat: %s: No such file or directory", name);
      break;
    } else {
      print_file(fp, flags);
    }
    optind++;
  }
}

void s21_cat(int argc, char *argv[]) {
  t_flags flags = {0};
  parse_flags(argc, argv, &flags);
  file_open(argv, &flags);
}

int main(int argc, char *argv[]) {
  s21_cat(argc, argv);
  return 0;
}
