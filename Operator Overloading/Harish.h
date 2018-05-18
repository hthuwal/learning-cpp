#ifndef HARISH_H
#define HARISH_H


class Harish
{
    public:
        int num;
        Harish();//constructor if no argument is passed//
        Harish(int);//constructor if integer argument is passed//
        Harish operator+(Harish);  //operator overloading function
};

#endif // HARISH_H
