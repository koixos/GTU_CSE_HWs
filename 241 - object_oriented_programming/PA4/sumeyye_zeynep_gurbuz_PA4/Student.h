#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>
#include "Course.h"

namespace PA4 {
    class Course; // Forward declaration of Course class

    class Student {
        private:
            std::string name = "";
            std::string surname = "";
            std::string id = "";
            Course *courseRefs = nullptr;
            int couNum = 0;
        public:
            Student();
            Student(const std::string &, const std::string &, const std::string &);

            const void addCourse(const Course &); // Function to add a course to a student
            const void dropCourse(const int); // Function to drop a course from a student

            const std::string getName() const;            
            const std::string getSurname() const;
            const std::string getId() const;
            Course * getCourseRef();
            const int getCouNum() const;
            
            void setName(const std::string &);
            void setSurname(const std::string &);
            void setId(const std::string &);
            void setCourseRef(Course *);

            ~Student();
    };   
}

#endif  //STUDENT_H