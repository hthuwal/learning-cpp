// the reason why we have the class file in two components that is the .h header file and .cpp the source file is that while distributing we give only the prototypes that is the header file not the complete description as we dont want anyone to change it :) //
#include<iostream>
#include "Harish.h"
using namespace std;
int main(void)
{
    Harish hc;
    cout<<"omg wtf is this on my shoe?"<<endl;
    //whenever we reach the end of the program the c++ automatically destroys all the objects created//there by calling the destructor if exists//
}
