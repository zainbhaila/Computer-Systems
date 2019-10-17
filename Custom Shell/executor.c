/*Zain Bhaila
115197541
zainb*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "command.h"
#include "executor.h"

#define SUCCESS 0
#define FAILURE 1
#define FILE_OPTIONS 0664

/*static void print_tree(struct tree *t);*/
static int check_status(int status);
static int file_redirect(char *input, char *output);

int execute(struct tree *t) {
   /*testing*/
   /*print_tree(t);
   printf("\n\n");*/

   /*execute commands*/
   if (t->conjunction == NONE) {
      /*cd command*/
      if (!strcmp(t->argv[0], "cd")) {
	 if (t->argv[1] == NULL) {
	    /*no arguments, change to home dir*/
	    if (chdir(getenv("HOME")) == -1) {
	       perror("cd");
	       return FAILURE;
	    }
	 }
	 else {
	    /*with directory argument*/
	    if (chdir(t->argv[1]) == -1) {
	       perror(t->argv[1]);
	       return FAILURE;
	    }
	 }
      }
      /*exit command*/
      else if (!strcmp(t->argv[0], "exit")) {
	 exit(0);
      }
      /*unix commands*/
      else {
	 int pid = fork();
	 int status = FAILURE;

	 if (pid == -1){
	    perror("fork");
	    return FAILURE;
	 }
	 else if (!pid) {
	    int ret_val = SUCCESS;

	    /*input and output redirection*/
	    if (file_redirect(t->input, t->output) == FAILURE) {
	       return FAILURE;
	    }

	    /*execute the command*/
	    if (execvp(t->argv[0], t->argv) == -1) {
	       fprintf(stderr, "Failed to execute %s\n", t->argv[0]);      
	       ret_val = FAILURE;
	    }

	    exit(ret_val);
	 }
	 else {
	    wait(&status);
	 }

	 return check_status(status);
      }
   }
   /*and conjunstion*/
   else if (t->conjunction == AND) {
      if (execute(t->left) != SUCCESS || execute(t->right) != SUCCESS) {
	 return FAILURE;
      }
   }
   /*pipe conjuntion*/
   else if (t->conjunction == PIPE) {
      int pipe_fd[2];
      int pid;
      
      /*ambiguous inputs and outputs using pipes*/
      if (t->left->output != NULL) {
	 printf("Ambiguous output redirect.\n");
	 return FAILURE;
      }
      if (t->right->input != NULL) {
	 printf("Ambiguous input redirect.\n");
	 return FAILURE;
      }

      pipe(pipe_fd);
      if ((pid = fork()) == -1) {
	 perror("fork");
	 return FAILURE;
      }
      /*pipe write end*/
      else if (!pid) {
	 /*close read end*/
	 if (close(pipe_fd[0]) == -1){
	    perror("close");
	    return FAILURE;
	 }

	 /*redirect output to pipe*/
	 if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
	    perror("dup2");
	    return FAILURE;
	 }      
	 if (close(pipe_fd[1]) == -1) {
	    perror("close");
	    return FAILURE;
	 }

	 exit(execute(t->left));
      }
      else {
	 int pid2 = fork();
	 if (pid2 == -1) {
	    perror("fork");
	    return FAILURE;
	 }
	 /*pipe read end*/
	 else if (!pid2) {
	    /*close write end*/
	    if (close(pipe_fd[1]) == -1){
	       perror("close");
	       return FAILURE;
	    }

	    /*redirect input from pipe*/
	    if (dup2(pipe_fd[0], STDIN_FILENO) == -1) {
	       perror("dup2");
	       return FAILURE;
	    }      
	    if (close(pipe_fd[0]) == -1) {
	       perror("close");
	       return FAILURE;
	    }

	    exit(execute(t->right));
	 }
	 else {
	    int status1 = FAILURE, status2 = FAILURE;
	    
	    /*close pipe for parent*/
	    if (close(pipe_fd[0]) == -1){
	       perror("close");
	       return FAILURE;
	    }
	    if (close(pipe_fd[1]) == -1){
	       perror("close");
	       return FAILURE;
	    }

	    /*reap children*/
	    wait(&status1);
	    wait(&status2);

	    if (check_status(status1) == SUCCESS) {
	       return check_status(status2);
	    }
	    else {
	       return FAILURE;
	    }
	 }
      }
   }
   /*subshell conjuntion*/
   else if (t->conjunction == SUBSHELL) {
      int pid = fork();
      int status = FAILURE;

       if (pid == -1){
	  perror("fork");
	  return FAILURE;
       }
       else if (!pid) {
	  /*input and output redirection*/
	  if (file_redirect(t->input, t->output) == FAILURE) {
	     return FAILURE;
	  }
	  
	  exit(execute(t->left));
       }
       else {
	  wait(&status);
       }

       return check_status(status);
   }

   return SUCCESS;
}

/*check the exit status of child process*/
static int check_status(int status) {
   if (WIFEXITED(status)) {
      return WEXITSTATUS(status);
   }
   else {
      return FAILURE;
   }
}

/*redirect file input and outpt*/
static int file_redirect(char *input, char *output) {
   if (input != NULL) {
      int fdi = open(input, O_RDONLY);
      if (fdi == -1) {
	 perror("open");
	 return FAILURE;
      }
      
      if (dup2(fdi, STDIN_FILENO) == -1) {
	 perror("dup2");
	 return FAILURE;
      }      
      if (close(fdi) == -1) {
	 perror("close");
	 return FAILURE;
      }
   }
   if (output != NULL) {
      int fdo = open(output, O_CREAT | O_WRONLY, FILE_OPTIONS);
      if (fdo == -1) {
	 perror("open");
	 return FAILURE;
      }
      
      if (dup2(fdo, STDOUT_FILENO) == -1) {
	 perror("dup2");
	 return FAILURE;
      }      
      if (close(fdo) == -1) {
	 perror("close");
	 return FAILURE;
      }
   }

   return SUCCESS;
}

/*static void print_tree(struct tree *t) {
   if (t != NULL) {
      print_tree(t->left);

      if (t->conjunction == NONE) {
         printf("NONE: %s,%s ", t->argv[0], t->argv[1]);
      } else {
         printf("%s, ", conj[t->conjunction]);
      }
      printf("IR: %s, ", t->input);
      printf("OR: %s\n", t->output);

      print_tree(t->right);
   }
}
*/
