#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef int fd;                         //file descriptor is an int
typedef fd Pipe[2];                     //pipe is a pair of fd's as an array
fd Reader(Pipe p) {​​​​ return p[0]; }​​​​
fd Writer(Pipe p) {​​​​ return p[1]; }​​​​

void check(int ok, char *where) {​​​​
  if (ok < 0) {​​​​
    fprintf(stderr, "Error in %s: %s\n", where, strerror(errno));
    exit(1);
  }​​​​
}​​​​


int evaluate(char *buffer, int len) {​​​​
  fprintf(stderr, "> %s", buffer);

  int ok = printf("%s", buffer);
  if (ok < 0) {​​​​
    return -1;
  }​​​​
  char* p = fgets(buffer, len, stdin);
  if (p == NULL) {​​​​
    return -1;
  }​​​​
  fprintf(stderr, "< %s", buffer);
  return 0;
}​​​​


int main()
{​​​​
  int ok;                                 //for error testing
  char buffer[100];
  Pipe up, down;                          //declare twp Pipe variables up & down and
                                          //open with pipe program
  setbuf(stdout, NULL);                   //Create buffers from stdout/in
  setbuf(stdin, NULL);
  ok = evaluate(buffer, sizeof(buffer));
  check(ok, "I/O");

  ok = pipe(up);             check(ok, "opening pipe");
  ok = pipe(down);           check(ok, "opening pipe");

  //Forking process
  if (fork() == 0 ) {​​​​                      //child process
    ok = close(Writer(down)); check(ok, "closing down reader");
    ok = close(Reader(up)); check(ok, "closing down reader");
    ok = dup2(Reader(down), 0); check(ok, "redirecting stdin to the down reader");
    ok = dup2(Writer(up), 1); check(ok, "redirecting stdout to the up writer");
    execve("bc",NULL,NULL);
  }​​​​
  else {​​​​                                  //parent process
    ok = close(Reader(down)); check(ok, "closing down reader");
    ok = close(Writer(up)); check(ok, "closing up writer");
    ok = write(Writer(down), &buffer, sizeof(buffer)); check(ok, "writing to write down");
    ok = read(Reader(up), &buffer, sizeof(buffer)); check(ok, "reading to read up");
    for (int i = 3; i <= 10; i++) {​​​​
    sprintf(buffer + strlen(buffer) - 1, "+%u\n", i);
    ok = evaluate(buffer, sizeof(buffer));
    check(ok, "I/O");
  }​​​​
     fprintf(stdout, "The result is %s", buffer);
     ok = close(Reader(up)); check(ok, "closing down reader");
     ok = close(Writer(down)); check(ok, "closing up writer");
   }​​​​
  return 0;
}​​​​
