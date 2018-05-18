//friend of a class is something that is completely seperated from the class but still has access to the stuffs inside the class//
#include<iostream>
using namespace std;

class harish
{
    public:
        harish()
        {
            jp=0;
        }
    private:
        int jp;
    friend void jyoti(harish &hc);
};

void jyoti(harish &hc)//even though the jyoti functionn is outside the class it still has access to the stuffs instide the class even the private variables as it is the friend of the class//
                      //that is it has access to all the members of the class including the private members of the class//
{
    hc.jp=99;
    cout <<hc.jp<<endl;
}
int main()
{
    harish bob;
    jyoti(bob);
}
