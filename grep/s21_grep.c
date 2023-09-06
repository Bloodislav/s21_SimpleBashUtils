#include "s21_grep.h"

int main(int argc, char** argv) {
  config config = {0};
  value_grep value = {0};

  if (argc < 2) {
    fprintf(stderr, "Usage: ./s21_grep [OPTION]... PATTERNS [FILE]...\n");
    return 1;
  }

  get_config_grep(&config, &value, argc, argv);

  if (!config.e && !config.f) {
    regex_copy(&value, config, *(argv + optind++));
  }

  value.files_count = argc - optind;

  if (config.help) {
    help_info();
  } else {
    while (optind < argc) {
      searcher(&config, &value, *(argv + optind));
      optind += 1;
    }
  }

  free(value.pattern);
  return 0;
}

void get_config_grep(config* config, value_grep* value, int argc, char** argv) {
  int option, option_index;
  const char* short_options = "e:ivclnhsf:o";
  const struct option long_options[] = {
      {"regexp", required_argument, NULL, 'e'},
      {"ignore-case", no_argument, NULL, 'i'},
      {"invert-match", no_argument, NULL, 'v'},
      {"count", no_argument, NULL, 'c'},
      {"files-with-matches", no_argument, NULL, 'l'},
      {"line-number", no_argument, NULL, 'n'},
      {"no-filename", no_argument, NULL, 'h'},
      {"no-messages", no_argument, NULL, 's'},
      {"only-matching", no_argument, NULL, 'o'},
      {"file", required_argument, NULL, 'f'},
      {"help", no_argument, NULL, 0},
      {NULL, 0, NULL, 0}};

  while ((option = getopt_long(argc, argv, short_options, long_options,
                               &option_index)) != -1) {
    if (option == 'e') {
      regex_copy(value, *config, optarg);
      config->e = 1;
    }
    if (option == 'i') {
      config->i = 1;
    }
    if (option == 'v') {
      config->v = 1;
    }
    if (option == 'c') {
      config->c = 1;
    }
    if (option == 'l') {
      config->l = 1;
    }
    if (option == 'n') {
      config->n = 1;
    }
    if (option == 'h') {
      config->h = 1;
    }
    if (option == 's') {
      config->s = 1;
    }
    if (option == 'f') {
      regex_copy_from_file(value, config, optarg);
    }
    if (option == 'o') {
      config->o = 1;
    }
    if (option == 0) {
      config->help = 1;
    }
    if (option == '?') {
      fprintf(stderr, "Try './s21_grep --help' for more information.\n");
      exit(EXIT_FAILURE);
    }
  }

  if (config->l) config->c = config->o = 0;
  if (config->c) config->o = 0;
  if (config->v && config->o) value->error = 1;
}

void regex_copy(value_grep* value, config config, char* regex) {
  int size = 0;

  if (!config.e && !config.f) {
    value->pattern = malloc(sizeof(char));
    if (value->pattern != NULL) *(value->pattern) = '\0';
    size = strlen(regex) + strlen(value->pattern) + 1;
    value->pattern = realloc(value->pattern, sizeof(char) * size);
    strcat(value->pattern, regex);
  } else {
    size = strlen(regex) + strlen(value->pattern) + 3;
    value->pattern = realloc(value->pattern, sizeof(char) * size);

    *(value->pattern + size - 3) = '\0';
    strcat(value->pattern, "\\|");
    *(value->pattern + size - 1) = '\0';
    strcat(value->pattern, regex);
  }
}

void regex_copy_from_file(value_grep* value, config* config, char* filename) {
  FILE* file = NULL;
  char* line = NULL;

  if (!access(filename, R_OK))
    file = fopen(filename, "r");
  else if (!config->s) {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", filename);
    exit(EXIT_FAILURE);
  }

  if (file) {
    int len = 0;
    size_t size = 0;

    while ((len = getline(&line, &size, file)) != -1) {
      if (*(line + len - 1) == '\n') *(line + len - 1) = '\0';
      regex_copy(value, *config, line);
      config->f = 1;
    }

    if (line) free(line);
    fclose(file);
  }
}

void searcher(config* config, value_grep* value, char* filename) {
  FILE* file = NULL;
  int flag = REG_NEWLINE;

  if (config->i) {
    flag |= REG_ICASE;
  }
  if (config->e || config->f || config->o) {
    flag |= REG_EXTENDED;
  }

  if (!access(filename, R_OK))
    file = fopen(filename, "r");
  else if (!config->s) {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", filename);
    return;
  }

  if (file) {
    int count_line = 0;
    int count_match_lines = 0;

    int len = 0;
    char* line = NULL;
    size_t size = 0;
    char last_char;

    while ((len = getline(&line, &size, file)) != -1) {
      last_char = *(line + strlen(line) - 1);
      count_line++;

      int match;
      search_maches(line, value->pattern, &match, flag);

      if (config->v) match = !match;

      if (match) count_match_lines += 1;

      if (match && config->l && !value->error) {
        printf("%s\n", filename);
        break;
      }

      if (config->o && !config->c && !config->l && !value->error) {
        print_flag_o(line, flag, *value, *config, filename, count_line);
      }

      if (match && !config->c && !value->error && !config->o) {
        if (value->files_count > 1 && !config->h) printf("%s:", filename);
        if (config->n) printf("%d:", count_line);
        printf("%s", line);
        if (last_char != '\n') printf("\n");
      }
    }

    if (config->c && !config->l) {
      if (value->files_count > 1 && !config->h) printf("%s:", filename);
      printf("%d", count_match_lines);
      if ((last_char != '\n') || len == -1) printf("\n");
    }

    if (line) free(line);
    fclose(file);
  }
}

void search_maches(char* line, char* pattern, int* match, int flag) {
  regex_t reg;
  *match = regcomp(&reg, pattern, flag);
  if (!(*match)) *match = !regexec(&reg, line, 0, NULL, 0);
  regfree(&reg);
}

void print_flag_o(char* line, int flag, value_grep value, config config,
                  char* filename, int count_line) {
  int status;
  regex_t reg;
  regcomp(&reg, value.pattern, flag);

  char* str = line;
  regmatch_t pmatch[1];
  while ((status = regexec(&reg, str, 1, pmatch, 0)) == 0) {
    if (value.files_count > 1 && !config.h) printf("%s:", filename);
    if (config.n) printf("%d:", count_line);

    printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
           str + pmatch[0].rm_so);
    str += pmatch[0].rm_eo;
  }
  regfree(&reg);
}

void help_info() {
  printf(
      "Usage: grep [OPTION]... PATTERNS [FILE]...\n\
Search for PATTERNS in each FILE.\n\
Example: grep -e 'hello world' menu.h main.c\n\
PATTERNS can contain multiple patterns separated by newlines.\n\
\n\
Pattern selection and interpretation:\n\
  -e, --regexp=PATTERNS     use PATTERNS for matching\n\
  -f, --file=FILE           take PATTERNS from FILE\n\
  -i, --ignore-case         ignore case distinctions in patterns and data\n\
\n\
Miscellaneous:\n\
  -s, --no-messages         suppress error messages\n\
  -v, --invert-match        select non-matching lines\n\
\n\
Output control:\n\
  -n, --line-number         print line number with output lines\n\
  -h, --no-filename         suppress the file name prefix on output\n\
  -o, --only-matching       show only nonempty parts of lines that match\n\
  -l, --files-with-matches  print only names of FILEs with selected lines\n\
  -c, --count               print only a count of selected lines per FILE\n\
\n\
Report bugs to: roninsha@student.21-school.ru\n\
GNU grep home page: <https://www.gnu.org/software/grep/>\n\
General help using GNU software: <https://www.gnu.org/gethelp/>\n");
}