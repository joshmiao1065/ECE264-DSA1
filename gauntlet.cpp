#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>

using namespace std;

// Helper function to generate a random integer
int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Helper function to generate a random double
double randomDouble(double min, double max) {
    double range = max - min;
    return min + ((double)rand() / RAND_MAX) * range;
}

// Helper function to generate a random string
string randomString(int length) {
    string result;
    string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < length; ++i) {
        result += charset[randomInt(0, charset.size() - 1)];
    }
    return result;
}

// Function to generate a random command
string generateCommand(const vector<string>& existingNames) {
    stringstream command;
    int commandType = randomInt(0, 2); // 0=create, 1=push, 2=pop

    if (commandType == 0 || existingNames.empty()) {
        // Create command
        char type = "ids"[randomInt(0, 2)]; // 'i', 'd', or 's'
        string name = type + randomString(randomInt(1, 5));
        string structure = (randomInt(0, 1) == 0) ? "stack" : "queue";
        command << "create " << name << " " << structure;
    } else if (commandType == 1) {
        // Push command
        string name = existingNames[randomInt(0, existingNames.size() - 1)];
        char type = name[0];
        if (type == 'i') {
            command << "push " << name << " " << randomInt(-1000, 1000);
        } else if (type == 'd') {
            command << "push " << name << " " << randomDouble(-1000.0, 1000.0);
        } else { // type == 's'
            command << "push " << name << " " << randomString(randomInt(3, 10));
        }
    } else {
        // Pop command
        string name = existingNames[randomInt(0, existingNames.size() - 1)];
        command << "pop " << name;
    }
    return command.str();
}

int main() {
    srand(time(0));

    // Output file
    string outputFilename = "random_commands.txt";
    ofstream outFile(outputFilename);

    if (!outFile) {
        cerr << "Error: Could not open file for writing!" << endl;
        return 1;
    }

    // Vector to track existing names
    vector<string> existingNames;

    // Generate one million commands
    for (int i = 0; i < 1000000; ++i) {
        string command = generateCommand(existingNames);

        // Add the name to the list if it's a "create" command
        if (command.find("create") == 0) {
            size_t spacePos = command.find(' ');
            size_t secondSpacePos = command.find(' ', spacePos + 1);
            string name = command.substr(spacePos + 1, secondSpacePos - spacePos - 1);
            existingNames.push_back(name);
        }

        outFile << command << endl;
    }

    outFile.close();
    cout << "Generated 1,000,000 commands in " << outputFilename << endl;
    return 0;
}
