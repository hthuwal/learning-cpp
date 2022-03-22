#include <iostream>
using namespace std;
#include "TestClass.h"

int main()
{
    TestClass obj1(15);
    TestClass obj2(10);
    TestClass obj3 = obj1 + obj2;
    cout << obj3.num;
}
