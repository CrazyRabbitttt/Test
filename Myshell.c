#include "csapp.h"
#define MAXARGS 128

void eval(char *cmdline);
int parseline(char * buf, char **argv);
int builtin_command(char **argv);

int main()
{
    char cmdline[MAXLINE];

    while(1) {
        printf("> ");
        Fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin))
            exit(0);
        eval(cmdline);      //judge cmdline if a syscall..
    }
}


int builtin_command(char **argv) {
    //if 'quit' ,call exit(0)
    //if '&' , return 0,running back
    //if equal , strcmp() return 0
    if (!strcmp(argv[0], "quit"))
        exit(0);
    if (!strcmp(argv[0], "&"))
        return 1;
    return 0;
}

int parseline(char *buf, char **argv)
{
    char *delim;        //Point to the space delimter
    int argc;           //The number of arguments
    int bg;             //if is background runing

    buf[strlen(buf) - 1] = ' ';         //replace '\n' by ' '

    while (*buf && (*buf == ' '))       //Ignore the leading space s
        buf ++;

    while (delim = strchr(buf, ' ')) {
        argv[argc++] = buf;
        *delim = '\0';                  //Separate the String by '\0'
        buf = delim + 1;
        while (*buf && (*buf == ' '))   //Ignore the leading space s
            buf++;
    }

    argv[argc] = NULL;                  //The last args will be NULL

    if (argc == 0)
        return 1;                       //Ignore blink line

    /*Judge if run in the background */
    if (*argv[argc - 1] == '&') {
        bg = 1;
        argv[--argc] = NULL;
    }

    return bg;


}

void eval(char *cmdline) {
    char *argv[MAXARGS];
    char buf[MAXLINE];
    int bg;
    pid_t pid;

    strcpy(buf, cmdline);
    bg = parseline(buf, argv);          //parse command

    if (argv == NULL) return ;

    if (!builtin_command(argv)) {       //not a builtin command
        if ((pid = Fork()) == 0) {      /*Child process*/
            if (execve(argv[0], argv, environ) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }

        /*Father waits for foreground job to terminate*/
        if (!bg) {
            int status;
            if (waitpid(pid, &status, 0) < 0) {
                unix_error("waitfg: waitpid error.");
            }
        }else printf("%d %s",pid , cmdline );
    }

    return ;

}

