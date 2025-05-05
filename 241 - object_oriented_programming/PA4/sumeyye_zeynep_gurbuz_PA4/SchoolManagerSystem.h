#ifndef SCHOOL_MANAGER_SYSTEM_H
#define SCHOOL_MANAGER_SYSTEM_H

#include <iostream>
#include <math.h> // only pow() will be used in toInt()
#include "Student.h"
#include "Course.h"

namespace PA4 {
    class SchoolManagerSystem {
        private:
            Student *stuRefs = nullptr;
            Course *couRefs = nullptr;
            int stuNum = 0;
            int couNum = 0;
        public:
            SchoolManagerSystem();

            const void run(); // Function to run the program
            const void mainMenu() const; // Function to show main menu
            const void genStuOps(); // Function to show general student operations menu
            const void genCouOps(); // Function to show general course operations menu
            const void stuOps(const int); // Function to show available operations of a spesific student
            const void couOps(const int); // Function to show available operations of a spesific course
            const void listStudents() const; // Function to list all the students
            const void listCourses() const; // Function to list all the courses
            const void addCourseToStudent(const int) const; // Function to add a course to a student
            const void dropCourseFromStudent(const int) const; // Function to drop a course from a student
            const void addStudent(); // Function to add a student to the system
            const void addCourse(); // Function to add a class to the system
            const void deleteStudent(const int); // Function to delete a student from the system
            const void deleteCourse(const int); // Function to delete a course from the system
            const int selectStudent() const; // Function to select a student & return its index
            const int selectCourse() const; // Function to select a course & return its index
            const long toInt(const std::string &) const; // Function to return a string to integer

            const Student * getStuRefs() const;
            const Course * getCourseRefs() const;

            const void setStuRefs(const std::string &, const std::string &, const std::string &);
            const void setCouRefs(const std::string &);

            ~SchoolManagerSystem();
    };   
}

#endif  //SCHOOL_MANAGER_SYSTEM_H