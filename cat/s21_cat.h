#ifndef S21_CAT_H
#define S21_CAT_H

typedef struct config {
  short unsigned int b : 1;
  short unsigned int e : 1;
  short unsigned int t : 1;
  short unsigned int v : 1;
  short unsigned int n : 1;
  short unsigned int s : 1;
  short unsigned int help : 1;
} config;

typedef struct value {
  int count_line;
  int count_s;
  int end_file_flag;
} value;

void get_config_cat(config* config, int argc, char** argv);
void input_stream_handling(config* config, char* filename, value* value);
void char_print(char symbols, char* prev_symbol, config* flags, value* value);
void v_print(unsigned char symbols);
void help_info();

#endif