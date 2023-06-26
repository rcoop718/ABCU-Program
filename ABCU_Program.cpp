//============================================================================
// Name        : ABCU_Program.cpp
// Author      : Ryan Cooper
// Version     : 1.0
// Description : Course Scheduler
//============================================================================

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>


using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct Course {
    string courseNumber; // unique identifier
    string courseName;
    std::vector<string> prerequisiteCourse;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * A class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseNumber);
};

/**
*Default constructor
*/
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}

/**
*Traverse the tree in order
*/
void BinarySearchTree::InOrder() {
    inOrder(root);
}

/**
 * Insert a course to bst
 */
void BinarySearchTree::Insert(Course course) {

    // if root equal to nullptr
    if (root == nullptr) {

        // root is equal to new node course
        root = new Node(course);
    }
    // else
    else {

        // add Node root and course
        this->addNode(root, course);
    }
}

/**
 * Searches for a course in the binary search tree based on the given course number.
 *
 * @param courseNumber The course number to search for.
 * @return A Course object representing the found course. If the course is not found, an empty Course object is returned.
 */
Course BinarySearchTree::Search(string courseNumber) {

    // set current node equal to root
    Node* cur = root;

    // keep looping downwards until bottom reached or matching courseNumber is found
    while (cur != nullptr) {

        // if match found, return current course
        if (cur->course.courseNumber.compare(courseNumber) == 0) {
            return cur->course;
        }

        // if course is smaller than current node then traverse left
        else if (courseNumber.compare(cur->course.courseNumber) < 0) {
            cur = cur->left;
        }

        // else it is larger so traverse right
        else {
            cur = cur->right;
        }
    }
    Course course;
    return course;
}

/**
 * Adds a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {

    // if node is larger then add to left
    if (node != nullptr && node->course.courseNumber.compare(course.courseNumber) > 0) {

        // if no left node
        if (node->left == nullptr) {

            // this node becomes left
            node->left = new Node(course);
            return;
        }
        // else recurse down the left node
        else {
            this->addNode(node->left, course);
        }
    }
    else {

        // if no right node
        if (node->right == nullptr) {

            // this node becomes right
            node->right = new Node(course);
            return;
        }
        else {

            // recurse down the right node
            this->addNode(node->right, course);
        }
    }
}

/**
 * Perform an in-order traversal of the binary search tree starting from the given node.
 *
 * @param node A pointer to the starting node of the traversal.
 */
void BinarySearchTree::inOrder(Node* node) {

  //if node is not equal to nullptr
    if (node != nullptr) {

        //InOrder not left
        inOrder(node->left);

        //output courseNumber and courseName
        cout << node->course.courseNumber << ", " << node->course.courseName << endl;

        //InOrder right
        inOrder(node->right);
    }
}

/**
*Display the course information to the console(std::out)
*
* @param course struct containing the course info
*/
void displayCourse(Course course) {

    // Prints course number and course name for a course
    cout << course.courseNumber << ", " << course.courseName << endl;

    cout << "Prerequisites: ";

    // Prints None for prerequisites if there are noe for the course
    if (course.prerequisiteCourse.size() == 0) {
        cout << "None" << endl;
    }
    // Else, it prints each prerequisites
    else {
        // Prints all prerequisite courses for a course
        for (unsigned int i = 0; i < course.prerequisiteCourse.size(); ++i) {

            if (i == course.prerequisiteCourse.size() - 1) {
                cout << course.prerequisiteCourse[i] << endl;
            }
            else {
                cout << course.prerequisiteCourse[i] << ", ";
            }
        }
        return;
    }
}


/**
*Checks that a course number is in the CSV file
*
* @param csvPath The path to the CSV file containing the course data.
* @param courseNumber The course number for which to check the prerequisites.
* @return true if all prerequisites exist, false otherwise.
*/
bool checkPrerequisiteExist(const string& csvPath, const string& courseNumber) {
    
    ifstream file(csvPath);

    // If the file can not be opened, return false
    if (!file) {
        cout << "Error: Failed to open CSV file to do a prerequisite check: " << csvPath << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string curCourseNumber;

        getline(ss, curCourseNumber, ',');

        if (curCourseNumber == courseNumber) {
            return true;  // Course number found in the CSV file
        }
    }

    // The prerequisite course is not in the file
    return false;
}

/**
 * Loads course data from a CSV file and input to a binary search tree.
 *
 * @param csvPath The file path of the CSV file containing the course data.
 * @param bst A pointer to the BinarySearchTree object to store the courses.
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {

    try {
        // Opens the CSV file
        ifstream file(csvPath);
        if (!file.is_open()) {
            throw runtime_error("Failed to open CSV file: " + csvPath);
        }

        string line;
        // Reads the lines of the CSV file
        while (getline(file, line)) {
            // Create a stringstream from the line
            stringstream ss(line);

            // Reads the course number
            string courseNumber;
            if (!getline(ss, courseNumber, ',')) {
                throw runtime_error("Failed to read course number");
            }

            // Reads the course name
            string courseName;
            if (!getline(ss, courseName, ',')) {
                throw runtime_error("Failed to read course name");
            }

            // Creates a Course object and populate its fields
            Course course;
            course.courseNumber = courseNumber;
            course.courseName = courseName;

            // Reads the prerequisite course numbers
            string prerequisiteCourse;
            while (getline(ss, prerequisiteCourse, ',')) {

                if (checkPrerequisiteExist(csvPath, prerequisiteCourse) == true) {
                    course.prerequisiteCourse.push_back(prerequisiteCourse);
               }
            }

            // Insert the course into the binary search tree
            bst->Insert(course);
        }

        // Close the file
        file.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}


int main()
{

    // A binary search tree to hold courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    string csvPath, courseNumber;

    cout << "Welcome to the course planner." << endl;

    int choice = -1;
    while (choice != 9) {
        cout << endl;
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {

        case 1:

            cout << "Enter complete CSV file path: " << endl;

            //cin >> csvPath;
            cin.ignore();

            // Reads the file path with spaces using getline()
            getline(cin, csvPath);

            // Check if quotes are present and remove them if necessary
            if (!csvPath.empty() && csvPath.front() == '"' && csvPath.back() == '"') {
                csvPath = csvPath.substr(1, csvPath.length() - 2);
            }

            // function to load the course
            loadCourses(csvPath, bst);
            break;

        case 2:

            cout << "Here is a sample schedule: " << endl;
            bst->InOrder();
            break;

        case 3:

            cout << "What course do you want to know about? ";

            cin >> courseNumber;

            cout << endl;

            course = bst->Search(courseNumber);

            if (!course.courseNumber.empty()) {
                displayCourse(course);
            }
            else {
                cout << "Course number " << courseNumber << " not found." << endl;
            }

            break;

        // The message that is displayed if user does not input an option in the switch case
        default: 
            cout << choice << " is not a valid option." << endl;
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard input buffer
        }
    }

    cout << "Thank you for using the course planner!" << endl;

    return 0;
}