#include <iostream>
#include <fstream>
using namespace std;
class Contact
{
private:
	string name;
	long int number;

public:
	Contact()
	{
		name = "";
		number = 0;
	}
	Contact(string name, long int number)
	{
		this->name = name;
		this->number = number;
	}
	void setname(string c)
	{
		name = c;
	}
	void setnumber(long int n)
	{
		number = n;
	}
	string getname()
	{
		return name;
	}
	long int getnumber()
	{
		return number;
	}
	friend ofstream &operator<<(ofstream &t, Contact &c)
	{
		t.write((char *)&c, sizeof(Contact));
		return t;
	}
	friend istream &operator>>(istream &t, Contact &c)
	{
		Contact temp;
		t.read((char *)&temp, sizeof(Contact));
		c = Contact(temp.getname(), temp.getnumber());
		return t;
	}
};
void add(char *filename)
{

	string name;
	long int number;
	ofstream os;
	os.open(filename, ios::binary | ios::app);

	cout << "Enter Name of contact : ";
	cin >> name;
	cout << "Enter Mobile Number : ";
	cin >> number;

	Contact c(name, number);

	os << c;
	os.close();
}

void copydatabase(char *f1, char *f2)
{
	ofstream os;
	ifstream is;
	os.open(f2, ios::binary);
	is.open(f1, ios::binary);
	while (!is.eof())
	{
		Contact c;
		is >> c;
		cout << c.getname() << endl;
	}
	os.close();
	is.close();
}
int main()
{
	string mainfile = "database.txt";
	string extra = "new.txt";
	add("database.txt");
	copydatabase("database.txt", "new.txt");
}
