#ifndef TEST_CLASS_H
#define TEST_CLASS_H

class TestClass
{
public:
    TestClass(int a, int b);
    void print();

private:
    int regVar;         // regular variable
    const int constVar; // constant varibale//a constant variable has to be initialized using the constructor intializer list
};

#endif // TEST_CLASS_H
