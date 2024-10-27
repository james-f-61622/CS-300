#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <time.h>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// Define a structure to hold course information
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;

    Course() {}

    Course(string number, string title) : courseNumber(number), courseTitle(title) {}
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
    Node(Course aCourse) : Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string courseNumber);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Remove(string courseNumber);
    Course Search(string courseNumber);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    while (root != nullptr) {
        root = removeNode(root, root->course.courseNumber);
    }
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        addNode(root, course);
    }
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseNumber) {
    root = removeNode(root, courseNumber);
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseNumber) {
    Node* current = root;
    while (current != nullptr) {
        if (current->course.courseNumber == courseNumber) {
            return current->course;
        }
        if (courseNumber < current->course.courseNumber) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    return Course();
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    if (course.courseNumber < node->course.courseNumber) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            addNode(node->right, course);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
        inOrder(node->right);
    }
}

/**
 * Remove a course from some node (recursive)
 */
Node* BinarySearchTree::removeNode(Node* node, string courseNumber) {
    if (node == nullptr) {
        return node;
    }
    if (courseNumber < node->course.courseNumber) {
        node->left = removeNode(node->left, courseNumber);
    }
    else if (courseNumber > node->course.courseNumber) {
        node->right = removeNode(node->right, courseNumber);
    }
    else {
        if (node->left == nullptr) {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        else if (node->right == nullptr) {
            Node* temp = node->left;
            delete node;
            return temp;
        }
        Node* temp = node->right;
        while (temp->left != nullptr) {
            temp = temp->left;
        }
        node->course = temp->course;
        node->right = removeNode(node->right, temp->course.courseNumber);
    }
    return node;
}

//============================================================================
// Function to load courses from the file
//============================================================================
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading file " << csvPath << endl;

    ifstream inputFile(csvPath);
    if (!inputFile.is_open()) {
        cout << "Error: File cannot be opened." << endl;
        return;
    }

    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string courseNumber, courseTitle;
        getline(ss, courseNumber, ',');
        getline(ss, courseTitle, ',');

        Course newCourse(courseNumber, courseTitle);

        string prerequisite;
        while (getline(ss, prerequisite, ',')) {
            if (!prerequisite.empty()) {
                newCourse.prerequisites.push_back(prerequisite);
            }
        }

        bst->Insert(newCourse);
    }

    inputFile.close();
    cout << "Courses loaded successfully." << endl;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course) {
    cout << course.courseNumber << ", " << course.courseTitle << endl;
    if (!course.prerequisites.empty()) {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
    else {
        cout << "No prerequisites." << endl;
    }
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "CS-300_ABCU_Advising_Program_Input.csv";
    }

    // Define a binary search tree to hold all courses
    BinarySearchTree* bst = new BinarySearchTree();
    Course course;

    int choice = 0;
    while (choice != 9) {
        cout << "\nWelcome to the course planner." << endl;
        cout << "\n1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "\nWhat would you like to do? ";
        cin >> choice;

        switch (choice) {
        case 1:
            loadCourses(csvPath, bst);
            break;

        case 2:
            cout << "\nHere is a sample schedule:" << endl;
            bst->InOrder();
            break;

        case 3: {
            string courseNumber;
            cout << "\nWhat course do you want to know about? ";
            cin >> courseNumber;
            transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);

            course = bst->Search(courseNumber);

            if (!course.courseNumber.empty()) {
                displayCourse(course);
            }
            else {
                cout << "Course Number " << courseNumber << " not found." << endl;
            }
            break;
        }

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;
        default:
            cout << choice << " is not a valid option. Please try again." << endl;
        }
    }

    return 0;
}
