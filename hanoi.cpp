#include<iostream>
#include<stack>
int x,count=0;
using namespace std;
stack <int> A,B,C,D,E,F;
void print()
{
    D=A;
    E=B;
    F=C;
    cout<<"Tower A -> ";
    while(!D.empty())
    {
        cout<<D.top()<<" ";
        D.pop();
    }
    cout<<endl;
    cout<<"Tower B -> ";
    while(!E.empty())
    {
        cout<<E.top()<<" ";
        E.pop();
    }
    cout<<endl;
    cout<<"Tower C -> ";
    while(!F.empty())
    {
        cout<<F.top()<<" ";
        F.pop();
    }
    cout<<endl;
    cout<<endl;
}
void hanoi(int n,stack <int> *A,stack <int> *B,stack <int> *C)
{
    if(n == x)
    {
        hanoi(n-2,A,C,B);
        (*B).push((*A).top());
        (*A).pop();
        count++;
        print();
        (*B).push((*A).top());
        (*A).pop();
        count++;
        print();
        hanoi(n-2,C,B,A);
    }
    else if(n>0)
    {
        hanoi(n-1,A,C,B);
        (*B).push((*A).top());
        (*A).pop();
        count++;
        print();
        hanoi(n-1,C,B,A);
    }
}
int main()
{
    int i,n;
    cin>>n;
    x=n;
    for(i=n-1;i>=0;i--)
    {
        A.push(i);
    }
    print();
    hanoi(n,&A,&B,&C);
    cout<<"Number of moves needed is : "<<count<<endl;
}
