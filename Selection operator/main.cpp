#include<iostream>
#include "Jp.h"
using namespace std;
int main()
{
    Jp Jpobject;
    Jp *Jppointer=&Jpobject;//decleraing a pointer Jppointer of pointing to an object Jpobject of class Jp//
    Jpobject.Jyoti();//accessing Jyoti function with object//
    Jppointer->Jyoti();//same as previous statement using pointer "->" this is called member selection operator//accessing Jyoti function with the pointer//
}
