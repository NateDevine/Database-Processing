#include "mkpkgdb.h"

int db_error(sqlite3 *db, char *msg)
{
  fprintf(stderr,"%s: %s\n", msg, sqlite3_errmsg(db));
  return 1;
}

int db_error_die(sqlite3 *db, char *msg)
{
  exit(db_error(db, msg));
}

void pdie(char *s)
{
  perror(s);
  exit(1);
}

char *mimetype(char *path)
{
  static char buf[4096];
  FILE *fp;
  char *p;
  sprintf(buf, "file -i -b '%s'", path);
  if ((fp = popen(buf, "r")) == NULL) {
    perror("popen");
    return "";
  }
  fgets(buf, 4096, fp);
  if ((p = strrchr(buf, '\n'))) *p = '\0';
  pclose(fp);
  return buf;
}


char *md5sum(char *path)
{
  static char buf[4096];
  FILE *fp;
  char *p;
  sprintf(buf, "md5sum -z '%s'", path);
  if ((fp = popen(buf, "r")) == NULL) {
    perror("popen");
    return "";
  }
  fgets(buf, 4096, fp);
  if ((p = strchr(buf, ' '))) *p = '\0';
  pclose(fp);
  return buf;  
}

char *timestamp(time_t t)
{
  static char tbuf[30];
  struct tm *tm = localtime(&t);
  strptime(tbuf, "%Y-%m-%d %H:%M:%S", tm);
  return tbuf;
}

char *filetype(mode_t mode)
{
  switch(mode & S_IFMT) {
    case S_IFSOCK: return "s";
    case S_IFLNK:  return "l";
    case S_IFREG:  return "-";
    case S_IFBLK:  return "b";
    case S_IFDIR:  return "d";
    case S_IFCHR:  return "c";
    case S_IFIFO:  return "p";
    default:       return "?";
  }
  /* NOT REACHED */
  return "?";
}

char *username(uid_t uid)
{
  struct passwd *pw = getpwuid(uid);
  if (pw == NULL) return "";
  return pw->pw_name;
}

char *group(gid_t gid)
{
  struct group *gr = getgrgid(gid);
  if (gr == NULL) return "";
  return gr->gr_name;
}

char **readfile(char *path, int *nlines)
{
  char buf[BUFSIZE];
  int n = 0, max;
  char **lines = malloc(sizeof(char *) * (max = 100));
  FILE *fp = fopen(path, "r");
  if (fp == NULL) pdie("fopen");
  while(fgets(buf, BUFSIZE, fp) != NULL) {
    if (n >= max-2) lines = realloc(lines, sizeof(char *) * (max += 100));
    lines[n++] = strdup(buf);
  }
  lines[n] = NULL;
  *nlines = n;
  fclose(fp);
  
  return lines;
}

void freefile(char **data)
{
  for(int i=0; data[i] != NULL; i++) free(data[i]);
  free(data);
}

/**
 * remove all whitespace from both the beginning and end of a string:
 */
char *trim(char *s)
{
  int l = strlen(s)-1;
  while(l && isspace(s[l])) s[l--] = '\0';
  while(isspace(*s)) s++;
  return s;
}

char *getdesc(char **lines, int *end)
{
  static char desc[MAXDESC];
  int i;
  
  desc[0] = '\0';
  for(i=5; strncmp(lines[i], "FILE LIST:", 10) != 0; i++) {
    strtok(lines[i], ":");
    char *w = strtok(NULL, "");
    while (*w == ' ') w++;
    strcat(desc, w);
  }
  *end = i+1;
  return desc;
}

char *get(char *s)
{
  strtok(s, ":");
  char *sw = strtok(NULL, "\n");
  return trim(sw);
}

