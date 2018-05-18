#include "TestClass.h"
#include <iostream>
using namespace std;

TestClass::TestClass(int num) : h(num)
{
}

void TestClass::printCrap()
{
    cout << "h=" << h << endl;
    cout << "this->h=" << this->h << endl; // the keyword this stores the address of the corrent object we are working with)
    cout << "(*this).h=" << (*this).h << endl;
}
