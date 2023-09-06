#ifndef S21_GREP_H
#define S21_GREP_H
#define _GNU_SOURCE

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct config {
  short unsigned int e : 1;
  short unsigned int i : 1;
  short unsigned int v : 1;
  short unsigned int c : 1;
  short unsigned int l : 1;
  short unsigned int n : 1;
  short unsigned int h : 1;
  short unsigned int s : 1;
  short unsigned int f : 1;
  short unsigned int o : 1;
  short unsigned int help : 1;
} config;

typedef struct value_grep {
  int files_count;
  int error;
  char* pattern;
} value_grep;

void help_info();
void searcher(config* config, value_grep* value, char* filename);
void search_maches(char* line, char* pattern, int* match, int flag);
void print_flag_o(char* line, int flag, value_grep value, config config,
                  char* filename, int count_line);

void get_config_grep(config* config, value_grep* value, int argc, char** argv);
void regex_copy(value_grep* value, config config, char* regex);
void regex_copy_from_file(value_grep* value, config* config, char* filename);

#endif