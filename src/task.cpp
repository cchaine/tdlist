#include "task.h"
#include <iostream>

Task::Task(std::string title, int hash) {
    this->m_title = title;
    this->m_hash = hash;
}

Task::Task(std::string title, int hash, char type) {
    this->m_title = title;
    this->m_hash = hash;
    this->m_type = type;
}

Task::Task(std::string title, int hash, bool status, char type) {
    this->m_title = title;
    this->m_hash = hash;
    this->m_status = status;
    this->m_type = type;
}

Task::~Task() {

}

bool Task::status() {
    return this->m_status;
}

void Task::setStatus(bool value) {
    this->m_status = value;
}

std::string Task::title() {
    return this->m_title;
}

void Task::setTitle(std::string title) {
    this->m_title = title;
}

int Task::hash() {
    return this->m_hash;
}

char Task::type() {
    return this->m_type;
}

void Task::setType(char type) {
    this->m_type = type;
}
