    #include <stdio.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/wait.h>
    #define MAX_LINE 80 /* The maximum length command */

/* parses basic arguments */
    int parse_str(char str[], char *args[]){
      char *p = NULL;
      int i = 0;

      p = strtok(str, " \t\n");

      while (p != NULL) {
	        args[i++] = p;
        p = strtok(NULL, " \t\n");
      }
      return i;
    }

/* parses multiple arguments */
    int parse_mult_cmds(char str[], char *multcmds[]){
      char *p = NULL;
      int i = 0;

      p = strtok(str, ";\n");

      while (p != NULL) {    
        multcmds[i++] = p;
        p = strtok(NULL, ";\n");
      }
      return i;
    }

/* parses piped arguments */
    int parse_pipe(char str[], char *args[]){
      char *p = NULL;
      int i = 0;

      p = strtok(str, "|");

      while (p != NULL) {
        args[i++] = p;
        p = strtok(NULL, "|");
      }
      return i;
    }

/* prints history */
    void print_history(char history[10][MAX_LINE], int size_history){
      if(size_history < 10){
	for(int i = 0; i < 10; i++){

	  if(size_history == i){
	    break;
	  }
	  else if(size_history < 10){
	    printf("%d %s", size_history - i, history[size_history  - i - 1]);
	  }
	}
      }
      else{
	 int j = 0;
	for(int i = 9; i >= 0; i--){
	  printf("%d %s", size_history - j, history[i]);
	    j++;
	}
      }
    }

/* adds arguments into history and increases number of arguments added */
    void add_to_history(char str[], char history[][MAX_LINE], int *size_history){

      if(strcmp(str, "\n") != 0){    
        if(*size_history < 10){
          strcpy(history[*size_history], str);
	  }

        else{
	  
          for(int k = 1; k < 10; k ++){
        strcpy(history[k - 1], history[k]);
          } 
          strcpy(history[9], str);        
	  }	
        (*size_history)++;
      }
    }

/* takes care of recent history cmds !! and !d */
    int recent_history(char str[], char history[][MAX_LINE], int *size_history){
      int c1,c2,c3,d;
      char strend[MAX_LINE];

      if(str[0] == '!' &&  str[1] == '!'){
        strcpy(strend,&str[2]);
        memset(str, 0, MAX_LINE);
        if(*size_history < 10){
          strcpy(str, history[*size_history-1]);
          str[strlen(history[*size_history - 1]) - 1] = '\0';
          strcat(str,strend);
	  add_to_history(str, history, size_history);
        }
        else if(*size_history >= 10){
          strcpy(str,history[9]);     
          str[strlen(history[9]) - 1] = '\0';
          strcat(str,strend);
	  add_to_history(str, history, size_history);
        }

        if(*size_history == 0){
          printf("No commands in history.\n");
	  return 2;
        }
        return 1;
      }
      else if(str[0] == '!' && (str[1] >= 48 && str[1] <=57) && (str[2] < 48 || str[2] > 57)){
        d = str[1] - '0';
        if(!(d < *size_history - 9 || *size_history == 0 || d > *size_history || d == 0)){
          strcpy(strend,&str[2]);
          memset(str, 0, MAX_LINE);
	  if(*size_history < 10){
          strcpy(str,history[d - 1]);
          str[strlen(history[d - 1]) - 1] = '\0';
	  }
	  else{
	  strcpy(str,history[abs(*size_history - d - 9)]);
          str[strlen(history[abs(*size_history - d - 9)]) - 1] = '\0';
	  }
          strcat(str,strend);
	  add_to_history(str, history, size_history);
        }
	else{   
	  printf("No such command in history.\n");
	  return 2;
	}
        return 1;
      }  
      else  if(str[0] == '!' && (str[1] >= 48 && str[1] <=57) && (str[2] >= 48 && str[2] <=57) && (str[3] < 48 || str[3] > 57) ){
        c1 = str[1] - '0';
        c2 = str[2] - '0';
        d = c1*10 + c2;
        if(!( d < *size_history - 9|| *size_history == 0 || d  > *size_history || d == 0)){
          strcpy(strend, &str[3]);
          memset(str, 0, MAX_LINE);
          strcpy(str,history[abs(*size_history - d - 9)]);
          str[strlen(history[abs(*size_history - d - 9)]) - 1] = '\0'; 
          strcat(str,strend);
	  add_to_history(str, history, size_history);
        }
	else{   
	  printf("No such command in history.\n");
	  return 2;
	}
        return 1;
      }
      else  if(str[0] == '!' && (str[1] >= 48 && str[1] <=57) && (str[2] >= 48 && str[2] <=57)  && (str[2] >= 48 && str[2] <=57)&& (str[4] < 48 || str[4] > 57) ){
        c1 = str[1] - '0';
        c2 = str[2] - '0';
        c3 = str[3] - '0';
        d = c1*100 + c2*10 + c3;
        if(!( d < *size_history - 9 || *size_history == 0 ||  d > *size_history || d == 0)){
          strcpy(strend,&str[4]);
          memset(str, 0, MAX_LINE);
          strcpy(str,history[abs(*size_history - d - 9)]);
          str[strlen(history[abs(*size_history - d - 9)]) - 1] = '\0';   
          strcat(str,strend);
	  add_to_history(str, history, size_history);
        }
	else{   
	  printf("No such command in history.\n");
	  return 2;
	}
        return 1;
      }

      return 0;
    }

    int main(void){ 
      char *args[MAX_LINE/2 + 1]; /* command line arguments */
      char *multcmds[MAX_LINE/2 + 1]; /* multiple command line arguments */
      int should_run = 1; /* flag to determine when to exit program */
      pid_t pid1, pid2;  /* child flags */
      char str[MAX_LINE]; /* for reading user input */
      char str2[MAX_LINE];  /* for reading multiple cmds */
      char history[10][MAX_LINE];  /* keeps track of history */
      int size_history = 0;  /* keeps track of number args added to history */
      int i;  /* used to find & */
      int status;  /* status of child processes */
      int added;  /* used to determine whether we add to history or not */
      int numcmds_read = 0;  /* number of cmds read in multiple cmds */
      int total_numcmds = 0;  /* total number of cmds in multiple cmds */
      int flag_wait, flag_multcmds, flag_pipe;  /* different flags for different args */
      char *pipe1[MAX_LINE/2 +1];  /* left side of the pipe */
      char *pipe2[MAX_LINE/2 +1];  /* right side of the pipe */
      int fd[2];  /* file descriptor */

      while (should_run) {
        flag_wait = 0;
        flag_pipe = 0;
        i = 1;

	/* gets input from user as long as were not reading multiple arguments */
        if(numcmds_read == 0 || numcmds_read == total_numcmds){
          printf("osh>");
          fflush(stdout);
          memset(str, 0, sizeof(str));
          fgets(str, MAX_LINE, stdin);
          numcmds_read = 0;
          flag_multcmds = 0;
        }  

	/* check if it is a recent history command */
        added = recent_history(str, history, &size_history);

	/* checks for multiple cmds */
        if(strstr(str, ";") != NULL && numcmds_read == 0){
          flag_multcmds = 1;
          add_to_history(str, history, &size_history);
          memset(multcmds, 0, sizeof(multcmds)/sizeof(multcmds[0]));
	  memset(str2, 0, sizeof(str2));
	  strcpy(str2, str);
          total_numcmds = parse_mult_cmds(str2, multcmds);      
        }

	/* does the work of breaking up multiple cmds into single cmds */
        while(flag_multcmds == 1){
          memset(str, 0, sizeof(str));
          strcpy(str, multcmds[numcmds_read]);
          memset(args, 0, sizeof(args)/sizeof(args[0]));
	  if(strstr(str, "|") == NULL)
          parse_str(str, args);
          numcmds_read++;
          break;       
        }

	/* checks for pipe cmds */
	if(strstr(str, "|") != NULL){
	  if(pipe(fd) == -1) {
	    perror("Pipe failed");
	    exit(1);
	  }
          flag_pipe = 1;
	  if(flag_multcmds == 0)
          add_to_history(str, history, &size_history);  
	  memset(args, 0, sizeof(args)/sizeof(args[0]));    
          parse_pipe(str,args);  
	  memset(pipe1, 0, sizeof(pipe1)/sizeof(pipe1[0]));
	  memset(pipe2, 0, sizeof(pipe2)/sizeof(pipe2[0]));
          parse_str(args[0], pipe1);
          parse_str(args[1], pipe2);
        }

	/* takes care of basic cmds */
        if(flag_multcmds == 0 && flag_pipe == 0){
          if(added == 0)
        add_to_history(str, history, &size_history);
           memset(args, 0, sizeof(args)/sizeof(args[0]));  
          parse_str(str, args);
        }

        if(args[0] == NULL) args[0] = "";

	/* history cmd */
        if(strcmp(args[0], "history") == 0)
          print_history(history, size_history);

	/* exit cmd */
        if(strcmp(args[0],"exit") == 0)
          should_run = 0;
	
	/* checks for & */
        while(args[i] != NULL){
          if(strcmp(args[i],"&") == 0){
	    args[i] = NULL;
	    flag_wait = 1;
	    break;
          }
          i++;
        }
	if(args[i + 1] != NULL) flag_wait = 0;

	/* create child process*/
        pid1  = fork();
        if(pid1 < 0) {  /*error occured */
          fprintf(stderr, "Fork Failed");
          exit(EXIT_FAILURE);
        }
        else if(pid1 == 0){
	  /* if pipe arg 
	     else basic cmd */
          if(flag_pipe == 1){
	    
	   close(STDOUT_FILENO);  //closing stdout
            dup(fd[1]);         //replacing stdout with pipe write 
            close(fd[0]);       //closing pipe read
            close(fd[1]);       //closing pipe write
            execvp(pipe1[0], pipe1);

          }
          else if(-1 == execvp(args[0], args) && strcmp(str,"\n") != 0  && strcmp(args[0],"history") != 0  && strcmp(args[0],"exit") && added != 2){	
	    printf("Error executing command\n");
          }
	  
          exit(EXIT_SUCCESS);
        }
        else{
	  /* if pipe arg - create second child
	   else wait for child 1 if no '&' */
	  if(flag_pipe == 1){
	    
	    pid2 = fork();
	    if(pid2 < 0) {   /*error occured */
	      fprintf(stderr, "Fork Failed");
	      exit(EXIT_FAILURE);
	    }
	    else if(pid2 == 0){
	      close(STDIN_FILENO);   //closing stdin
	      dup(fd[0]);         //replacing stdin with pipe read
	      close(fd[1]);       //closing pipe write
	      close(fd[0]);      //closing pipe read

	      execvp(pipe2[0], pipe2);
	      exit(EXIT_FAILURE);
	    }
	    close(fd[0]);  //close pipe read
	    close(fd[1]);  //close pipe write
	    waitpid(pid1, &status, 0); //wait for child 1
	    waitpid(pid2, &status, 0); // wait for child 2
	    
	  }
	  else if(flag_wait == 0){
	    waitpid(pid1, &status, 0); //wait for child 1
	 
	  }
	}    
      }
      return 0;
    }
