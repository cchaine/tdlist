#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "termcolor.hpp"
#include "task.h"

std::fstream file;
std::vector<Task> tasks;
int hashValue = -1;

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
    
    std::getline(file, line);
    if(strlen(line.c_str()) > 0){
        hashValue = stoi(line.substr(line.find("|")+1));
    }

    while(std::getline(file, line)){
        std::string title = line.substr(0, line.find("::"));
        bool status = stoi(line.substr(line.find("::")+2, 1));
        char type = line[line.find("::")+3];
        int hash = stoi(line.substr(line.find("|") + 1));
        tasks.push_back(Task(title, hash, status, type));
    }
    file.close();
}

void write() {
    file.open(".tdlist", std::ios::out);
    file << "tdlist|" + std::to_string(hashValue) << std::endl;
    for(int i = 0; i < tasks.size(); i++) {
        file << tasks[i].title() + "::" + std::to_string(tasks[i].status()) + tasks[i].type() + "|" + std::to_string(tasks[i].hash()) << std::endl;
    }
    file.close();
}

void init() {
   load();
   if(tasks.size() != 0){
       printFatal("tdlist project already initialized");
       exit(-1);
   }
   file.open(".tdlist", std::ios::out);
   file << "tdlist|1" << std::endl;
   file.close();
}

void checkInit() {
    if(hashValue == -1){
        printFatal("not a tdlist project directory");
        exit(-1);
    }
}

int find(std::string task) {
    for(int i = 0; i < tasks.size(); i++) {
        if(!strcmp(tasks[i].title().c_str(), task.c_str())){
            return i;
        } else {
            if(!strcmp(std::to_string(tasks[i].hash()).c_str(), task.c_str())) {
                return i;
            }
        }
    }
    return -1;
}

void open(std::string title, char type) {
    load();
    checkInit();

    if(title.find_first_not_of("0123456789") == std::string::npos) {
        printFatal("task title cannot be only digits");
        exit(-1);
    }

    if(find(title) == -1) {
        tasks.push_back(Task(title, hashValue, type));
        hashValue++;
    } else if(tasks[find(title)].status()) {
        tasks.erase(tasks.begin() + find(title));
        tasks.push_back(Task(title, hashValue, type));
        hashValue++;
    } else {
        printFatal(title + " already exist");
        exit(-1);
    }
    write();
}

void open(std::string title) {
    open(title, 'u');
}

void rm(std::vector<std::string> *titles) {
    load();
    checkInit();
    
    bool allExists = true;
    for(int i = 0; i < titles->size(); i++) {
        if(find((*titles)[i]) == -1) {
            allExists = false;
            printFatal((*titles)[i] + " doesn't exist");
        }
    }

    if(allExists) {
        for(int i = 0; i < titles->size(); i++) {
            tasks.erase(tasks.begin() + find((*titles)[i]));
            printInfo("removed " + (*titles)[i]);
        }
    } else {
        exit(-1);
    }
    write();
}

void rm(std::string title) {
    std::vector<std::string> titles = std::vector<std::string>();
    titles.push_back(title);
    rm(&titles);
}

void list() {
    load();
    checkInit();
    if(hashValue != -1 && tasks.size() == 0) {
        printInfo("list empty");
    }

    bool anyOpened = false;
    for(int i = 0; i < tasks.size(); i++) {
        if(!tasks[i].status()) {
            anyOpened = true;
        }
    }
    if(anyOpened) {
        std::cout << "Opened tasks:" << std::endl;
        std::cout << " (use \"tdlist open (-<type>) <name>\" to open a new task)" << std::endl << std::endl;
    }

    for(int i = 0; i < tasks.size(); i++) {
        if(!tasks[i].status()) {
            if(tasks[i].type() == 'e') {
                std::cout << termcolor::red << termcolor::dark << termcolor::bold << "#" << tasks[i].hash() << termcolor::reset << termcolor::red << "\t" << tasks[i].title() << termcolor::reset << std::endl;
            } else if(tasks[i].type() == 'i') {
                std::cout << termcolor::green << termcolor::dark << termcolor::bold << "#" << tasks[i].hash() << termcolor::reset << termcolor::green << "\t" << tasks[i].title() << termcolor::reset << std::endl;
            } else if(tasks[i].type() == 'f') {
                std::cout << termcolor::blue<< termcolor::dark << termcolor::bold << "#" << tasks[i].hash() << termcolor::reset << termcolor::blue << "\t" << tasks[i].title() << termcolor::reset << std::endl;
            } else if(tasks[i].type() == 'u') {
                std::cout << termcolor::white << termcolor::dark << termcolor::bold << "#" << tasks[i].hash() << termcolor::reset << termcolor::white << "\t" << tasks[i].title() << termcolor::reset << std::endl;
            }
        }
    }

    bool anyClosed = false;
    for(int i = 0; i < tasks.size(); i++) {
        if(tasks[i].status()) {
            anyClosed = true;
        }
    }
    if(anyClosed) {
        std::cout << std::endl << "Closed tasks:" << std::endl;
        std::cout << " (use \"tdlist close <name>\" to close a task)" << std::endl << std::endl;
    }
    for(int i = 0; i < tasks.size(); i++) {
        if(tasks[i].status()) {
            std::cout << termcolor::grey << termcolor::dark << termcolor::bold << "#" << tasks[i].hash() << termcolor::reset << termcolor::grey << "\t" << tasks[i].title() << termcolor::reset << std::endl;
        }
    }
    std::cout << std::endl;
}

void close(std::string title) {
    load();
    checkInit();
    if(find(title) != -1) {
        tasks[find(title)].setStatus(1);
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
    if(find(title) != -1) {
        tasks[find(title)].setType(type);
        printInfo("switched type for " + title);
    } else {
        printFatal(title + " doesn't exists");
    }
    write();
}

int main(int argc, char * argv[]) {
    tasks = std::vector<Task>();

    if(argc == 1) {
        printUsage("tdlist <command> [<args>]");

    } else if(!strcmp(argv[1], "list")) {
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
            if(argv[2][0] != 'u' && argv[2][0] != 'i' && argv[2][0] != 'e' && argv[2][0] != 'f'){
                printFatal("unknown type:");
                std::cout << "\t" << "e error" << std::endl;
                std::cout << "\t" << "i improvement" << std::endl;
                std::cout << "\t" << "f feature" << std::endl;
                std::cout << "\t" << "u untyped" << std::endl << std::endl;
            }

            type(argv[3], argv[2][0]);
        } else {
            printUsage("tdlist type <type> <name>");
        }
    } else {
        printUsage("tdlist --help to list commands");
    }
}
