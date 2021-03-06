/*--------------------------------------------------------------------

File: dp.c
Student name: Felix Singerman 
Student id: 7970742

Description:  Double pipe program.  To pipe the output from the standard
          output to the standard input of two other processes.
          Usage:  dp  <cmd1> : <cmd2> : <cmd3
          Output from process created with cmd1 is piped to
          processes created with cmd2 and cmd3

-------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
/* prototypes */
int doublePipe(char **,char **,char **);

/*--------------------------------------------------------------------

File: dp.c

Description: Main will parse the command line arguments into three arrays
         of strings one for each command to execv().
--------------------------------------------------------------------*/

int main(int argc, char *argv[])
{

   int i,j;         /*indexes into arrays */
   char *cmd1[10];  /*array for arguments of first command */
   char *cmd2[10];  /*array for arguments of second command */
   char *cmd3[10];  /*array for arguments of third command */
   if(argc == 1)
   {
     printf("Usage: dp <cmd1 arg...> : <cmd2 arg...> : <cmd3 arg....>\n");
     exit(1);
   }

   /* get the first command */
   for(i=1,j=0 ; i<argc ; i++,j++)
   {
      if(!strcmp(argv[i],":")) break; /* found first command */
      cmd1[j]=argv[i];
   }
   cmd1[j]=NULL;
   if(i == argc) /* missing : */
   {
      printf("Bad command syntax - only one command found\n");
      exit(1);
   }
   else i++;

   /* get the second command */
   for(j=0 ; i<argc ; i++,j++)
   {
      if(!strcmp(argv[i],":")) break; /* found second command */
      cmd2[j]=argv[i];
   }
   cmd2[j]=NULL;
   if(i == argc) /* missing : */
   {
      printf("Bad command syntax - only two commands found\n");
      exit(1);
   }
   else i++;

   /* get the third command */
   for(j=0 ; i<argc ; i++,j++) cmd3[j]=argv[i];
   cmd3[j]=NULL;
   if(j==0) /* no command after last : */
   {
      printf("Bad command syntax - missing third command\n");
      exit(1);
   }

   exit(doublePipe(cmd1,cmd2,cmd3));
}

/*--------------------------------------------------------------------------
  ----------------- You have to implement this function --------------------
  --------------------------------------------------------------------------
Function: doublePipe()

Description:  Starts three processes, one for each of cmd1, cmd2, and cmd3.
          The parent process will receive the output from cmd1 and
          copy the output to the other two processes.
-------------------------------------------------------------------------*/

int doublePipe(char **cmd1, char **cmd2, char **cmd3)
{

  pid_t pid;

  int p1to2fd[2];  /* pipe from prc1 to prc2 */
  int p1to3fd[2];  /* pipe from prc1 to prc3 */

  /* create the pipes */
  pipe(p1to2fd);
  pipe(p1to3fd);

  /* create process 1  */
  pid = fork();
  if(pid == 0){ /* child  process */

    dup2(p1to2fd[1], 1); //write to 2
    close(p1to2fd[0]);
    close(p1to2fd[1]);
    close(p1to3fd[0]);
    close(p1to3fd[1]);

    execvp(*cmd1, cmd1);

  }
  pid = fork();
  if(pid == 0){ /* child  process */
    dup2(p1to3fd[1], 1); // write to 3
    close(p1to2fd[0]);
    close(p1to2fd[1]);
    close(p1to3fd[0]);
    close(p1to3fd[1]);

    execvp(*cmd1, cmd1);

  }
  else{

    /* create process 2 */
    pid = fork();
    if(pid == 0){
      dup2(p1to2fd[0], 0); // read
      close(p1to2fd[0]);
      close(p1to2fd[1]);
      close(p1to3fd[0]);
      close(p1to3fd[1]);
      execvp(*cmd2, cmd2);

    }
    /* create process 3 */
    pid = fork();
    if(pid == 0){
      dup2(p1to3fd[0], 0); // read
      close(p1to2fd[0]);
      close(p1to2fd[1]);
      close(p1to3fd[0]);
      close(p1to3fd[1]);
      execvp(*cmd3, cmd3);

    } 

  }
  // only the parent gets here and waits for 3 children to finish

  close(p1to2fd[0]);
  close(p1to2fd[1]);
  close(p1to3fd[0]);
  close(p1to3fd[1]); 

  for (int i = 0; i < 3; i++)
    wait(NULL);
  

  return 0;

}
