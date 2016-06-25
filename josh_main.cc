

#include"josh_main.hh"

#include<readline/readline.h>
#include<readline/history.h>

sigjmp_buf ctrlc_buf;


void JOSH::execute(Parser *p) {
    
    if (p->valid == false) {
        std::cout<<"\nERROR: Very Bad Command :'( ";
        p->printError();
        return;
    }
    //Substitute Shell Variables
    for (int i = 0; i < p->cListSize; i++) {
        replaceShellVar((const char**)p->cList[i].argList,p->cList[i].argListSize);
    }
    
    int cntBI = 0;
    for (int i = 0; i < p->cListSize; i++) {
        if (isBuiltIn(p->cList[i].argList[0])) {
            cntBI++;
        }
    }
    /*if (cntBI > 1 || (cntBI > 0 && p->cListSize > 1)) {
        std::cout<<"\nERROR: More than 1 Builtins provided OR Builtins with PIPING given...\n";
        return;
    }*/
    executeEach(p);
    
    
        
}
    
    
    
void JOSH::executeEach(Parser *p) {

    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stdin = dup(STDIN_FILENO);

    if (p->cListSize == 1) { //No Piping
        if (isBuiltIn(p->cList[0].argList[0])) {
            if (p->outDirected) {
                changeIO(p->outFile,O_WRONLY|O_CREAT|O_TRUNC,STDOUT_FILENO);
             }
            executeBuiltIn((const char**)p->cList[0].argList,p->cList[0].argListSize); 
            dup2(saved_stdout,STDOUT_FILENO);
            close(saved_stdout);
        } else {
            pid_t pid;
            int status;
            switch (pid = fork()) {
            
                case -1: perror("Forking ERROR: ");
                         exit(-1);
                         
                case 0:  if (p->inDirected) {
                            changeIO(p->inFile,O_RDONLY,STDIN_FILENO);
                         }
                         if (p->outDirected) {
                            changeIO(p->outFile,O_WRONLY|O_CREAT|O_TRUNC,STDOUT_FILENO);
                         }
                         execvp(p->cList[0].argList[0],p->cList[0].argList);
                         perror("Execing ERROR: ");
                         exit(-1);
                         
                default: if (p->background == false) {
                            while (wait(&status) != pid);
                         }
            }
        } 
    } else {  //PIPINGs
        int fifo[2][2];
        int bg = p->background;
        int curr_pipe = 0;
        pid_t pid;
        
        for (int i = 0; i < p->cListSize; i++) {
            if (pipe(fifo[curr_pipe]) == -1) {
                perror("Piping ERROR: ");
                return;
            }
            switch (fork()) {
            
                case -1: perror("Forking ERROR: ");
                         exit(-1);
                         
                case 0:  if (i > 0) { //Not the first Command
                            dup2(fifo[1-curr_pipe][0],STDIN_FILENO);
                            close(fifo[1-curr_pipe][0]);
                         } else if (p->inDirected) {
                            changeIO(p->inFile,O_RDONLY,STDIN_FILENO);
                         }
                         if (i != p->cListSize -1) {//Not the Last Command
                            dup2(fifo[curr_pipe][1],STDOUT_FILENO);
                         } else if (p->outDirected) {
                            changeIO(p->outFile,O_WRONLY|O_CREAT|O_TRUNC,STDOUT_FILENO);
                         }
                         close(fifo[curr_pipe][1]);
                         close(fifo[1-curr_pipe][0]);
                         if (isBuiltIn(p->cList[i].argList[0])) {
                            executeBuiltIn((const char**)p->cList[i].argList,p->cList[i].argListSize);
                            exit(0);
                         } else {                            
                             execvp(p->cList[i].argList[0],p->cList[i].argList);
                             perror("Execing ERROR: ");
                             exit(-1);
                         }
             }
             if (i > 0) {
                 close(fifo[1-curr_pipe][0]);
             }
                 close(fifo[curr_pipe][1]);
             
             curr_pipe = 1 - curr_pipe;
         }
         close(fifo[1-curr_pipe][0]);
         while (!bg && (pid = waitpid(-1,0,0)) != -1);  
    }
}


void JOSH::start() {
    //housekeeping like populating BuiltIn List,storing PATH variable etc.
    populateBuiltIns();
    prevDir = getcwd(NULL, MAX_LEN);
    rl_bind_key('\t', rl_complete);
    signal(SIGINT, SIG_IGN);
	signal(SIGINT, handle_signal);
	rl_catch_signals = 1;
	rl_set_signals();
    
    //shellVars["s1"] = "Hello";
    //shellVars["s2"] = "Bye";
    
    while ( sigsetjmp( ctrlc_buf, 1 ) != 0 );
    while(true) {
        snprintf(prompt, sizeof(prompt), "[%s] on JOSH ~%s# ", getenv("USER"), getcwd(NULL, MAX_LEN));
        inputLine = readline(prompt);
        if (!inputLine || strcmp(inputLine,"bye") == 0) {
            bye();
        } else {
            char *i = inputLine;
            inputLine = trim(inputLine);
            Parser p(inputLine);
            p.parse();
            execute(&p);
            //p.print();
            //replacing shell variables
            //replaceShellVars(&p);
            if (inputLine && *inputLine) {
                add_history(inputLine);
            }
            free(i);
        }
    }
}


//System Command
void handle_signal(int signo) {
	if (signo == SIGINT) {
	    std::cout<<std::endl;
	    siglongjmp(ctrlc_buf, 1);
    }
    
    
}
 
int main()
{
    
    JOSH obj;
    obj.start();
}
