#include "mkpkgdb.h"

void make_tables(sqlite3 *db)
{
  int rc;
  char *error = NULL;
  
  rc = sqlite3_exec(db, "drop table if exists packages;", NULL, NULL, &error);
  if (rc != 0 && error) db_error_die(db, error);
  rc = sqlite3_exec(db, "drop table if exists files;", NULL, NULL, &error);
  if (rc != 0 && error) db_error_die(db, error);
  rc = sqlite3_exec(db, 
    "create table if not exists packages (\n"
    "  pid		integer	primary key  autoincrement,\n"
    "  fullname		text,\n"
    "  name		text,\n"
    "  version		text,\n"
    "  arch		text,\n"
    "  pkgversion	integer,\n"
    "  custom		text,\n"
    "  location		text,\n"
    "  description	text,\n"
    "  pkgmdsum		text,\n"
    "  csize		text,\n"
    "  ucsize		text\n"
    ");\n", NULL, NULL, &error);
  if (rc != 0 && error) db_error_die(db, error);

  rc = sqlite3_exec(db, 
    "create table if not exists files (\n"
    "  pid	integer references packages ( pid ),\n"
    "  path	text,\n"
    "  type	text,\n"
    "  mimetype	text,\n"
    "  mode	integer,\n"
    "  user	text,\n"
    "  uid	integer,\n"
    "  `group`	text,\n"
    "  gid	integer,\n"
    "  size	integer,\n"
    "  mtime	timestamp,\n"
    "  mdsum	text\n"
    ");\n", NULL, NULL, &error);
  if (rc != 0 && error) db_error_die(db, error);

  return;
}
