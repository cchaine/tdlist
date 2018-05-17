#ifndef TASK_H
#define TASK_H

#include <string>

class Task {
public:
    Task(std::string title, int hash);
    Task(std::string title, int hash, char type);
    Task(std::string title, int hash, bool status, char type);
    ~Task();
    bool status();
    void setStatus(bool value);
    std::string title();
    void setTitle(std::string title);
    int hash();
    char type();
    void setType(char type);
private:
    std::string m_title;
    bool m_status = 0;
    char m_type = 'u';
    int m_hash;
};

#endif
