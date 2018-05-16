#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "termcolor.hpp"

std::fstream file;
std::vector<std::string> * tasks;

void load() {
    file.open(".tdlist", std::ios::in);
    std::string line;
    while(std::getline(file, line)){
        tasks->push_back(line);
    }
    file.close();
}

void write() {
    file.open(".tdlist", std::ios::out);
    for(int i = 0; i < tasks->size(); i++) {
        file << tasks->at(i) << std::endl;
    }
    file.close();
}

void init() {
   load();
   if(tasks->size() != 0){
        std::cerr << termcolor::red << "fatal:" << termcolor::reset << " tdlist project already initialized" << std::endl;
        exit(-1);
   }
   file.open(".tdlist", std::ios::out);
   file << "tdlist" << std::endl;
   file.close();
}

void checkInit() {
    if(tasks->size() == 0){
        std::cerr << termcolor::red << "fatal:" << termcolor::reset << " not a tdlist project directory" << std::endl;
        exit(-1);
    }
}

int find(std::string task) {
   int counter = 0;
   for(int i = 1; i < tasks->size(); i++) {
        std::string bufferTitle = tasks->at(i).substr(0, tasks->at(i).find("::"));
        if(!strcmp(bufferTitle.c_str(), task.c_str())){
            return i;
        }
   }
   return -1;
}

void open(std::string title) {
    load();
    checkInit();
    if(find(title) == -1) {
        tasks->push_back(title + "::0u");
    } else if(tasks->at(find(title))[tasks->at(find(title)).find("::") + 2] == '1') {
        tasks->erase(tasks->begin() + find(title));
        tasks->push_back(title + "::0u");
    } else {
        std::cerr << termcolor::red << "fatal: " << termcolor::reset << title << " already exists" << std::endl;
        exit(-1);
    }
    write();
}

void rm(std::string title) {
    load();
    checkInit();
    int taskIndex = find(title);
    if(taskIndex != -1) {
        tasks->erase(tasks->begin() + taskIndex);
    }
    write();
}

void list() {
    load();
    checkInit();
    for(int i = 1; i < tasks->size(); i++){
        std::cout << tasks->at(i) << std::endl;
    }
    if(tasks->size() == 1) {
        std::cout << termcolor::blue << "info:" << termcolor::reset << "list empty" << std::endl;
    }
}

std::string updateTaskParam(std::string title, int paramIndex, std::string value) {
    int taskIndex = find(title);
    std::string newTask = title;
    std::string task = tasks->at(taskIndex);
    int divider = task.find("::") + 2;
    newTask += "::" + task.substr(divider, paramIndex) + value + task.substr(divider + paramIndex + 1);
    return newTask;
}

void close(std::string title) {
    load();
    checkInit();
    int taskIndex = find(title);
    if(taskIndex != -1) {
        tasks->at(taskIndex) = updateTaskParam(title, 0, "1");    
        std::cout << termcolor::blue << "info:" << termcolor::reset << " closed " << title << std::endl;
    } else {
        std::cerr << termcolor::red << "fatal: " << termcolor::reset << title << " doesn't exists" << std::endl;
        exit(-1);
    }
    write();
}

int main(int argc, char * argv[]) {
    tasks = new std::vector<std::string>();
    
    if(argc == 1) {
        std::cerr << termcolor::yellow << "usage:" << termcolor::reset << " tdlist <command> [<args>]" << std::endl;
        exit(-1);

    } else if(!strcmp(argv[1], "init")) {
        init();

    } else if(!strcmp(argv[1], "open")){
        if(argc >= 3){
            open(argv[2]);
        } else {
            std::cerr << "usage: tdlist open <name>" << std::endl;
            exit(-1);
        }

    } else if(!strcmp(argv[1], "rm")){
        if(argc >= 3){
            rm(argv[2]);
        } else {
            std::cerr << "usage: tdlist rm <name>" << std::endl;
            exit(-1);
        }

    } else if(!strcmp(argv[1], "list")){
       list(); 
    
    } else if(!strcmp(argv[1], "close")) {
       if(argc >= 3) {
            close(argv[2]);
       } else {
            std::cerr << "usage: tdlist close <name>" << std::endl;
            exit(-1);
       }
     
    } else if(!strcmp(argv[1], "open")) {
        if(argc >= 3) {
            open(argv[2]);
        } else {
            std::cerr << "usage: tdlist open <name>" << std::endl;
        }

    } else {
        std::cerr << "help: tdlist --help to list commands" << std::endl;
    }
}
