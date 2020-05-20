#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <dirent.h>

#define pidx	sqlite3_bind_parameter_index


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

void error(sqlite3 *db, char *msg)
{
  fprintf(stderr, "%s: %s\n", msg, sqlite3_errmsg(db));
  sqlite3_close(db);
  exit(0);
}

int bint(sqlite3_stmt *st, int pos, int value)
{
  if (sqlite3_bind_int(st, pos, value) != SQLITE_OK) {
    printf("Could not bind int at position %d\n", pos);
    return 1;
  }
  return 0;
}
int bdouble(sqlite3_stmt *st, int pos, double value)
{
  if (sqlite3_bind_double(st, pos, value) != SQLITE_OK) {
    printf("Could not bind int at position %d\n", pos);
    return 1;
  }
  return 0;
}

int btext(sqlite3_stmt *st, int pos, char *value)
{
  if (sqlite3_bind_text(st, pos, value, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
    printf("Could not bind int at position %d\n", pos);
    return 1;
  }
  return 0;
}

int main(void)
{
  int fail = 0;
  sqlite3 *db;
  sqlite3_stmt *st;

  sqlite3_open("pkgs.db", &db);

// Make the statement:
  sqlite3_prepare_v2(db, "drop table if exists files;", -1, &st, NULL);
// Execute it:
  sqlite3_step(st);
// Clean up the statement:
  sqlite3_finalize(st);

// Make the statement:
  sqlite3_prepare_v2(db, "drop table if exists packages;", -1, &st, NULL);
// Execute it:
  sqlite3_step(st);
// Clean up the statement:
  sqlite3_finalize(st);


// Make the statement:
  sqlite3_prepare_v2(db, "create table packages (pid integer primary key, fullname text, name text, version text, arch text, pkgversion integer, custom text, location text, description text, mdsum text, csize text, ucsize text);", -1, &st, NULL);
// Execute it:
  sqlite3_step(st);
// Clean up the statement:
  sqlite3_finalize(st);

// Make the statement:
  sqlite3_prepare_v2(db, "create table files (pid integer, path text, type text, mimetype text, mode integer, user text, uid integer, grou text, gid integer, size integer, mtime integer, mdsum integer);", -1, &st, NULL);
// Execute it:
  sqlite3_step(st);
// Clean up the statement:
  sqlite3_finalize(st);
  /*
  if (sqlite3_prepare_v2(db, "insert into X values (?, ?, ?, @x), (?, ?2, ?, @y)", -2, &st, NULL) != SQLITE_OK)
    error(db, "prepare");
  
  fail += bint(st, 1, 1);
  fail += bint(st, 5, 2);
  fail += bdouble(st, 2, 5.5);
  fail += btext(st, 3, "c1");
  fail += btext(st, 6, "c2");
  fail += btext(st, pidx(st, "@x"), "@x");
  fail += btext(st, pidx(st, "@y"), "@y");

  if (fail) error(db, "binding params");

  sqlite3_step(st);
  sqlite3_finalize(st);
  */
  struct dirent **files;
  int n;

  // Read all the files from /var/lib/pkgtools/packages in alpabetic order:
  n = scandir("/var/lib/pkgtools/packages", &files, filter, alphasort);
  if (n == -1) die("scandir");

  for(int i = 0; i < n; i++) {
    printf("%s\n", files[i]->d_name);
    free(files[i]);
  }
  free(files);

  sqlite3_close(db);
  return 0;
}