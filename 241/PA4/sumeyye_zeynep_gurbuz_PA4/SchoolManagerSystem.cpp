#include "SchoolManagerSystem.h"

namespace PA4 {
    SchoolManagerSystem::SchoolManagerSystem() { 
        this->stuRefs = new Student; // initializing ptr.s
        this->couRefs = new Course;
    }

    const void SchoolManagerSystem::run() {
        std::string choice = "";
        std::cout << "Main_menu" << std::endl;
        while (true) {
            SchoolManagerSystem::mainMenu();
            std::cin >> choice;
            if (SchoolManagerSystem::toInt(choice) == 0)
                return;
            if (SchoolManagerSystem::toInt(choice) == 1)
                SchoolManagerSystem::genStuOps();
            else if (SchoolManagerSystem::toInt(choice) == 2)
                SchoolManagerSystem::genCouOps();
            else if (SchoolManagerSystem::toInt(choice) == 3)
                SchoolManagerSystem::listStudents();
            else if (SchoolManagerSystem::toInt(choice) == 4)
                SchoolManagerSystem::listCourses();
        }
    }

    const void SchoolManagerSystem::mainMenu() const {
        std::cout
            << "0 exit" << std::endl
            << "1 student" << std::endl
            << "2 course" << std::endl
            << "3 list_all_students" << std::endl
            << "4 list_all_courses" << std::endl
            << ">> ";
    }

    const void SchoolManagerSystem::genStuOps() {
        while (true) {
            std::string choice = "";
            std::cout
                << "0 up" << std::endl
                << "1 add_student" << std::endl
                << "2 select_student" << std::endl
                << ">> ";
            std::cin >> choice;
            if (SchoolManagerSystem::toInt(choice) == 0)
                return;
            if (SchoolManagerSystem::toInt(choice) == 1) {
                SchoolManagerSystem::addStudent();
            } else if (SchoolManagerSystem::toInt(choice) == 2) {
                int ind = SchoolManagerSystem::selectStudent();
                if (ind != -1) // if ind == -1 --> no student found
                    SchoolManagerSystem::stuOps(ind);
            }
        }
    }

    const void SchoolManagerSystem::genCouOps() {
        while (true) {
            std::string choice = "";
            std::cout
                << "0 up" << std::endl
                << "1 add_course" << std::endl
                << "2 select_course" << std::endl
                << ">> ";
            std::cin >> choice;
            if (SchoolManagerSystem::toInt(choice) == 0)
                return;
            if (SchoolManagerSystem::toInt(choice) == 1) {
                SchoolManagerSystem::addCourse();
            } else if (SchoolManagerSystem::toInt(choice) == 2) {
                int ind = SchoolManagerSystem::selectCourse();
                if (ind != -1) // if ind == -1 --> no course found
                    SchoolManagerSystem::couOps(ind);
            }
        }
    }

    const void SchoolManagerSystem::stuOps(const int ind) {
        while (true) {
            std::string choice = "";
            std::cout
                << "0 up" << std::endl
                << "1 delete_student" << std::endl
                << "3 add_selected_student_to_a_course" << std::endl
                << "4 drop_selected_student_from_a_course" << std::endl
                << ">> ";
            std::cin >> choice;
            if (SchoolManagerSystem::toInt(choice) == 0)
                return;
            if (SchoolManagerSystem::toInt(choice) == 1) {
                SchoolManagerSystem::deleteStudent(ind);
                return;
            } else if (SchoolManagerSystem::toInt(choice) == 3) {
                SchoolManagerSystem::addCourseToStudent(ind);
            } else if (SchoolManagerSystem::toInt(choice) == 4) {
                SchoolManagerSystem::dropCourseFromStudent(ind);
            }
        }
    }

    const void SchoolManagerSystem::couOps(const int ind) {
        while (true) {
            std::string choice = "";
            std::cout
                << "0 up" << std::endl
                << "1 delete_course" << std::endl
                << "2 list_students_registered_to_the_selected_course" << std::endl
                << ">> ";
            std::cin >> choice;
            if (SchoolManagerSystem::toInt(choice) == 0)
                return;
            if (SchoolManagerSystem::toInt(choice) == 1) {
                SchoolManagerSystem::deleteCourse(ind);
                return;
            } else if (SchoolManagerSystem::toInt(choice) == 2) {
                this->couRefs[ind].findStudents(this->stuRefs, this->stuNum);
            }
        }
    }

    const void SchoolManagerSystem::listStudents() const {
        for (int i = 1; i < this->stuNum+1; i++)
            std::cout
                << this->stuRefs[i].getName() << " "
                << this->stuRefs[i].getSurname() << " "
                << this->stuRefs[i].getId() << std::endl;
    }

    const void SchoolManagerSystem::listCourses() const {
        for (int i = 1; i < this->couNum+1; i++)
            std::cout
                << this->couRefs[i].getCode() << " " 
                << this->couRefs[i].getName() << std::endl;
    }

    const void SchoolManagerSystem::addCourseToStudent(const int ind) const {
        std::string name = "";
        for (int i = 1; i < this->couNum+1; i++) {
            bool flag = false; // flag to define if student already had the course 
            for (int j = 1; j < this->stuRefs[ind].getCouNum()+1; j++)
                if (this->couRefs[i].getName().compare(this->stuRefs[ind].getCourseRef()[j].getName()) == 0)
                    flag = true; // student is taking the course already
            if (!flag) // if the course is available for student, print
                std::cout
                    << this->couRefs[i].getCode() << " "
                    << this->couRefs[i].getName() << std::endl;
        }
        std::cin >> name;
        for (int i = 1; i < this->couNum+1; i++)
            if (this->couRefs[i].getName().compare(name) == 0) // finding the index of the given course
                this->stuRefs[ind].addCourse(this->couRefs[i]); // adding course to student 
    }

    const void SchoolManagerSystem::dropCourseFromStudent(const int ind) const {
        std::string name = "";
        for (int i = 1; i < this->stuRefs[ind].getCouNum()+1; i++) // Listing all courses student has
            std::cout
                << this->stuRefs[ind].getCourseRef()[i].getCode() << " "
                << this->stuRefs[ind].getCourseRef()[i].getName() << std::endl;
        std::cin >> name;
        for (int i = 1; i < this->stuRefs[ind].getCouNum()+1; i++)
            if (this->stuRefs[ind].getCourseRef()[i].getName().compare(name) == 0) // finding the index of the given course
                this->stuRefs[ind].dropCourse(i); // dropping course from student
    }

    const void SchoolManagerSystem::addStudent() {
        std::string name = "", surname = "", id = "";
        std::cout << ">> ";
        std::cin >> name >> surname >> id;
        this->setStuRefs(name, surname, id);
    }

    const void SchoolManagerSystem::addCourse() {
        std::string name = "";
        std::cout << ">> ";
        std::cin >> name;
        this->setCouRefs(name);
    }

    const void SchoolManagerSystem::deleteStudent(const int ind) {
        Student *temp = new Student[stuNum]; // creating a temp ptr with decreased size
        for (int i = 1; i < ind; i++)
            temp[i] = this->stuRefs[i]; // filling temp ptr with existing students till given index
        for (int i = ind+1; i < this->stuNum+1; i++)
            temp[i-1] = this->stuRefs[i]; // jumping given index & filling temp ptr with remained students
        this->stuRefs = temp; // making main ptr to reference head of temp ptr
        --this->stuNum; // decreasing size of the ptr
        delete[] temp; // destructing temp ptr
        temp = nullptr;
    }

    const void SchoolManagerSystem::deleteCourse(const int ind) {
        Course *temp = new Course[couNum]; // creating a temp ptr with decreased size
        temp[0] = this->couRefs[0];
        for (int i = 1; i < ind; i++) 
            temp[i] = this->couRefs[i]; // filling temp ptr with existing courses till given index
        for (int i = ind+1; i < this->couNum+1; i++) {
            temp[i-1] = this->couRefs[i]; // jumping given index & filling temp ptr with remained courses
            temp[i-1].setCode(i-1); // changing codes of the courses
        }
        this->couRefs = temp; // making main ptr to reference head of temp ptr
        --this->couNum; // decreasing size of the ptr
        delete[] temp; // destructing temp ptr
        temp = nullptr;
    }

    const int SchoolManagerSystem::selectStudent() const {
        std::string name = "", surname = "", id = "";
        std::cout << ">> ";
        std::cin >> name >> surname >> id;
        for (int i = 1; i < this->stuNum+1; i++)
            if (this->stuRefs[i].getId().compare(id) == 0)
                return i; // returns index of the selected student
        return -1; // no student found
    }

    const int SchoolManagerSystem::selectCourse() const {
        std::string name = "";
        std::cout << ">> ";
        std::cin >> name;
        for (int i = 1; i < this->couNum+1; i++)
            if (this->couRefs[i].getName().compare(name) == 0)
                return i; // returns index of the selected course
        return -1; // no course found
    }

    // Function to turn a string into its integer form
    const long SchoolManagerSystem::toInt(const std::string &str) const {
        int num = 0;
        for (int i = 0; i < str.length(); i++)
            num += ((str[str.length()-i-1]-48)*pow(10,i)); // Turning each string character to its integer form
        return num; // returning whole number as integer
    }

    const Student * SchoolManagerSystem::getStuRefs() const { return this->stuRefs; }

    const Course * SchoolManagerSystem::getCourseRefs() const { return this->couRefs; }
    
    const void SchoolManagerSystem::setStuRefs(const std::string &_name, const std::string &_surname, const std::string &_id) {
        ++stuNum; // increasing the size of the ptr
        Student *temp = new Student[stuNum+1]; // creating a temp ptr with the increasized size
        for (int i = 0; i < stuNum; i++)
            temp[i] = this->stuRefs[i]; // filling temp ptr with existing students
        temp[stuNum] = Student(_name, _surname, _id); // the last element of temp ptr = new value
        this->stuRefs = temp; // making main ptr to reference head of temp ptr
        delete[] temp; // destructing temp ptr
        temp = nullptr;
    }

    const void SchoolManagerSystem::setCouRefs(const std::string &_name) { 
        ++couNum; // increasing the size of the ptr
        Course *temp = new Course[couNum+1]; // creating a temp ptr with the increasized size
        for (int i = 0; i < couNum; i++)
            temp[i] = this->couRefs[i]; // filling temp ptr with existing courses
        temp[couNum] = Course(_name, couNum); // the last element of temp ptr = new value
        this->couRefs = temp; // making main ptr to reference head of temp ptr
        delete[] temp; // destructing temp ptr
        temp = nullptr;
    }

    SchoolManagerSystem::~SchoolManagerSystem() {
        delete this->stuRefs;
        delete this->couRefs;
        this->stuRefs = nullptr;
        this->couRefs = nullptr;
    }        
}