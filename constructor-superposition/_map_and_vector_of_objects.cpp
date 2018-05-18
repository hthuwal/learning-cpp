#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Harish{
public:
    int *a;

    Harish() {
        this->a = new int;
        cout<<"Default Constructor...\n";
    }
    Harish(int a){
        this->a = new int;
        *(this->a) = a;
        cout<<"Parameterized constructor...\n";
    }

    // Copy Constructor
    Harish(const Harish& b) {
        this->a = b.a;
        cout<<"Copy constructor...\n";
    }

    // Move Constructor
    Harish(const Harish&& obj) {
        this->a = obj.a;
        cout<<"Move Constructor... \n";
    }

    int geta() {
        return (*a);
    }
};

int main() {
    // cout << "\n################## Map ##################\n\n";
    // map<string, Harish> a;
    // a["z"]=Harish(79);

    cout << "\n################## Vector ##################\n\n";
    vector<Harish> y;
    y.push_back(Harish(63));
}

