#include "Harish.h"
#include<iostream>
using namespace std;
Harish::Harish()
{
}
Harish::Harish(int a)
{
    num=a;
}
Harish Harish::operator+(Harish hc)
{
    Harish jp;
    jp.num = num + hc.num;
}
