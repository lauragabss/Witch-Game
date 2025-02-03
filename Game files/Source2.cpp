#include <cstdio>
#include <iostream>

using namespace std;

class Animal() 
{
	public:
		string name;
		void PrintName();
	protected:
		float size;
	private:
		int age;
		string gender;
}


class Cat : public Animal()
{
	public:
		string name = "Cat"; 
}

class Dog : public Animal() 
{
public:
	string name = "Dog";
	void PrintName();
}

void Animal::PrintName()
{
	cout << name << endl;
}

int main() 
{
	printf("0");
	return 0;
}
