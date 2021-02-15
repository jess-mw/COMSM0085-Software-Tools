#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef int fd;
typedef fd Pipe[2];
fd Reader(Pipe p) { return p[0]; }
fd Writer(Pipe p) { return p[1]; }

char* command = "/bin/bc";
char* argv[] = {"echo", "Hello", NULL};
char* envp[] = {NULL};

void check(int ok, char *where) {
  if (ok < 0) {
    fprintf(stderr, "Error in %s: %s\n", where, strerror(errno));
    exit(1);
  }
}

int main() {
  int ok, id;
  Pipe up, down;

  ok = pipe(up);             check(ok, "opening pipe up");
  ok = pipe(down);           check(ok, "opening pipe down");
  id = fork();

  if(id==0){
     /*child proces reads from down reader and writes to up writer, then execs bc*/
     ok = close(Reader(up));    check(ok, "closing reader up");
     ok = close(Writer(down));  check(ok, "closing writer down");
     ok = execve(command, argv, envp);
 }

 /*parent process writes to down pipe and reads from up pipe*/
 ok = close(Reader(down));  check(ok, "closing reader down");
 /* here we can write to p */
 ok = close(Writer(up));    check(ok, "closing writer up");


  return 0;
}
