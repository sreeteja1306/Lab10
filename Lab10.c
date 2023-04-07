#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>


int getLine(char *line,FILE *fp) {

	int c, i=0;
	while ((c = getc(fp)) != '\n' && c != EOF)
		line[i++] = c;
	line[i] = '\0';

	return i;
}

void cmdParser(char *opt,char **cmd){
    int count = 0;
    char *commandLine = strdup(opt);
    commandLine = strtok(commandLine," ");
    while (commandLine != NULL){
        cmd[count++]=commandLine;
        commandLine = strtok(NULL, " ");
    }
    cmd[count++]=NULL;
}

int main (int argc, char **argv)
{
    char str[1000];
	FILE *fp,*fpout;

    if((fp = fopen(argv[1],"r"))==NULL){
        printf("File Opening Error");
        exit(-1);
    }

    if((fpout = fopen("output.log","w"))==NULL){
        printf("File Opening Error");
        exit(-1);
    }

    while (getLine(str, fp) > 0){
        char *cmd[100];
        int status;
        cmdParser(str,cmd);
        time_t end_time, start_time;
        time(&start_time);
		int forkid = fork();
        if (forkid == 0){
            int fderr, fdout;
            char file_out[BUFSIZ], file_err[BUFSIZ];

            snprintf(file_out, BUFSIZ, "%d.out", getpid());
            snprintf(file_err, BUFSIZ, "%d.err",  getpid());

            fdout = open(file_out, O_CREAT | O_TRUNC | O_WRONLY, 0755);
            fderr = open(file_err, O_CREAT | O_TRUNC | O_WRONLY, 0755);

            if (fdout == -1 || fderr == -1)
            {
                printf("Error opening file %d\n", getpid());
                exit(-1);
            }

            dup2(fdout, 1);
            dup2(fderr, 2);
            execvp(cmd[0],cmd);
        }
        else if (forkid > 0){
            wait(NULL);
            time(&end_time);
            fprintf(fpout,"%s\nstart time: %send time: %s", str, ctime(&start_time),ctime(&end_time));
        }
    }
    fclose(fp);
    fclose(fpout);
    return 0;
}