#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef int fd;
typedef fd Pipe[2];
fd Reader(Pipe p) { return p[0]; }
fd Writer(Pipe p) { return p[1]; }

char* command = "/usr/bin/bc";
char* argv[] = {"bc", NULL};
char* envp[] = {NULL};

void check(int ok, char *where) {
  if (ok < 0) {
    fprintf(stderr, "Error in %s: %s\n", where, strerror(errno));
    exit(1);
  }
}

int evaluate(char *buffer, int len, Pipe up, Pipe down)
{
  int ok;
  printf("> %s\n", buffer);
  do{
     ok = write(Writer(down), buffer, sizeof(buffer));
  } while(ok < 0 && errno ==EINTR);
  check(ok, "failed to write to bc");

  do{
     ok = read(Reader(up), buffer, sizeof(buffer));
  } while(ok < 0 && errno == EINTR);
  check(ok, "failed to read from bc");

  buffer[ok] = '\0'; printf("< %s", buffer);
  return 0;
}

int main() {
  int ok, id, i;
  Pipe up, down;
  char buffer[100];

  ok = pipe(up);             check(ok, "opening pipe up");
  ok = pipe(down);           check(ok, "opening pipe down");
  id = fork();               check(ok, "fork failed");

  if(id==0){
     /*child proces reads from down reader and writes to up writer, then execs bc*/
     ok = dup2(Reader(down), STDIN_FILENO);   check(ok, "redirecting child in to down reader");
     ok = dup2(Writer(up), STDOUT_FILENO);    check(ok, "redirecting child out to up writer");
     ok = close(Reader(up));    check(ok, "closing reader up");
     ok = close(Writer(down));  check(ok, "closing writer down");

     ok = execve(command, argv, envp); check(ok, "execcve failed");
 }

 /*parent process writes to down pipe and reads from up pipe*/
 ok = close(Reader(down));  check(ok, "closing reader down");
 /* here we can write to p */
 ok = close(Writer(up));    check(ok, "closing writer up");

 strcpy(buffer, "1+2\n");
 for(i = 3; i <= 10; i++){
    sprintf(buffer + strlen(buffer) - 1, "+%u\n", i);
    ok = evaluate(buffer, sizeof(buffer), up, down);
    check(ok, "I/O");
}
  ok = close(Reader(up));    check(ok, "closing reader up");
  ok = close(Writer(down));  check(ok, "closing writer down");
  printf("result: %s\n", buffer);

  return 0;
}
