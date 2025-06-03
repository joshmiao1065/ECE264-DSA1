/*Josh Miao DSA1 Program 1
    Created base foobar class then dervied classes. Read input files and populates
    a foobar vector with their names, positions, and strengths

    Updated each foobar's position based on the order it appears in the input file
    
    Wrote output file displaying name then strength in appropriate format
    
    Cleaned dynamically allocated memory
*/

#include <iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>

//Base class Foobar
class Foobar {
    private:
        //only string name and position should be private
        std::string name;
        int position = 0;

    public:
        Foobar(const std::string foobar_name) : name(foobar_name), position(0) {}

        virtual int get_strength() const {
            //base foobar strength = position
            return position;
        }
        std::string get_name() const {
            return name;
        }

        void set_position(int pos) {
            position = pos;
        }

    protected:
        int get_position() const {
            return position;
        }
};

//Dervied Foo class
class Foo: public Foobar {
    public:
        Foo(const std::string foo_name) : Foobar(foo_name) {}
        
        int get_strength() const override {
            return get_position() * 3;
        }
};

//Derived Bar class
class Bar: public Foobar {
    public:
        Bar(const std::string bar_name) : Foobar(bar_name) {}

        int get_strength() const override {
            return get_position() + 15;
        }
};

//Read input files and populate foobar vector
void read_input_file(const std::string& input_file_name, std:: vector<Foobar*>& foobars) {
    std::ifstream input_file(input_file_name);
    std::string line;

    while(std::getline(input_file, line)) {
        std::stringstream ss(line);
        std::string type, name;
        ss >> type >> name;
        /*use pointer casting when add new instances of foobars rather 
        than sticking it in vector to ensure a vector of foobars*/
        if (type == "foobar") {
            foobars.push_back(static_cast<Foobar*>(new Foobar(name)));
        }
        else if (type == "foo") {
        foobars.push_back(static_cast<Foobar*>(new Foo(name)));
        }
        else if (type == "bar") {
            foobars.push_back(static_cast<Foobar*>(new Bar(name)));
        }
    }
    input_file.close();
}

//update position function
void update_positions(std::vector<Foobar*>& foobars) {
    int position = 1;
    for(int i = foobars.size() - 1; i >= 0; i--) {
        foobars[i]->set_position(position++);
    }
}

//Funciton to write foobars' data to output file
void write_output_file(const std::string& output_file_name, const std::vector<Foobar*>& foobars) {
    std::ofstream output_file(output_file_name);
    for(int i = 0; i < foobars.size(); i++) {
        output_file << foobars.at(i)->get_name() << " " << foobars.at(i)->get_strength() << std::endl;
    }
    output_file.close();
}

int main() {
    std::vector<Foobar*> foobars;
    std::string input_file_name, output_file_name;

    //Get filenames from Sable
    std::cout << "Please enter the name of the input file: ";
    std::cin >> input_file_name;
    std::cout << "Please enter the name of the output file: ";
    std::cin >> output_file_name;

    //Process the input files
    read_input_file(input_file_name, foobars);
    
    //update foobar positions
    update_positions(foobars);

    //write output file
    write_output_file(output_file_name, foobars);

    //clean up dynamically allocated memory
    for (int i = 0; i< foobars.size(); i++) {
        delete foobars.at(i);
    }

    return 0;
}