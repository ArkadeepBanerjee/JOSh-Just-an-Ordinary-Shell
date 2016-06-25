/*******************************************************************************
*   JOSh 
*******************************************************************************/

#include"josh_main.hh"

#include<readline/readline.h>
#include<readline/history.h>

void JOSH::executeBuiltIn(const char* arg[], int argNo) {
    if (std::string(arg[0]) == "chdir") {
        changeDir(arg);
    } else if (std::string(arg[0]) == "set") {
        set(arg);
    } else if (std::string(arg[0]) == "unset") {
        unset(arg);
    }  else if (std::string(arg[0]) == "say") {
        say(arg, argNo);
    } else if (std::string(arg[0]) == "appendpath") {
        setPATH(arg);
    }
}

void JOSH::populateBuiltIns() {
    builtIns.push_back(std::string("chdir"));
    builtIns.push_back(std::string("set"));
    builtIns.push_back(std::string("unset"));
    builtIns.push_back(std::string("aka"));
    builtIns.push_back(std::string("unaka"));
    builtIns.push_back(std::string("bye"));
    builtIns.push_back(std::string("say"));
    builtIns.push_back(std::string("appendpath"));
}

bool JOSH::isBuiltIn(const char* bi) {
    stringList::iterator it;
    
    for (it = builtIns.begin(); it != builtIns.end(); it++) {
        if (*it == bi)
            return true;
    }
    return false;
}


//BUILT-INs
void JOSH::changeDir(const char* arg[]) {
    std::string temp;
    if (strcmp(arg[1],"-") == 0) {
        temp = prevDir.c_str();
    } else if (strcmp(arg[1],"~") == 0) {
        temp = "/";
    } else {
        temp = arg[1];
    }
    prevDir = getcwd(NULL, MAX_LEN);
    if (chdir(temp.c_str())) {
        std::cout<<"Change Directory to "<<temp.c_str()<<" unsuccessful.\n";
    }
}

void JOSH::set(const char* arg[]) {
    std::string var = arg[1];
    std::string value = arg[2];
    shellVars[var] = value;
    std::cout<<"$"<<var<<" = "<<value<<std::endl;
}
        
void JOSH::unset(const char* arg[]) {
    stringMap::iterator it;
    it = shellVars.find(std::string(arg[1]));
    
    if (it != shellVars.end()) {
        std::cout<<"$"<<(*it).first<<" = "<<(*it).second<<std::endl;
        shellVars.erase(it);
    } else {
        std::cout<<arg[1]<<" is not a Shell Variable\n";
    }
    
}

void JOSH::setPATH(const char* arg[]) {
    struct stat st;
    if( stat(arg[1],&st) == 0 && (st.st_mode & S_IFDIR) ) {
        std::string s (getenv("PATH"));
        s = s +":";
        s =s + arg[1];
        setenv("PATH",s.c_str(),1);
        std::cout<<"PATH = "<<getenv("PATH")<<"\n";
    } else {
        std::cout<<arg[1]<<" is not a Directory\n";
    }
}
        

void JOSH::say(const char* arg[], int argNo) {
    for (int i = 1; i< argNo; i++) {
        std::cout<<arg[i]<<" ";
    }
    std::cout<<std::endl;
}
    

void JOSH::bye() {
    std::cout<<"\nExiting...\n";
    exit(0);
}
