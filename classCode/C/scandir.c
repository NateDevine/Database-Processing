/**
 * Read "man 3 scandir", code taken almost entirely from the manual page:
 */
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sqlite3.h>

void die(char *msg)
{
  perror(msg);
  exit(1);
}

// Filter out all files beginning with a dot (.):
int filter(const struct dirent *de)
{
  return !(de->d_name[0] == '.');
}

int main(void)
{
  struct dirent **files;
  int n;

  // Read all the files from /var/lib/pkgtools/packages in alpabetic order:
  n = scandir("/var/lib/pkgtools/packages/", &files, filter, alphasort);
  if (n == -1) die("scandir");

  for(int i = 0; i < n; i++) {
    printf("%s\n", files[i]->d_name);
    free(files[i]);
  }
  free(files);

  return 0;
}