#include <stdio.h>
#include <string.h>

int main(void)
{
  FILE *fp = popen("md5sum popen.c", "r");

  if (fp == NULL) {
    perror("popen");
    return 1;
  }
  char buf[4096];

  int line = 1;
  while (fgets(buf, 4096, fp) != NULL) {
    printf("%d: %s\n", line++, strtok(buf, " "));
  }

  pclose(fp);
  return 0;
}
