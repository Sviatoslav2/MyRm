#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <thread>
#include <cmath>
#include <sstream>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

namespace fs1 = boost::filesystem;

void Enter(){
    std::cout<<"Please enter  Y[es]/N[o]/A[ll]/C[ancel]"<<std::endl;
}


void Help(){
    std::cout << "myrm [-h|--help] [-f] [-R] <file1> <file2> <file3>" << std::endl;
}

std::string get_current_directory(){
    char buf[200];
    getcwd(buf, sizeof(buf));
    return std::string(buf);
}

bool isFileExitInDirectory(std::string dir){
    fs1::path p{dir};
    bool key = fs1::is_regular_file(p);
    return key;
}

void Remove(fs1::path Path){
    if (not (fs1::remove (Path))){
        //if(isFileExitInDirectory(Path.string())){
        std::cout<<"File was not deleted(it was not detected in path : "<<Path<<" )"<<std::endl;
        //}
        exit(1);
    }
}

std::vector<std::string> split_cmd_line(std::istream& is) {
    std::string commandline;
    std::vector<std::string> parsed_command;
    std::getline(is, commandline);
    std::stringstream ss(commandline);
    std::string word;
    while (ss >> word) {
        parsed_command.push_back(word);
    }
    return parsed_command;
}

std::string getFromUser(){

    bool key = true;
    std::vector<std::string>Vector;
    while(key){
        Enter();
        Vector = split_cmd_line(std::cin);
        if(Vector[0] == "Y"|| Vector[0] == "y"|| Vector[0] == "N"|| Vector[0] == "n"|| Vector[0] == "A"|| Vector[0] == "a" || Vector[0] == "C"|| Vector[0] == "c"){
            key = false;
        }
    }
    return Vector[0];
}


bool rmFile(fs1::path Path, bool keyf, bool AllKey){
    //std::cout<<Path.string()<<std::endl;
    if(keyf){
        if(Path.parent_path().empty()){
            fs1::path src_path = get_current_directory() + "/" + Path.filename().string();
            Remove(src_path);
        }
        else{
            Remove(Path);
        }
    }
    else{

        std::string key = getFromUser();
        if(key == "Y" || key == "y"){
            rmFile(Path,not keyf,AllKey);
        }
        else if(key == "C" || key == "c"){

            exit(0);
        }
        else if(key == "A" || key == "a"){
            AllKey = true;
        }
    }
    return AllKey;
}

int main(int argc, char *argv[]) {
    bool keyf = false;
    bool helpKey = false;
    bool RegimeR = false;
    bool AllKey = false;
    std::string R = "-R";
    std::string f = "-f";
    
    std::vector<std::string> DataFromConsol;
    for(int i = 1; i < argc; ++i){

        if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
            helpKey = true;
        }
        else if(strcmp(argv[i], "-R") == 0){
            RegimeR = true;
        }
        else if((strcmp(argv[i], "-f") == 0)){
            keyf = true;
        }
        else{
            DataFromConsol.emplace_back(argv[i]);
        }
    }
    if(helpKey){
        Help();
        exit(0);
    }
    int n = 0;
    if(RegimeR && keyf){
        n = 2;
    }
    else if(RegimeR || keyf){
        n = 1;
    }


    for(int i = n; i < DataFromConsol.size(); ++i){
        fs1::path Path = DataFromConsol[i];

        if(RegimeR){
            if(fs1::is_directory(Path)){
                std::cout<<Path.string()<<std::endl;
                bool key = rmFile(Path,keyf,AllKey);
                if(key){keyf = true;
                    rmFile(Path,keyf,AllKey);
                }

            }
            else{
                std::cout<<Path.string()<<std::endl;
                bool key = rmFile(Path,keyf,AllKey);
                if(key){keyf = true;
                    rmFile(Path,keyf,AllKey);
                }
            }
        }
        else{
            if(not (fs1::is_directory(Path))){
            std::cout<<Path.string()<<std::endl;
            bool key = rmFile(Path,keyf,AllKey);
            if(key){
                keyf = true;
                rmFile(Path,keyf,AllKey);
                }
            }
            else{
                std::cout<<"Error: you use directory without -R"<<std::endl;
                exit(1);
            }
        }
    }
    return 0;
}