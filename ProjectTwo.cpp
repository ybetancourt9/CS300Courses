//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Yaumel Betancourt
// Version     : 1.0
// Copyright   : Copyright   2025 SNHU COCE
// Description : Project Two Courses List
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Course {
    string number; // unique identifier
    string name;
    vector<string> prerequisites;
    Course() {
        name = "PLACEHOLDER"; // default course name used when course is not found
    }
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

    // initialize with a bid
    Node(Course course) :
        Node() {
        this->course = course;
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

public:
    BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseNumber);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    inOrder(root);
}

/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {
        Node* newNode = new Node(course);
        root = newNode;
    }
    else {
        Node* currNode = root;
        addNode(currNode, course);
    }
}

/**
 * Search for a bid
 */
Course BinarySearchTree::Search(string courseNumber) {
    Node* currNode = root;
    while (currNode != nullptr) {
        if (currNode->course.number == courseNumber) {
            return currNode->course;
        }
        else if (courseNumber < currNode->course.number) {
            currNode = currNode->left;
        }
        else {
            currNode = currNode->right;
        }
    }
    Course course;
    return course;
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    if (course.number < node->course.number) {
        if (node->left == nullptr) {
            Node* newNode = new Node(course);
            node->left = newNode;
        }
        else {
            addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            Node* newNode = new Node(course);
            node->right = newNode;
        }
        else {
            addNode(node->right, course);
        }
    }
}
void BinarySearchTree::inOrder(Node* node) { // print all courses in alphanumeric order
    if (node != nullptr) {
        inOrder(node->left);

        cout << node->course.number + " " + node->course.name;
        if (!node->course.prerequisites.empty()) {
            vector<string> prerequisites = node->course.prerequisites;
            cout << " | Prerequisites:";
            for (string courseName : prerequisites)
            {
                cout << " " + courseName;
            }
        }
        cout << endl;

        inOrder(node->right);
    }

}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayCourse(Course course) {
    cout << course.number + " " + course.name;
    if (!course.prerequisites.empty()) {
        vector<string> prerequisites = course.prerequisites;
        cout << " | Prerequisites:";
        for (string prereq : prerequisites) {
            cout << " " + prereq;
        }
    }
    cout << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    try {
        std::ifstream file(csvPath);
        std::string line;
        // loop to read rows of a CSV file
        while(std::getline(file, line)) {

            // Create a data structure and add to the collection of bids
            vector<std::string> courseDetails;
            std::stringstream ss(line);
            string courseDetail;

            while(std::getline(ss, courseDetail, ',')) {
                courseDetails.push_back(courseDetail);
            }

            Course course;
            course.number = courseDetails[0];
            course.name = courseDetails[1];
            // populate prerequisite vector
            if (courseDetails.size() > 2) {
                for (int i = 2; i < courseDetails.size(); ++i) {
                    course.prerequisites.push_back(courseDetails[i]);
                }
            }

            // add this course to BST
            bst->Insert(course);
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseNumber;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        courseNumber = argv[2];
        break;
    case 3:
        csvPath = argv[1];
        courseNumber = argv[2];
        break;
    default:
        csvPath = "CS 300 ABCU_Advising_Program_Input.csv";
        courseNumber = "CSCI100";
    }

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Print Courses" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            // Complete the method call to load the bids
            loadCourses(csvPath, bst);
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            course = bst->Search(courseNumber);

            if (!(course.name == "PLACEHOLDER")) {
                displayCourse(course);
            }
            else {
                cout << "Course " << courseNumber << " not found." << endl;
            }

            break;

        default:
            cout << "Choice " << choice << " is not an valid option." << endl;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
