/*Josh Miao DSA1 Project 2.
The program uses linked lists through the SimpleList abstract base class
COmmand processing is centralized in the processCommand fucntion which dynamically
selects the correct list based on data type and executes the appropriate
action based on the input.Error handling acknowledges errors for the requested
invalid operations. */

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <memory>
#include <stdexcept>
#include <sstream>

using namespace std;

// Abstract base class: SimpleList
template <typename T>
class SimpleList {
protected:
    class Node {
    public:
        T data;
        Node* next;
        Node(const T& d, Node* n = nullptr) : data(d), next(n) {}
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    string name;

    // Helper methods for linked list operations
    void insertAtStart(const T& value) {
        Node* newNode = new Node(value, head);
        if (!head) tail = newNode;
        head = newNode;
    }

    void insertAtEnd(const T& value) {
        Node* newNode = new Node(value);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    T removeFromStart() {
        if (!head) throw runtime_error("ERROR: This list is empty!");
        Node* temp = head;
        T value = head->data;
        head = head->next;
        if (!head) tail = nullptr;
        delete temp;
        return value;
    }

public:
    SimpleList(const string& n) : name(n) {}
    virtual ~SimpleList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    string getName() const { return name; }
    virtual void push(const T& value) = 0;
    virtual T pop() = 0;
};

// Stack implementation
template <typename T>
class Stack : public SimpleList<T> {
public:
    Stack(const string& n) : SimpleList<T>(n) {}

    void push(const T& value) override {
        this->insertAtStart(value);
    }

    T pop() override {
        return this->removeFromStart();
    }
};

// Queue implementation
template <typename T>
class Queue : public SimpleList<T> {
public:
    Queue(const string& n) : SimpleList<T>(n) {}

    void push(const T& value) override {
        this->insertAtEnd(value);
    }

    T pop() override {
        return this->removeFromStart();
    }
};

// Function to find a SimpleList by name
template <typename T>
SimpleList<T>* findList(list<SimpleList<T>*>& lists, const string& name) {
    for (SimpleList<T>* l : lists) { // Explicit type instead of auto
        if (l->getName() == name) return l;
    }
    return nullptr;
}

// Helper function to process a "create" command
template <typename T>
void handleCreate(list<SimpleList<T>*>& lists, const string& name, const string& type) {
    if (findList(lists, name)) {
        throw runtime_error("ERROR: This name already exists!");
    } else if (type == "stack") {
        lists.push_front(new Stack<T>(name));
    } else if (type == "queue") {
        lists.push_front(new Queue<T>(name));
    }
}

// Helper function to process a "push" command
template <typename T>
void handlePush(list<SimpleList<T>*>& lists, const string& name, const string& value) {
    SimpleList<T>* list = findList(lists, name);
    if (!list) {
        throw runtime_error("ERROR: This name does not exist!");
    } else {
        if constexpr (is_same_v<T, int>) {
            list->push(stoi(value));
        } else if constexpr (is_same_v<T, double>) {
            list->push(stod(value));
        } else {
            list->push(value); // Assumes T is string
        }
    }
}

// Helper function to process a "pop" command
template <typename T>
void handlePop(list<SimpleList<T>*>& lists, const string& name, ostream& output) {
    SimpleList<T>* list = findList(lists, name);
    if (!list) {
        throw runtime_error("ERROR: This name does not exist!");
    } else {
        try {
            T value = list->pop();
            output << "Value popped: " << value << '\n';
        } catch (const runtime_error&) {
            throw runtime_error("ERROR: This list is empty!");
        }
    }
}

// Function to process a single command
void processCommand(const string& command, list<SimpleList<int>*>& intLists, list<SimpleList<double>*>& doubleLists, 
                    list<SimpleList<string>*>& stringLists, ostream& output) {
    istringstream cmdStream(command);
    string operation, name, typeOrValue;
    cmdStream >> operation >> name;

    char type = name[0];
    try {
        if (operation == "create") {
            cmdStream >> typeOrValue;
            if (type == 'i') {
                handleCreate(intLists, name, typeOrValue);
            } else if (type == 'd') {
                handleCreate(doubleLists, name, typeOrValue);
            } else {
                handleCreate(stringLists, name, typeOrValue);
            }
        } else if (operation == "push") {
            cmdStream >> typeOrValue;
            if (type == 'i') {
                handlePush(intLists, name, typeOrValue);
            } else if (type == 'd') {
                handlePush(doubleLists, name, typeOrValue);
            } else {
                handlePush(stringLists, name, typeOrValue);
            }
        } else if (operation == "pop") {
            if (type == 'i') {
                handlePop(intLists, name, output);
            } else if (type == 'd') {
                handlePop(doubleLists, name, output);
            } else {
                handlePop(stringLists, name, output);
            }
        }
    } catch (const runtime_error& e) {
        output << e.what() << '\n';
    }
}

//This was the shortest I was able to make my main function. 
int main() {
    string inputFileName, outputFileName;
    cout << "Enter name of input file: ";
    cin >> inputFileName;
    cout << "Enter name of output file: ";
    cin >> outputFileName;

    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    list<SimpleList<int>*> intLists;
    list<SimpleList<double>*> doubleLists;
    list<SimpleList<string>*> stringLists;

    string command;
    while (getline(inputFile, command)) {
        outputFile << "PROCESSING COMMAND: " << command << '\n';
        processCommand(command, intLists, doubleLists, stringLists, outputFile);
    }

    // Cleanup
    for (SimpleList<int>* l : intLists) delete l;
    for (SimpleList<double>* l : doubleLists) delete l;
    for (SimpleList<string>* l : stringLists) delete l;

    return 0;
}
