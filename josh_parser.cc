#include"josh_parser.hh"


void Parser::parse() {
    
    std::size_t pipeLoc = -1;
    std::size_t prevLoc = -1;
    
    if (cText.find('&') != std::string::npos) {
        background = true;
        cText.erase(cText.find('&'),1);
    }
    
    while ((pipeLoc = cText.find('|',pipeLoc+1)) != std::string::npos) {
        //std::cout<<pipeLoc;
        
        std::string com(cText,prevLoc+1,(pipeLoc-prevLoc-1));
        //std::cout<<com<<"\n";
        prevLoc = pipeLoc;
        parseCommand(com);
        cListSize++;
        
    }
    std::string com(cText,prevLoc+1,(cText.length()-prevLoc-1));
    //std::cout<<com<<"\n";
    if (com.length() != 0) { 
        parseCommand(com);
        cListSize++;
    }
    else {
        valid =false;
        err = 1;
    }
}

int Parser:: spaceCruncher(std::string s, int i) {
    while(s[i] == ' ') {
        i++;
    }
    return i;
}

    
void Parser::parseCommand(std::string s) {
    
    cList[cListSize].argListSize = 0;
    std::string buff;
    
    for (int i = 0; i < s.length() ; i++) {
        if (s[i] == '>') {
            if(outDirected == true) {
                valid = false;
                err = 2;
            }
            if (buff.length() != 0) {
                cList[cListSize].argList[cList[cListSize].argListSize] = (char *)malloc(sizeof(char)*(buff.size()+1));
                strcpy(cList[cListSize].argList[cList[cListSize].argListSize++],buff.c_str());
                buff = std::string();
            }
            if (cList[cListSize].argListSize == 0) {
                valid =false;
                err = 3;
            }
            outDirected = true;
            i++;
            i = spaceCruncher(s,i);
            while( s[i] != '>' && s[i] != '<' && s[i] != ' ' && i != s.length() ) {
                buff = buff + s[i];
                i++;
            }
            if (buff.length() != 0) {
                strcpy(outFile,buff.c_str());
                buff = std::string();
            } else {
                valid = false;
                err = 4;
            }
            i--;
        } else if (s[i] == '<') {
            if(inDirected == true) {
                valid = false;
                err = 2;
            }
            if (buff.length() != 0) {
                cList[cListSize].argList[cList[cListSize].argListSize] = (char *)malloc(sizeof(char)*(buff.size()+1));
                strcpy(cList[cListSize].argList[cList[cListSize].argListSize++],buff.c_str());
                buff = std::string();
            }
            if (cList[cListSize].argListSize == 0) {
                valid =false;
                err = 3;
            }
            inDirected = true;
            i++;
            i = spaceCruncher(s,i);
            while( s[i] != '>' && s[i] != '<' && s[i] != ' ' && i != s.length() ) {
                buff = buff + s[i];
                i++;
            }
            if (buff.length() != 0) {
                strcpy(inFile,buff.c_str());
                buff = std::string();
            } else {
                valid = false;
                err = 4;
            }
            i--;   
        } else if (s[i] == ' ' || i == (s.length() - 1)) {
            if ( i == (s.length() - 1) && s[i] != ' ') {
                buff = buff + s[i]; 
            }
            if (buff.length() != 0) {
                cList[cListSize].argList[cList[cListSize].argListSize] = (char *)malloc(sizeof(char)*(buff.size()+1));
                strcpy(cList[cListSize].argList[cList[cListSize].argListSize++],buff.c_str());
                buff = std::string();
            }
        } else {
            buff = buff + s[i];
        }
    }
    cList[cListSize].argList[cList[cListSize].argListSize] = (char *)malloc(sizeof(char));
    cList[cListSize].argList[cList[cListSize].argListSize] = NULL;
    //print();
    
}
        
        
void Parser::printError() {
    switch(err) {
        case 1: std::cout<<"\nKindly check if there is a dangling pipe";
                break;
        case 2: std::cout<<"\nKindly check if there is multiple indirection or redirection";
                break;
        case 3: std::cout<<"\nKindly check if there is a command before indirection or redirection";
                break;
        case 4: std::cout<<"\nKindly check if there is valid infile/outfile specified";
                break;
        default: std::cout<<"\nUnknown Error";
                break;
    }
    std::cout<<std::endl;
}

void Parser:: print() {
    std::cout<<"\n COMMAND: \n"<<cText<<((background)?"&\n":"\n");
    std::cout<<"\nNo. of Command: "<<cListSize;
    if(inDirected) {
        std::cout<<"\nIndirected: "<<inDirected;
        std::cout<<"\nIndirected File: "<<inFile;
    }
    if(outDirected) {
        std::cout<<"\nOutirected: "<<outDirected;
        std::cout<<"\nOutdirected File: "<<outFile;
    }
    std::cout<<"\nBackground: "<<background;
    std::cout<<"\nVALIDITY of INPUT: "<< ((valid)?"OK":"FAIL");
    //commandList::iterator it;
    std::cout<<"\nIndividual Commands\n";
    for (int i =0; i < cListSize; i++) {
        
        for(int j = 0; j < cList[i].argListSize; j++) {
            std::cout<<cList[i].argList[j]<<"  ";
        }
        std::cout<<std::endl;
    }
}
        
    
    
    
    
   
/*int main() {
   Parser obj("cat -f $ggf \"casd\"   <abc.txt|dir -ls  -LCC   -cd=std++11|echo   $SDF  -f | cat   c:/panu/leone/stat.txt      --name=line |cp    -rm c:/asd c:asqw    | rmdir -all +fda> c:/OS/whatever &   ");
   //Parser obj("   cat    >   abc.txt  <   ls.txt  -lfa  |   srd=c++11  x al &   ");
   //Parser obj("cat&");
   obj.parse(); 
   obj.print();
}*/
