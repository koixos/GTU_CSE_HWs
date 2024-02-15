#ifndef COURSE_H
#define COURSE_H

#include <iostream>
#include <string>
#include "Student.h"

namespace PA4 {
    class Student; // forward declaration of Student class

    class Course {
        private:
            std::string name = "";
            int code = -1;
            Student *studentRefs = nullptr;
        public:
            Course();
            Course(const std::string &, const int);

            const void findStudents(Student *, const int) const; // Function to find & list students who takes that class

            const std::string getName() const;
            const int getCode() const;
            
            const void setName(const std::string &);
            const void setCode(const int);
            
            ~Course();
    };   
}

#endif  //COURSE_H