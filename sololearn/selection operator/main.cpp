#include<iostream>
#include "Selection.h"
using namespace std;
int main()
{
    Selection obj;
    Selection *ptr = &obj;  //decleraing a pointer ptr pointing to an obj of class Selection
    obj.Select();       //accessing Select function with obj
    ptr->Select();      //same as previous statement using pointer "->" this is called member selection operator
}
