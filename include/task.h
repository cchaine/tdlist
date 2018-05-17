#ifndef TASK_H
#define TASK_H

#include <string>

enum TYPE {
    IMPROVEMENT,
    FEATURE,
    ERROR,
    UNTYPED
};

class Task {
public:
    Task(std::string title);
    Task(std::string title, TYPE type);
    ~Task();
    bool status();
    void setStatus(bool value);
    std::string title();
    void setTitle(std::string title);
private:
    std::string m_title;
    bool m_status = 0;
    TYPE m_type = UNTYPED;
};

#endif
