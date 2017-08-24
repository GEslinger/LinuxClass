#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

class Person{
public:
	Person(string s, int a, float h);
	virtual void grow(int y);
	virtual float getHeight() const;
	virtual int getAge() const;
	virtual string getName() const;
public:
	string name;
	int age;
	float height;
};

Person::Person(string s, int a, float h){
	name = s;
	age = a;
	height = h;
}

void Person::grow(int y){
	age += y;
	height += ((8-height)*y + height*(1/y))/2;

	if(age > 90){
		cout << "DEAD";
	}
}

float Person::getHeight() const{ return height; }

int Person::getAge() const{ return age; }

string Person::getName() const{ return name; }


class Employee : public Person{
public:
	Employee(string s, int a, float h, string j, int dolla);
private:
	string job;
	int salary;
};

Employee::Employee(string s, int a, float h, string j, int dolla)
: Person(s,a,h){
	job = j;
	salary = dolla;
};


class Manager : public Employee{
public:
	Manager(string s, int a, float h, int dolla);
};

Manager::Manager(string s, int a, float h, int dolla)
: Employee(s,a,h,"Manager",dolla){}


int main(){

	vector<Person*> people;

	people.push_back(new Person("Griffin Eslinger", 17, 6.12));
	people.push_back(new Employee("Jon Shannon", 54, 6.3, "IT chump", 120000));
	people.push_back(new Manager("Elon Musk", 32, 5.9, 500000));

	for(int i = 0; i < people.size(); i++){
		people[i]->grow(5);
	}

	return 0;
}

