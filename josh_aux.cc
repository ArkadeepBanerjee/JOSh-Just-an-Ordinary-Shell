/*******************************************************************************
*   JOSh 
*******************************************************************************/

#include"josh_main.hh"

void JOSH::replaceShellVar(const char* arg[], int argNo) {
    stringMap::iterator it;
    
    for (int i = 0; i < argNo; i++) {
        if (arg[i][0] == '$') {
            it = shellVars.find(std::string(std::string(arg[i]),1,std::string::npos));
            if( it != shellVars.end()) {
                arg[i] = (*it).second.c_str();
            }
        }
    }
}


void JOSH::changeIO(const char* fileName, mode_t mode, int old) {
    int fd = open(fileName,mode,0777);
    if (fd == -1) {
        perror("Redirecting ERROR: ");
        exit(-1);
    } else {
        if (dup2(fd,old) == -1) {
            perror("dup2 ERROR: ");
            exit(-1);
        }
        close(fd);
    }
}

char* JOSH::trim(char* s) {
    while(*s == ' ' && *s != '\0') {
        s++;
    }
    return s;
}
        
