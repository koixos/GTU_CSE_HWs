#include "Course.h"

namespace PA4 {
    Course::Course() { }

    Course::Course(const std::string& _name, const int _code)
        : name(_name), code(_code) { }

    const void Course::findStudents(Student *ptr, const int stuNum) const {
        for (int i = 1; i < stuNum+1; i++) {
            for (int j = 1; j < ptr[i].getCouNum()+1; j++) {
                // listing the students who take the course
                if (this->code == ptr[i].getCourseRef()[j].getCode()) {
                    std::cout
                        << ptr[i].getName() << " "
                        << ptr[i].getSurname() << " "
                        << ptr[i].getId() << std::endl;
                }
            }
        }
    }

    const std::string Course::getName() const { return name; }

    const int Course::getCode() const { return code; }

    const void Course::setName(const std::string &_name) { this->name = _name; }

    const void Course::setCode(const int _code) { this->code = _code; }

    Course::~Course() { }
}