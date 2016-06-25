#ifndef _JOSH_MAIN_
#define _JOSH_MAIN_

#include<iostream>
#include<vector>
#include<map>
#include<unordered_set>
#include<algorithm>
#include<cctype>
#include<string>
#include<stack>
#include<queue>
#include<cstdlib>
#include<cstdio>
#include<unistd.h>
#include<signal.h>
#include<setjmp.h>
#include<errno.h>
#include<error.h>
#include<getopt.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/wait.h>
#include<sys/stat.h>

#include"josh_parser.hh"

#define MAX_LEN 1024
#define CLOSE_PIPE

class JOSH;

typedef std::map<std::string,std::string> stringMap;
typedef std::vector<std::string> stringList;
typedef void (*sighandler_t)(int);

void handle_signal(int signo);


class JOSH {
private:
    char prompt[MAX_LEN];
    char *inputLine;
    stringList builtIns;
    stringMap shellVars;
    stringList path;
    std::string prevDir;
    
    char* trim (char*);
    
    
public:
    JOSH() {
    inputLine = NULL;
    } 
    
    void start();
    void execute(Parser *);
    void executeEach(Parser*);
    void changeIO(const char*, mode_t ,int);
    
    
    //Full Paths
    
    //Related to Shell Variable Substitution
    void replaceShellVar(const char* [], int);
    //Related to Builtins
    void executeBuiltIn(const char* [], int);
    void populateBuiltIns();
    bool isBuiltIn(const char*);
    //Builtins
    void changeDir(const char* []);
    void setPATH(const char*[]);
    void set(const char*[]);
    void unset(const char*[]);
    void bye();
    //void aka(const char* []);
    //void unaka(const char* []);
    void say(const char* [], int argNo);
    
};

#endif

