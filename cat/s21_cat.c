#include "s21_cat.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Main func
int main(int argc, char* argv[]) {
  config config = {0};
  get_config_cat(&config, argc, argv);

  if (config.help) {
    help_info();
  } else {
    value value = {0, 1, 0};

    if (optind == argc) {
      input_stream_handling(&config, "stdin", &value);
    } else {
      for (int i = optind; i < argc; i++) {
        input_stream_handling(&config, argv[i], &value);
      }
    }
  }
  return 0;
}

// Input processing function
void get_config_cat(config* config, int argc, char** argv) {
  int option;
  const char* short_options = "betvnshET";
  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {"help", no_argument, NULL, 0},
      {NULL, 0, NULL, 0}};
  while ((option = getopt_long(argc, argv, short_options, long_options,
                               NULL)) != -1) {
    if (option == 'b') {
      config->b = 1;
    }
    if (option == 'v') {
      config->v = 1;
    }
    if (option == 'e') {
      config->e = 1;
      config->v = 1;
    }
    if (option == 'n') {
      config->n = 1;
    }
    if (option == 's') {
      config->s = 1;
    }
    if (option == 't') {
      config->t = 1;
      config->v = 1;
    }
    if (option == 'E') {
      config->e = 1;
    }
    if (option == 'T') {
      config->t = 1;
    }
    if (option == 0) {
      config->help = 1;
    }
    if (option == '?') {
      fprintf(stderr, "Try './s21_cat --help' for more information.\n");
      exit(EXIT_FAILURE);
    }
  }

  if (config->b) config->n = 0;
}

// Processing a stream for printing
void input_stream_handling(config* config, char* filename, value* value) {
  FILE* fp = NULL;

  if (!strcmp(filename, "stdin"))
    fp = stdin;
  else if (!access(filename, R_OK))
    fp = fopen(filename, "r");
  else {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", filename);
    return;
  }

  if (fp) {
    char symbols, prev_symbol = '\n';

    while ((symbols = fgetc(fp)) != EOF) {
      char_print(symbols, &prev_symbol, config, value);

      if (symbols == '\n') value->end_file_flag = 0;
    }
  }

  if (fp != stdin) {
    fclose(fp);
    value->end_file_flag = 1;
  }
}

// Printing symbols depending on the config
void char_print(char symbols, char* prev_symbol, config* flags, value* value) {
  if (flags->s && symbols == '\n') {
    if (value->count_s >= 2) return;
    value->count_s += 1;
  } else
    value->count_s = 0;

  if ((!value->end_file_flag) &&
      ((flags->n && *prev_symbol == '\n') ||
       (flags->b && symbols != '\n' && *prev_symbol == '\n'))) {
    value->count_line += 1;
    printf("%6d\t", value->count_line);
  }

  if (flags->e && symbols == '\n') printf("$");

  if (flags->t && symbols == '\t') {
    printf("^");
    symbols = '\t' + 64;
  }

  if (flags->v) {
    v_print(symbols);
  } else {
    printf("%c", symbols);
  }
  *prev_symbol = symbols;
}

// Printing symbols with the v flag turned on
void v_print(unsigned char symbols) {
  if (symbols == 9 || symbols == 10) {
    printf("%c", symbols);
  } else if (symbols >= 32 && symbols < 127) {
    printf("%c", symbols);
  } else if (symbols == 127) {
    printf("^?");
  } else if (symbols >= 128 + 32) {
    printf("M-");
    (symbols < 128 + 127) ? printf("%c", symbols - 128) : printf("^?");
  } else {
    (symbols > 32) ? printf("M-^%c", symbols - 128 + 64)
                   : printf("^%c", symbols + 64);
  }
}

// Help info
void help_info() {
  printf(
      "Usage: cat [OPTION]... [FILE]...\n\
Concatenate FILE(s) to standard output.\n\
\n\
With no FILE, or when FILE is -, read standard input.\n\
\n\
 -b, --number-nonblank    number nonempty output lines, overrides -n\n\
 -e                       equivalent to -vE\n\
 -E, --show-ends          display $ at end of each line\n\
 -n, --number             number all output lines\n\
 -s, --squeeze-blank      suppress repeated empty output lines\n\
 -t                       equivalent to -vT\n\
 -T, --show-tabs          display TAB characters as ^I\n\
 -v, --show-nonprinting   use ^ and M- notation, except for LFD and TAB\n\
     --help               display this help and exit\n\n\
Examples:\n\
  cat f - g  Output f's contents, then standard input, then g's contents.\n\
  cat        Copy standard input to standard output.\n\
\n\
Report bugs to: roninsha@student.21-school.ru\n\
GNU coreutils online help: <https://www.gnu.org/software/coreutils/>\n\
Full documentation <https://www.gnu.org/software/coreutils/cat>\n\
or available locally via: info '(coreutils) s21_cat invocation\'\n");
}