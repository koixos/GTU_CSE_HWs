#include "Student.h"

namespace PA4 {
    Student::Student() { courseRefs = new Course; }

    Student::Student(const std::string &_name, const std::string &_surname, const std::string &_id)
        : name(_name), surname(_surname), id(_id) { courseRefs = new Course; }

    const void Student::addCourse(const Course &course) {
        ++this->couNum; // increasing the size of the ptr
        Course *temp = new Course[couNum+1]; // creating a temp ptr with the increasized size
        for (int i = 0; i < couNum; i++) 
            temp[i] = this->courseRefs[i]; // filling temp ptr with existing courses
        temp[couNum] = course; // the last element of temp ptr = given value
        this->courseRefs = temp; // making main ptr to reference head of temp ptr
        delete[] temp; // destructing temp ptr
        temp = nullptr;
    }

    const void Student::dropCourse(const int ind) {
        Course *temp = new Course[couNum]; // creating a temp ptr with decreased size
        temp[0] = this->courseRefs[0]; 
        for (int i = 1; i < ind; i++)
            temp[i] = this->courseRefs[i]; // filling temp ptr with existing courses till given index
        for (int i = ind+1; i < this->couNum+1; i++)
            temp[i-1] = this->courseRefs[i]; // jumping given index & filling temp ptr with remained courses
        this->courseRefs = temp; // making main ptr to reference head of temp ptr
        --this->couNum; // decreasing size of the ptr
        delete[] temp; // destructing temp ptr
        temp = nullptr;
    }

    const std::string Student::getName() const { return name; }

    const std::string Student::getSurname() const { return surname; }

    const std::string Student::getId() const { return id; }

    Course * Student::getCourseRef() { return courseRefs; }

    const int Student::getCouNum() const { return couNum; }

    void Student::setName(const std::string &_name) { this->name = _name; }

    void Student::setSurname(const std::string &_surname) { this->surname = _surname; }

    void Student::setId(const std::string &_id) { this->id = id; }

    void Student::setCourseRef(Course *ptr) { this->courseRefs = ptr; }

    Student::~Student() { }
}
