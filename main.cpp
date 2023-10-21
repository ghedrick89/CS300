/*

SNHU CS-300: 23EW1
Project Two: College Course Search Using a Binary Search Tree
By: Graham Durant Hedrick
Completed on: 10/15/2023

Overview: 
    - Program presents a menu of 4 options for the user to select.
    - Program asks for a reference to an external file from which to load data.
    - Program loads external data and loads in into the data structure.
    - Program displays accurate data loaded from the data structure depending 
      on the option selected by the user.
    - Program prompts the user to enter a course number and displays the course 
      information.

Notes:

I encountered a number of issues while writing this code. The first was a the
program terminating with an infinate loop when invalid input was entered. I was
able to correct this by impleneting a proper loop to the Main Menu when users 
entered invalid input. Another issue I encoutered was the program was returning 
user input as invalid when they entered a course number with lower case letters.
To correct this, I had to research and implement a function using the <cctype> 
library to allow for the acceptance of either typecase when entered by the user.

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <cctype>  // Addition required to account for typecase differences.

// The below function had to be added to account for the possibility of the user to enter
// a valid course ID, but with lower case letters. The purpose is to expand usability and
// prevent input being flagged as invalid desipte being technically correct.

std::string TypeCase(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        c = std::tolower(c);
    }
    return result;
}

// The 'Course' structure contains elements corresponding to the indetiufiable information of each course.
struct Course {
    std::string courseNum;
    std::string title;
    std::vector<std::string> prereqs;
};

// TreeNode for the Binary Search Tree (BST)
struct TreeNode {
    Course course;
    TreeNode* left;
    TreeNode* right;

    TreeNode(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// Binary Search Tree implementation
class BinarySearchTree {
private:
    TreeNode* root;

    TreeNode* insert(TreeNode* node, Course course);  //for insertion into structure
    void inorder(TreeNode* node);//for displaying data elements in order
    TreeNode* search(TreeNode* node, const std::string& courseNum); //for search within data structure

public:
    BinarySearchTree() : root(nullptr) {}
    void insert(Course course);// Insertion of a course into the strucutre
    void DisplayCourses();// Displays all courses in order
    Course* searchCourse(const std::string& courseNumber);// Search for a course by ID number
};

// The insertion method called
TreeNode* BinarySearchTree::insert(TreeNode* node, Course course) {
    if (!node) {
        return new TreeNode(course);
    }

// Comparison of courses by course number.
// Added calls to TypeCase funtion after realizing failed results could occur
// if the user did not capitalize all letter in the Course Number designation.
    if (TypeCase(course.courseNum) < TypeCase(node->course.courseNum)) {
        node->left = insert(node->left, course);
    } else if (TypeCase(course.courseNum) > TypeCase(node->course.courseNum)) {
        node->right = insert(node->right, course);
    }

    return node;
}

//For insertion of a course into the data structure
void BinarySearchTree::insert(Course course) {
    root = insert(root, course);
}

// Recursive inorder traversal (prints courses in alphanumeric order)
void BinarySearchTree::inorder(TreeNode* node) {
    if (!node) return;

    inorder(node->left);
    std::cout << node->course.courseNum << " - " << node->course.title << std::endl;
    inorder(node->right);
}

// Display courses in designated order
void BinarySearchTree::DisplayCourses() {
    inorder(root);
}

// Searches the tree for courses by their ID number
TreeNode* BinarySearchTree::search(TreeNode* node, const std::string& courseNumber) {
    if (!node) return nullptr;

    if (TypeCase(courseNumber) == TypeCase(node->course.courseNum)) {
        return node;
    } else if (TypeCase(courseNumber) < TypeCase(node->course.courseNum)) {
        return search(node->left, courseNumber);
    } else {
        return search(node->right, courseNumber);
    }
}

// Implements a search for object by designated ID number:
Course* BinarySearchTree::searchCourse(const std::string& courseNumber) {
    TreeNode* node = search(root, courseNumber);
    if (node) return &(node->course);
    else return nullptr;
}

// The CourseData Function loads data from the external file into the Binary Search Tree:
void CourseData(BinarySearchTree& tree) {
    std::string DataFile;
    std::cout << "Enter the name of the file to load: ";
    std::cin >> DataFile;
    std::ifstream file(DataFile);

    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            std::stringstream ss(line);
            Course course;
            getline(ss, course.courseNum, ',');
            getline(ss, course.title, ',');

            std::string prereq;
            while (getline(ss, prereq, ',')) {
                course.prereqs.push_back(prereq);
            }

            tree.insert(course);
        }

        file.close();
        std::cout << "Data File Found!" << std::endl;
    } else {
        std::cout << "Data File Not Found!" << std::endl;
    }
}

// Function to display course infromation when loaded from the text file.
void DisplayCourse(BinarySearchTree& tree) {
    std::string courseNumber;
    std::cout << "Enter the course ID number: ";
    std::cin >> courseNumber;

    Course* course = tree.searchCourse(courseNumber);
    if (course) {
        std::cout << "\nCourse Number: " << course->courseNum << std::endl;
        std::cout << "Title: " << course->title << std::endl;
        std::cout << "Prerequisites:";
        for (const std::string& prereq : course->prereqs) {
            std::cout << " " << prereq;
        }
        std::cout << std::endl;
    } else {
        std::cout << "Course not found!" << std::endl; // For invlaid course entry
    }
}
// Main function, displays the Main Menu with options 1-4.
int main() {
    BinarySearchTree tree;
    int choice;

    do {
        std::cout << "\n-------Main Menu-------\n" << std::endl;
        std::cout << "(1) Load Data from File" << std::endl;
        std::cout << "(2) Display Course List" << std::endl;
        std::cout << "(3) Search Course" << std::endl;
        std::cout << "(4) Exit" << std::endl;
        std::cout << "\nChoose an option: ";

// First checks to see if loading data from the file named is succesful.
        if(!(std::cin >> choice)) {
            std::cin.clear(); // Clears the error state if the data loading process is successful.
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
// Displays if the data could not be loaded and loops Main Menu.
            std::cout << "Invalid option: Please enter: 1, 2, 3, or 4." << std::endl;
            continue;
        }
      
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
// Cases corresponding to options selected.
        switch (choice) {
            case 1:
                CourseData(tree);
                break;
            case 2:
                tree.DisplayCourses();
                break;
            case 3:
                DisplayCourse(tree);
                break;
            case 4:
                std::cout << "\nGoodbye!" << std::endl;
                break;
            default:
                std::cout << "\nInvalid Input!" << std::endl;
        }
    } while (choice != 4);

    return 0;
}