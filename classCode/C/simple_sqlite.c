#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

/**
 * To compile: gcc -o simple_sqlite simple_sqlite.c -lsqlite3
 */
int db_error(sqlite3 *db, char *msg)
{
  fprintf(stderr, "%s: %s\n", msg, sqlite3_errmsg(db));
  sqlite3_close(db);
  return 1;
}

int student_rows(sqlite3_stmt *res)
{
  int rc;
  while ((rc = sqlite3_step(res)) == SQLITE_ROW) {
    printf("RowID: %3ld, sID: %d, sName: %-10s, GPA: %.2f, sizeHS: %5d\n",
	   sqlite3_column_int(res, 0),
	   sqlite3_column_int(res, 1),
	   sqlite3_column_text(res, 2),
	   sqlite3_column_double(res, 3),
	   sqlite3_column_int(res, 4));
  }
  sqlite3_finalize(res);
  return (rc == SQLITE_ERROR);
}

int list_student_table(sqlite3 *db)
{
  sqlite3_stmt *res;
  int rc;
  
  rc = sqlite3_prepare_v2(db, "SELECT rowid, * FROM Student", -1, &res, NULL);
  if (rc != SQLITE_OK) return 1;
  return student_rows(res);
}

int query_student_table(sqlite3 *db, int sID)
{
  sqlite3_stmt *res;
  int rc;
  
  rc = sqlite3_prepare_v2(db, "SELECT rowid, * FROM Student WHERE sID = ?", -1, &res, NULL);
  if (rc != SQLITE_OK) return 1;
  // Parameters (the ?'s above) start at 1, not 0:
  // Bind sID to the first parameter.
  sqlite3_bind_int(res, 1, sID);
  return student_rows(res);
}

int insert_student(sqlite3 *db, int sID, char *sName, double GPA, int sizeHS)
{
  sqlite3_stmt *res;
  int rc;
  
  rc = sqlite3_prepare_v2(db, "INSERT INTO Student VALUES (?, ?, ?, ?)", -1, &res, NULL);
  if (rc != SQLITE_OK) return 1;
  // Parameters (the ?'s above) start at 1, not 0:
  // Bind sID to the first parameter.
  sqlite3_bind_int(res, 1, sID);
  sqlite3_bind_text(res, 2, sName, -1, SQLITE_TRANSIENT);
  sqlite3_bind_double(res, 3, GPA);
  sqlite3_bind_int(res, 4, sizeHS);

  if ((rc = sqlite3_step(res)) != SQLITE_DONE) {
    db_error(db, "Unable to insert data");
  }

  sqlite3_finalize(res);
  printf("Rowid = %ld\n", sqlite3_last_insert_rowid(db));
  
  return (rc != SQLITE_DONE);
}

/**
 * Usage: simple_sqlite <dbfile> [sID]
 */
int main(int argc, char *argv[])
{
  sqlite3 *db;
//  char *zErrMsg = 0;
  int rc;

  if (argc < 2) {
    printf("Usage: %s <dbfile> [sID]\n", argv[0]);
    return 0;
  }

  rc = sqlite3_open(argv[1], &db);
  if ( rc ) {
    return db_error(db, "Can't open database\n");
  } else {
    fprintf(stderr, "Opened database successfully.\n");
  }
  
  if (argc < 3) {
    if (list_student_table(db) != 0) return db_error(db, "list student table");
  } else {
    if (argc < 6) {
      if (query_student_table(db, atoi(argv[2])) != 0) return db_error(db,"query student id");
    } else {
      if (insert_student(db, atoi(argv[2]), argv[3], atof(argv[4]), atoi(argv[5])) != 0)
	return db_error(db,"insert student data");
    }
  }
  
  sqlite3_close(db);
}
