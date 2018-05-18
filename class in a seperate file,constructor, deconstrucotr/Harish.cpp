//here we are going to actually build the function.It contains body of the class//
#include "Harish.h"
#include <iostream>
// :: the binary scope resolution operator.this operator implies Harish function (second one) belongs to the class Harish. for example demo::test() implies the function test belongs to class demo.//
using namespace std;
Harish::Harish()//the constructor description or body//automatically called on creation of object//
{
    cout<<"i am the constructor!"<<endl;
}
Harish::~Harish()//the deconstrucor body//destructor can never hava return value nor arguments//no deconstructor overloading//destructor function is called automatically on deletion or destroyed of object//
{
    cout<<"i am the deconstructor!"<<endl;
}
