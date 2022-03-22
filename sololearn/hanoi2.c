#include <stdio.h>
#include <malloc.h>
#include <process.h>
struct node
{
	int info;
	struct node *link;
};
struct node *temp, *nw;
void printhc()
{
}
void print(struct node *top)
{
	temp = top;
	while (1)
	{
		printf("||%d||\n", temp->info);
		if (temp->link == NULL)
			break;
		temp = temp->link;
	}
}
int push(int k, struct node *top)
{
	if (top == NULL)
	{
		nw = (struct node *)malloc(sizeof(struct node));
		nw->info = k;
		nw->link = NULL;
		top = nw;
	}
	else
	{
		nw = (struct node *)malloc(sizeof(struct node));
		nw->info = k;
		nw->link = top;
		top = nw;
	}
}
int pop(struct node *top)
{
	int x;
	if (top == NULL)
		printf("Underflow");
	else if (top->link == NULL)
	{
		x = top->info;
		free(top);
		top = NULL;
		printf("Stack Empty");
	}
	else
	{
		x = top->info;
		temp = top;
		top = top->link;
		free(temp);
	}

	temp = top;
	return x;
}
void hanoi(int n, struct node *A, sturct node *B, sturct node *C)
{
	if (n == x)
	{
		hanoi(n - 2, A, C, B);
		(*B).push((*A).top());
		(*A).pop();
		count++;
		print();
		(*B).push((*A).top());
		(*A).pop();
		count++;
		print();
		hanoi(n - 2, C, B, A);
	}
	else if (n > 0)
	{
		hanoi(n - 1, A, C, B);
		(*B).push((*A).top());
		(*A).pop();
		count++;
		print();
		hanoi(n - 1, C, B, A);
	}
}
int main()
{
	int i, n;
	scanf("%d", &n);
	x = n;
	for (i = n - 1; i >= 0; i--)
	{
		A.push(i);
	}
	print();
	hanoi(n, &A, &B, &C);
	cout << "Number of moves needed is : " << count << endl;
}
