#include "task.h"

Task::Task(std::string title) {
    this->m_title = title;
}

Task::Task(std::string title, TYPE type) {
    this->m_title = title;
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
