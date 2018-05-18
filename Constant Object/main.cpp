#include<iostream>
#include "Harish.h"
using namespace std;
int main()
{
    int x = 3; //we set up value of x =3;
    x=5; // we changed it to 5
    cout << x <<endl; // this would print 5

    const int y=3;//constant variable y = 3 and its value can't be changed through out the program
    //y = 5; //error assignment of read-only variable 'y'
    //cout << 5 <<endl;

    //we cannot call regular functins with constant objects they need constant functions//
    Harish jyoti;
    jyoti.hc();
    const Harish jp;//declaring a constant object//
//jp.hc();//gives a error for a constant object we are calling a regulat function//we need to have a constant function//
    jp.pandey(); //this won't give an error because we have defined pandey as a constant function and jp is a constant object//
}
