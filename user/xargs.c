#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"
#include "user/user.h"

void fmt_params(char* line, char** params, int* cnt){
  char cur_param[MAXARG];
  int cur_param_len = 0;
  for(int i=0; i<strlen(line); i++){
    if((line[i] == ' ' || line[i] == '\n')&&(cur_param_len != 0)){
      params[*cnt] = malloc(MAXARG);
      memmove(params[*cnt], cur_param, cur_param_len);
      params[*cnt][cur_param_len] = 0;
      cur_param_len = 0;
      (*cnt)++;
    }else{
      cur_param[cur_param_len] = line[i];
      cur_param_len++;
    }
  }
}

int main(int argc, char *argv[]){
  if(argc == 1)exit(0);
  char* params[MAXARG];
  int cnt = 0;
  for(int i=1; i<argc; i++){
    params[i-1] = argv[i];
    cnt++;
  }
  char r_buf[MAXARG];
  while(read(0,r_buf,MAXARG) > 0){
    if(fork()==0){
      fmt_params(r_buf, params, &cnt);
      exec(params[0], params);
    }else{
      wait(0);
    }
  }
  exit(0);
}