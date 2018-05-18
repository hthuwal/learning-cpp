#include "Birthday.h"
#include<iostream>
using namespace std;
//the constructor intializer list is placed between the parantesis and the body !!!//
Birthday::Birthday(int m,int d,int y)
{
    month=m;
    day=d;
    year=y;
}
void Birthday::printDate()
{
    cout<<month<<"/"<<day<<"/"<<year<<endl;
}
