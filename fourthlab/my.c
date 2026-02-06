#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void    checkArg(const char *a)
{
  while (*a)
    {
      if (   (*a == ';')
          || (*a == '&')
          || (*a == '|')
          || (*a == ',')
          || (*a == '$')
          || (*a == '(')
          || (*a == ')')
          || (*a == '{')
          || (*a == '}')
          || (*a == '`')
          || (*a == '>')
          || (*a == '<') ) {
        puts("Forbidden !!!");
        return;
      }
        a++;
    }
}

int     main()
{
  char  *arg = malloc(0x20);
  char  *cmd = malloc(0x400);

  strcpy(cmd, "/bin/ls -l ");

  printf("Enter directory you want to display : ");

  if (!fgets(arg, 20, stdin)) {
    fprintf(stderr, "Input error\n");
    free(arg);
    free(cmd);
    return 1;
  }

  checkArg(arg);

  int n = snprintf(cmd, 400, "/bin/ls -la %s", arg);
  if (n < 0 || n >= 400) {
    fprintf(stderr, "Command too long\n");
    free(arg);
    free(cmd);
    return 1;
  }

  system(cmd);

  free(arg);
  free(cmd);

  return 0;
}