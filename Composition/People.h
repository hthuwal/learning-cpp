#ifndef PEOPLE_H
#define PEOPLE_H
#include<iostream>
#include<cstdio>
#include<cstring>
#include "Birthday.h"
class People
{
    public:
        People(string x, Birthday bo);
        void printInfor();
    private:
        string name;
        Birthday dob;
};

#endif // PEOPLE_H
