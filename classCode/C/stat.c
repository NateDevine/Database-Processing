/**
 * man 2 stat
 * man 7 inode (for mode information)
 * man 3 getpwuid getgrgid (for uid/gid to name conversion)
 */

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define K	1024

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

char filetype(mode_t mode)
{
  switch(mode & S_IFMT) {
    case S_IFSOCK: return 's';
    case S_IFLNK:  return 'l';
    case S_IFREG:  return '-';
    case S_IFBLK:  return 'b';
    case S_IFDIR:  return 'd';
    case S_IFCHR:  return 'c';
    case S_IFIFO:  return 'p';
    default: return '?';
  }
  /* NOT REACHED */
  return '?';
}

char *perm(mode_t mode, char *buf)
{
  buf[0] = mode & 04 ? 'r' : '-';
  buf[1] = mode & 02 ? 'w' : '-';
  buf[2] = mode & 01 ? 'x' : '-';
  buf[3] = '\0';
  return buf;
}

/**
 * Usage: stat <filename>
 */
int main(int argc, char *argv[])
{
  struct stat st;
  char u[4], g[4], o[4], tbuf[K];

  if (argc < 2) {
    printf("Usage: stat <filename>\n");
    return 0;
  }
  
  if (stat(argv[1], &st) < 0) {
    perror("stat");
    return 1;
  }
  
  printf("Filename: %s\n", argv[1]);
  printf("UID: %d (%s), GID: %d (%s)\n", st.st_uid, username(st.st_uid), st.st_gid, group(st.st_gid));
  printf("Filetype: %c Perms: %s%s%s\n", filetype(st.st_mode),
	 perm((st.st_mode & 0700) >> 6, u),
	 perm((st.st_mode & 0070) >> 3, g),
	 perm((st.st_mode & 0007),o));
  struct tm *tm = localtime(&(st.st_mtime));
  strftime(tbuf, K, "%Y-%m-%d %I:%M:%S %p", tm);
  printf("mtime: %s\n", tbuf);
  
  return 0;
}