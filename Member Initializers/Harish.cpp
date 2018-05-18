#include "Harish.h"
#include<iostream>
using namespace std;
//the constructor intializer list is placed between the parantesis and the body !!!//
Harish::Harish(int a,int b)
: regVar(a),constVar(b)
{
}
void Harish::print()
{
    cout<<"regular variable is "<<regVar<<endl;
    cout<<"constant variable is "<<constVar<<endl;
}
