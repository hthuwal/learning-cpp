#include <iostream>
#include "Birthday.h"
#include "People.h"
using namespace std;
int main()
{
    Birthday birthObj(10, 01, 1994);
    People peopleobj("Harish", birthObj);
    peopleobj.printInfor();
}
