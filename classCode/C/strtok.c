/**
 * Read "man 3 strtok"
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXPATH		4096
#define MAXBUF		4096

void die(char *msg)
{
  perror(msg);
  exit(1);
}

int main(void)
{
  char path[MAXPATH], buf[MAXBUF];
  struct dirent **files;
  int line = 0;

  snprintf(path, MAXPATH-1, "/var/lib/pkgtools/packages/%s", "coreutils-8.31-x86_64-1");
  FILE *fp = fopen(path, "r");
  while (fgets(buf, MAXBUF, fp) != NULL) {
    // First time strtok is called, the first parameter is the string to be 'tokenized'.
    // Note: The string being tokenized is destructively modified:
    char *tag = strtok(buf, ":");
    // Subseqent calls to strtok, use NULL as the first parameter to continue tokenizing
    // the initial string.
    char *info = strtok(NULL, "\n");

    printf("%-5d: tag: %s, info: %s\n", ++line, tag, info);
    if (strcmp(tag, "FILE LIST") == 0) break;
  }

  fclose(fp);
  return 0;
}