#include <iostream>
#include "TestClass.h"
using namespace std;
int main()
{
    int x = 3;         // we set up value of x =3;
    x = 5;             // we changed it to 5
    cout << x << endl; // this would print 5

    const int y = 3; // constant variable y = 3 and its value can't be changed through out the program
    // y = 5; //error assignment of read-only variable 'y'
    // cout << 5 <<endl;

    // we cannot call regular functins with constant objects they need constant functions
    TestClass obj;
    obj.TestMethod1();

    const TestClass const_obj; // declaring a constant object//
    // const_obj.TestMethod1();   // gives a error for a constant object we are calling a regulaR function
    const_obj.TestMethod2(); // this won't give an error because we have defined TestMethod2 as a constant function and const_obj is a constant object
}
