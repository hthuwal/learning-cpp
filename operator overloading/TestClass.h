#ifndef TEST_CLASS_H
#define TEST_CLASS_H

class TestClass
{
public:
    int num;
    TestClass();                    // constructor if no argument is passed//
    TestClass(int);                 // constructor if integer argument is passed//
    TestClass operator+(TestClass); // operator overloading function
};

#endif // TEST_CLASS_H
