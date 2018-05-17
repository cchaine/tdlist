#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "termcolor.hpp"

std::fstream file;
std::vector<std::string> * tasks;

void printUsage(std::string msg) {
    std::cout << termcolor::yellow << "usage: " << termcolor::reset << msg << std::endl;
}

void printFatal(std::string msg) {
    std::cout << termcolor::red << "fatal: " << termcolor::reset << msg << std::endl;
}

void printInfo(std::string msg) {
    std::cout << termcolor::blue << "info: " << termcolor::reset << msg << std::endl;
}

void load() {
    file.open(".tdlist", std::ios::in);
    std::string line;
    while(std::getline(file, line)){
        tasks->push_back(line);
    }
    file.close();
}

std::string getHashValue() {
    std::string value = tasks->at(0).substr(tasks->at(0).find("|")+1);
    tasks->at(0) = "tdlist|" + std::to_string(std::stoi(value)+1);
    return value;
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
       printFatal("tdlist project already initialized");
       exit(-1);
   }
   file.open(".tdlist", std::ios::out);
   file << "tdlist|1" << std::endl;
   file.close();
}

void checkInit() {
    if(tasks->size() == 0){
        printFatal("not a tdlist project directory");
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
        tasks->push_back(title + "::0u" + "|" + getHashValue());
    } else if(tasks->at(find(title))[tasks->at(find(title)).find("::") + 2] == '1') {
        tasks->erase(tasks->begin() + find(title));
        tasks->push_back(title + "::0u" + "|" + getHashValue());
    } else {
        printFatal(title + " already exists");
        exit(-1);
    }
    write();
}

void open(std::string title, char type) {
    load();
    checkInit();
    if(find(title) == -1) {
        tasks->push_back(title + "::0" + type + "|" + getHashValue());
    } else if(tasks->at(find(title))[tasks->at(find(title)).find("::") + 2] == '1') {
        tasks->erase(tasks->begin() + find(title));
        tasks->push_back(title + "::0" + type + "|" + getHashValue());
    } else {
        printFatal(title + " already exist");
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
        printInfo("removed " + title);
    } else {
        printFatal(title + " doesn't exist");
    }
    write();
}

void rm(std::vector<std::string> titles[]) {
    load();
    checkInit();
    bool allExists = true;
    for(int i = 0; i < titles->size(); i++) {
        if(find(titles->at(i)) == -1) {
            allExists = false;
            printFatal(titles->at(i) + " doesn't exist");
        }
    }
    if(allExists) {
        for(int i = 0; i < titles->size(); i++) {
            tasks->erase(tasks->begin() + find(titles->at(i)));
            printInfo("removed " + titles->at(i));
        }
    } else {
        exit(-1);
    }
    write();
}

void list() {
    load();
    checkInit();
    if(tasks->size() == 1) {
        printInfo("list empty");
    }

    bool anyOpened = false;
    for(int i = 1; i < tasks->size(); i++) {
        int divider = tasks->at(i).find("::") + 2;
        if(!strcmp(tasks->at(i).substr(divider, 1).c_str(), "0")) {
            anyOpened = true;
        }
    }
    if(anyOpened) {
        std::cout << "Opened tasks:" << std::endl;
        std::cout << " (use \"tdlist open (-<type>) <name>\" to open a new task)" << std::endl << std::endl;
    }
    //errors
    for(int i = 1; i < tasks->size(); i++) {
        int divider = tasks->at(i).find("::") + 2;
        if(!strcmp(tasks->at(i).substr(divider, 2).c_str(), "0e")) {
            std::cout << termcolor::red << termcolor::dark << termcolor::bold << "#" << tasks->at(i).substr(tasks->at(i).find("|")+1) << termcolor::reset << termcolor::red << "\t" << tasks->at(i).substr(0, divider-2) << termcolor::reset << std::endl;
        }
    }
    //improvements
    for(int i = 1; i < tasks->size(); i++){
        int divider = tasks->at(i).find("::") + 2;
        if(!strcmp(tasks->at(i).substr(divider, 2).c_str(), "0i")) {
            std::cout << termcolor::green << termcolor::dark << termcolor::bold << "#" << tasks->at(i).substr(tasks->at(i).find("|")+1) << termcolor::reset << termcolor::green << "\t" << tasks->at(i).substr(0, divider-2) << termcolor::reset << std::endl;
        }
    }
    //features
    for(int i = 1; i < tasks->size(); i++){
        int divider = tasks->at(i).find("::") + 2;
        if(!strcmp(tasks->at(i).substr(divider, 2).c_str(), "0f")) {
            std::cout << termcolor::blue << termcolor::dark << termcolor::bold << "#" << tasks->at(i).substr(tasks->at(i).find("|")+1) << termcolor::reset << termcolor::blue << "\t" << tasks->at(i).substr(0, divider-2) << termcolor::reset << std::endl;
        }
    }
    for(int i = 1; i < tasks->size(); i++) {
        int divider = tasks->at(i).find("::") + 2;
        if(!strcmp(tasks->at(i).substr(divider, 2).c_str(), "0u")) {
            std::cout << termcolor::white << termcolor::dark << termcolor::bold << "#" << tasks->at(i).substr(tasks->at(i).find("|")+1) << termcolor::reset << termcolor::white << "\t" << tasks->at(i).substr(0, divider-2) << termcolor::reset << std::endl;
        }
    }

    bool anyClosed = false;
    for(int i = 1; i < tasks->size(); i++) {
        int divider = tasks->at(i).find("::") + 2;
        if(!strcmp(tasks->at(i).substr(divider, 1).c_str(), "1")) {
            anyClosed = true;
        }
    }
    if(anyClosed) {
        std::cout << std::endl << "Closed tasks:" << std::endl;
        std::cout << " (use \"tdlist close <name>\" to close a task)" << std::endl << std::endl;
    }
    for(int i = 1; i < tasks->size(); i++){
        int divider = tasks->at(i).find("::") + 2;
        if(!strcmp(tasks->at(i).substr(divider, 1).c_str(), "1")) {
            std::cout << termcolor::grey << termcolor::dark << termcolor::bold << "#" << tasks->at(i).substr(tasks->at(i).find("|")+1) << termcolor::reset << termcolor::grey << "\t" << tasks->at(i).substr(0, divider-2) << termcolor::reset << std::endl;
        }
    }
    std::cout << std::endl;
}

std::string updateTaskParam(std::string title, int paramIndex, char value) {
    int taskIndex = find(title);
    std::string newTask = title;
    std::string task = tasks->at(taskIndex);
    int divider = task.find("::") + 2;
    newTask += "::" + task.substr(divider, paramIndex) + value + task.substr(divider + paramIndex + 1, task.find("|"));
    return newTask;
}

void close(std::string title) {
    load();
    checkInit();
    int taskIndex = find(title);
    if(taskIndex != -1) {
        tasks->at(taskIndex) = updateTaskParam(title, 0, '1');
        printInfo(title + " closed");
    } else {
        printFatal(title + " doesn't exists");
        exit(-1);
    }
    write();
}

void type(std::string title, char type) {
    load();
    checkInit();
    int taskIndex = find(title);
    if(taskIndex != -1 && (type == 'u' || type == 'i' || type == 'e' || type == 'f')) {
        tasks->at(taskIndex) = updateTaskParam(title, 1, type);
        printInfo("switched type for " + title);
    } else if(type != 'u' && type != 'i' && type != 'e' && type != 'f'){
        printFatal("unknown type:");
        std::cout << "\t" << "e error" << std::endl;
        std::cout << "\t" << "i improvement" << std::endl;
        std::cout << "\t" << "f feature" << std::endl;
        std::cout << "\t" << "u untyped" << std::endl << std::endl;
    } else {
        printFatal(title + " doesn't exists");
    }
    write();
}


int main(int argc, char * argv[]) {
    tasks = new std::vector<std::string>();
    
    if(argc == 1) {
        list();

    } else if(!strcmp(argv[1], "init")) {
        init();

    } else if(!strcmp(argv[1], "open")){
        if(argc == 3){
            open(argv[2]);
        } else if(argc == 4) {
            open(argv[3], argv[2][1]);
        } else {
            printUsage("tdlist open (-<type>) <name>");
            exit(-1);
        }

    } else if(!strcmp(argv[1], "rm")){
        if(argc == 3){
            rm(argv[2]);
        } else if(argc > 3) {
            std::vector<std::string> * titles = new std::vector<std::string>();
            for(int i = 0; i < argc-2; i++) {
                titles->push_back(argv[i+2]);
            }
            rm(titles);
            delete(titles);
        } else {
            printUsage("tdlist rm <name>");
            exit(-1);
        }

    } else if(!strcmp(argv[1], "close")) {
       if(argc == 3) {
            close(argv[2]);
       } else {
            printUsage("tdlist close <name>");
            exit(-1);
       }
     
    } else if(!strcmp(argv[1], "open")) {
        if(argc == 3) {
            open(argv[2]);
        } else {
            printUsage("tdlist open <name>");
        }

    } else if(!strcmp(argv[1], "type")) {
        if(argc == 4) {
            type(argv[2], argv[3][0]);
        } else {
            printUsage("tdlist type <name> <type>");
        }
    } else {
        printUsage("tdlist --help to list commands");
    }
}
