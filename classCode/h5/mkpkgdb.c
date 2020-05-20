#include "mkpkgdb.h"

int insert_new_package(sqlite3 *db, char *package, char *name, char *version,
    char *arch, int pkgversion, char *custom, char *csize, char *ucsize,
    char *location, char *desc)
{
  sqlite3_stmt *res;
  int rc;

  rc = sqlite3_prepare_v2(db, "INSERT INTO packages "
    "(fullname, name, version, arch, pkgversion, custom, csize, ucsize, location, description) "
    "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", -1, &res, NULL);
  if (rc == SQLITE_OK) {
    sqlite3_bind_text(res, 1, package , -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(res, 2, name    , -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(res, 3, version , -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(res, 4, arch    , -1, SQLITE_TRANSIENT);
    sqlite3_bind_int (res, 5, pkgversion);
    sqlite3_bind_text(res, 6, custom  , -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(res, 7, csize   , -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(res, 8, ucsize  , -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(res, 9, location, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(res,10, desc    , -1, SQLITE_TRANSIENT);
    if ((rc = sqlite3_step(res)) != SQLITE_DONE) {
      if (rc == SQLITE_ERROR) db_error_die(db, "Failed to insert data for new package");
    }
    int pid = sqlite3_last_insert_rowid(db);
    sqlite3_finalize(res);
    return pid;
  }
  db_error_die(db, "Failed to prepare insert for new package");
  /* NOT REACHED */
  return -1;
}

int insert_file(sqlite3 *db, sqlite3_stmt *res, int pid, char *path, char *type,
		char *mime, int mode, char *user, int uid, char *group, int gid,
		int size, char *mtime, char *mdsum)
{
  int rc;
  sqlite3_clear_bindings(res);
  sqlite3_bind_int (res, 1, pid);
  sqlite3_bind_text(res, 2, path , -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(res, 3, type , -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(res, 4, mime , -1, SQLITE_TRANSIENT);
  sqlite3_bind_int (res, 5, mode);
  sqlite3_bind_text(res, 6, user , -1, SQLITE_TRANSIENT);
  sqlite3_bind_int (res, 7, uid);
  sqlite3_bind_text(res, 8, group, -1, SQLITE_TRANSIENT);
  sqlite3_bind_int (res, 9, gid);
  sqlite3_bind_int (res,10, size);
  sqlite3_bind_text(res,11, mtime, -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(res,12, mdsum, -1, SQLITE_TRANSIENT);
  
  if ((rc = sqlite3_step(res)) != SQLITE_DONE) {
    if (rc == SQLITE_ERROR) db_error_die(db, "Failed to insert file");
  }
  sqlite3_reset(res);
  return 0;
}


void process_file(sqlite3 *db, sqlite3_stmt *res, int pid, char *path,
		  int fn, int total)
{
  struct stat st;
  char *type = "?", *mime = "";
  char *usr = "", *grp = "", *mtime = "0000-00-00 00:00:00";
  char *mdsum = "";
  
  if (fn % 10 == 0) fprintf(stderr, "\r%4d/%4d", fn, total);
  if (stat(path, &st) < 0) memset(&st, 0, sizeof(st));
  else {
    type = filetype(st.st_mode);
    usr = username(st.st_uid);
    grp = group(st.st_gid);
    if (type[0] == '-') {
      mdsum = md5sum(path);
      mime = mimetype(path);
    }
    mtime = timestamp(st.st_mtime);
  }
  insert_file(db, res, pid, path, type, mime, st.st_mode & 07777, usr,
	      st.st_uid, grp, st.st_gid, st.st_size, mtime, mdsum);
}


void process_files(sqlite3 *db, int pid, char **line, int fstart, int lines)
{
  sqlite3_stmt *res;
  char path[MAXPATH];
  int i, rc;
  
  rc = sqlite3_prepare_v2(db, "INSERT INTO files "
    "(pid, path, type, mimetype, mode, user, uid, `group`, gid, size, mtime, mdsum) "
    "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", -1, &res, NULL);
  if (rc != SQLITE_OK) db_error_die(db, "Failed to prepare insert for files");
  
  for(i=fstart; line[i] != NULL; i++) {
    strcpy(path, ROOT);
    strcat(path, line[i]);
    char *f = strrchr(path, '\n');
    if (f != NULL) *f = '\0';
    if (strcmp(path, "/./") == 0 || strncmp(path, "/install/", 9) == 0) continue;
    process_file(db, res, pid, path, i-fstart, lines-fstart);
  }
  fprintf(stderr, "\r");
  
  sqlite3_finalize(res);
}


void parse_pkgname(char *package, char **name, char **version, char **arch, 
		   int *pkgversion, char **custom)
{
  static char s[1024];
  char *p, *w[20];
  int i = 0;
  
  strcpy(s, package);
  
  for(p = strtok(s, "-"); p != NULL; p = strtok(NULL, "-")) {
    w[i++] = p;
  }
  i--;
  *pkgversion = strtol(w[i--], &p, 10);
  *custom = (p != NULL)? p : "";
  *arch = w[i--];
  *version = w[i--];
  while(i) {
    *(w[i--]-1) = '-';
  }
  *name = s;
}
    
void process_package(sqlite3 *db, char *package)
{
  char buf[BUFSIZE];
  char *name, *version, *arch, *custom, *csize, *ucsize, *location, *desc;
  int pid, pkgversion, fstart, lines;

  printf("Processing: %s\n", package);
  
  parse_pkgname(package, &name, &version, &arch, &pkgversion, &custom);
  sprintf(buf, "/var/lib/pkgtools/packages/%s", package);
  char **line = readfile(buf, &lines);
  
  csize = get(line[1]);
  ucsize = get(line[2]);
  location = get(line[3]);
  desc = getdesc(line, &fstart);
  
  pid = insert_new_package(db, package, name, version, arch, pkgversion,
			   custom, csize, ucsize, location, desc);
  if (pid < 0) return;
  process_files(db, pid, line, fstart, lines);
  
  freefile(line);
}

int dotfilter(const struct dirent *de)
{
  return de->d_name[0] != '.';
}

void load_packages(sqlite3 *db)
{
  struct dirent **files;
  int i, n;

  if ((n = scandir("/var/lib/pkgtools/packages", &files, dotfilter, alphasort)) == -1)
    pdie("load_packages: scandir");
  
  for(i=0; i < n; i++) {
    process_package(db, files[i]->d_name);
    free(files[i]);
  }
  free(files);
}

int main(void)
{
  sqlite3 *db;
  int rc;
  
  if ((rc = sqlite3_open("pkg.db", &db))) {
    return db_error(db, "Can't open database");
  }

  make_tables(db);
  load_packages(db);

  sqlite3_close(db);
}
