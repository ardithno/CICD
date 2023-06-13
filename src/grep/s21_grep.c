#include "s21_grep.h"

void printfFlagO() {
  // regex_t reg;
  // regmatch_t pmatch;
  // int cflags = REG_EXTENDED;
  // int status = 0;
  // if (flags->i == 1) {
  //     cflags = REG_ICASE;}
  // regcomp(&reg, tem, cflags);
  // while (status != 1) {
  //     status = regexec(&reg, line, 1, &pmatch, 0);
  //     for (int i = pmatch.rm_so; i < pmatch.rm_eo; i++) {
  //         if (line[i] != '\0')
  //         printf("%c", line[i]);
  //         line[i] = '\0';}
  //     }
  // regfree(&reg);
}

void fileFlagsF(char* name, char* eopt) {
  FILE* fp;
  char line[1000] = "\0";
  if ((fp = fopen(name, "r")) == NULL) {
    printf("grep: %s: No such file or directory", name);
  } else {
    while (fgets(line, 1000, fp) != NULL) {
      strcat(eopt, line);
      if (eopt[strlen(eopt) - 1] == '\n') eopt[strlen(eopt) - 1] = '|';
      if (eopt[strlen(eopt) - 1] != '|') strcat(eopt, "|");
    }
  }
}

int parse_flags(int argc, char* argv[], t_flags* flags, char* eopt) {
  char c = 0;
  int n = 0;
  while (c != -1) {
    n++;
    c = getopt_long(argc, argv, "e:vcilnhsf:o", NULL, NULL);
    if (c == 'e') {
      n++;
      flags->e = 1;
      strcat(eopt, optarg);
      strcat(eopt, "|");
    }
    if (c == 'i') flags->i = 1;
    if (c == 'c') flags->c = 1;
    if (c == 'v') flags->v = 1;
    if (c == 'n') flags->n = 1;
    if (c == 'l') flags->l = 1;
    if (c == 'h') flags->h = 1;
    if (c == 's') flags->s = 1;
    if (c == 'f') {
      flags->f = 1;
      fileFlagsF(optarg, eopt);
    }
    if (c == 'o') flags->o = 1;
  }
  if (flags->e == 1 || flags->f == 1) eopt[strlen(eopt) - 1] = '\0';
  return n;
}

int regular(char* tem, char* line, t_flags* flags) {
  regex_t reg;
  regmatch_t pmatch;
  int cflags = REG_EXTENDED;
  if (flags->i == 1) {
    cflags = REG_ICASE;
  }
  regcomp(&reg, tem, cflags);
  int status = regexec(&reg, line, 1, &pmatch, 0);
  regfree(&reg);
  return status;
}

int len_argv(char* argv[]) {
  int len = 0;
  while (argv[len]) {
    len++;
  }
  return len;
}

void print_flagv(char* name, char* line, t_flags* flags, int n, char* argv[],
                 int nf, char* tem) {
  if (flags->h == 0 && len_argv(argv) - (nf - 1) > 3) printf("%s:", name);
  if (flags->n == 1) printf("%d:", n);
  if (flags->o == 1 && flags->v == 0)
    printfFlagO(tem, line, flags);
  else
    printf("%s", line);
  if (line[strlen(line) - 1] != '\n') printf("\n");
}

void print_file(FILE* fp, t_flags* flags, char* tem, char* name, char* argv[],
                int nf) {
  char line[1000] = "\0";
  int count = 0;
  int status = 0;
  int n = 0;
  while (fgets(line, 1000, fp) != NULL) {
    n++;
    status = regular(tem, line, flags);
    if (status != REG_NOMATCH) {
      if (flags->c == 1 && flags->v == 0) {
        count += 1;
        if (flags->l == 0) continue;
      }
      if (flags->l == 1 && flags->v == 0) {
        break;
      } else {
        if (flags->v == 0) {
          print_flagv(name, line, flags, n, argv, nf, tem);
        }
      }
    } else {
      if (flags->v == 1) {
        if (flags->c == 1) {
          count += 1;
          if (flags->l == 0) continue;
        }
        if (flags->l == 1) {
          break;
        }
        print_flagv(name, line, flags, n, argv, nf, tem);
      }
    }
  }
  if (flags->c != 0) {
    if (len_argv(argv) - (nf - 1) > 3 && flags->h == 0) printf("%s:", name);
    printf("%d\n", count);
  }
  if (flags->l == 1 && ((status != REG_NOMATCH && flags->v == 0) ||
                        (status == REG_NOMATCH && flags->v == 1)))
    printf("%s\n", name);
}

void file_open(char* argv[], t_flags* flags, char* eopt, int nf) {
  char* tem;
  if (flags->e == 0 && flags->f == 0 && len_argv(argv) > 1) {
    tem = argv[optind];
    optind++;
  } else {
    tem = eopt;
  }
  while (argv[optind]) {
    FILE* fp;
    char* name = argv[optind];
    if ((fp = fopen(name, "r")) == NULL) {
      if (flags->s == 1) {
        optind++;
        continue;
      }
      printf("grep: %s: No such file or directory", name);
    } else {
      print_file(fp, flags, tem, name, argv, nf);
    }
    optind++;
  }
}

void s21_grep(int argc, char* argv[]) {
  int nf = 0;
  char eopt[1000] = "\0";
  t_flags flags = {0};
  nf = parse_flags(argc, argv, &flags, eopt);
  file_open(argv, &flags, eopt, nf);
}

int main(int argc, char* argv[]) {
  s21_grep(argc, argv);
  return 0;
}
