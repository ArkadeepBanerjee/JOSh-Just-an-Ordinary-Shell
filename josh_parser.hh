
#ifndef _JOSh_PARSER_
#define _JOSh_PARSER_

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
#include<csignal>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<setjmp.h>
#include<error.h>
#include<getopt.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include <fcntl.h>

#define MAX_ARG 32
#define MAX_VAL 1024

struct command {
    char* argList[MAX_ARG];
    int argListSize;
};

typedef std::vector<command*> commandList;

class Parser {
public:
    bool valid; //checks certain discernible cases of validity of the Command
    command cList[MAX_ARG];
    std::string cText; 
    int cListSize;
    bool inDirected;
    bool outDirected;
    bool background;
    char inFile[MAX_VAL];
    char outFile[MAX_VAL];
    int err;
    void parseCommand(std::string);
    int spaceCruncher(std::string , int);
    
       
    Parser(const char* cLine) {
        valid = true;
        cText = std::string(cLine);
        cListSize = 0;
        inDirected = false;
        outDirected = false;
        err = -1;
    }
    ~Parser() {
        for (int i = 0; i<cListSize; i++) {
            for (int j =0; j <cList[i].argListSize; j++)
            {
                if(cList[i].argList[j])
                    free(cList[i].argList[j]);
            }
        }
        
    }
    void parse();
    void printError();
    void print();
      
};
#endif    
    
