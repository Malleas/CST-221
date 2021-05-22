#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>

//Yasin, Awsa (01, February 2021) POSIX Spawn with C Programming.  Retrieved From: https://linuxhint.com/posix-spawn-c-programming/



extern char **environ;


int main(int argc, char const *argv[]) {
  pid_t pid;
  int status;
  status = posix_spawn(&pid, "/bin/sh", NULL, NULL, argv, environ);
  if(status == 0){
    printf("Child pid: %i\n", pid);
    if(waitpid(pid, &status, 0) != -1){
      printf("Child process exited with status: %i\n", status);
    }else{
      perror("waitpid");
    }

  }else {
    printf("posix_spawn: %i\n", strerror(status));
  }


  return 0;
}
