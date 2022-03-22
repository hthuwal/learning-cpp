#include "TestClass.h"
#include <iostream>
using namespace std;

TestClass::TestClass()
{
}
TestClass::TestClass(int a)
{
    num = a;
}
TestClass TestClass::operator+(TestClass hc)
{
    return TestClass(this->num + hc.num);
}
