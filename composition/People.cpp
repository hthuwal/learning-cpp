#include "People.h"
#include <iostream>
#include <string>
#include "Birthday.h"
using namespace std;
// everytime we use a class inside another class we need to use memeber initializer list to initialize object//

People::People(string x, Birthday bo) : name(x), dob(bo)
{
}

void People::printInfor()
{
    cout << name << " was born on ";
    dob.printDate();
}
