#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <sqlite3.h>
#include <time.h>

#define ROOT	"/"
#define BUFSIZE	4096
#define MAXPATH	4096
#define MAXDESC	65536

// util.c:
int db_error(sqlite3 *db, char *msg);
int db_error_die(sqlite3 *db, char *msg);
void pdie(char *s);
char *mimetype(char *path);
char *md5sum(char *path);
char *timestamp(time_t t);
char *filetype(mode_t mode);
char *group(gid_t gid);
char *username(uid_t uid);
char **readfile(char *path, int *nlines);
void freefile(char **data);
char *getdesc(char **lines, int *end);
char *get(char *s);

// setup.c:
void make_tables(sqlite3 *db);
