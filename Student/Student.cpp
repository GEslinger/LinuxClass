#include <iostream>
#include <string>
#define dbg(x) cerr << #x << " is " << x << endl
using namespace std;

class Student{
public:
	string getFirstName() const;
	string getLastName() const;
	double getGrade() const;
	void setFirstName(string n);
	void setLastName(string n);
	void setGrade(double g);
	void printInfo();
private:
	string fn;
	string ln;
	double grade;
};

string Student::getFirstName() const { return fn; }
string Student::getLastName() const { return ln; }
double Student::getGrade() const { return grade; }

void Student::setFirstName(string n){ fn = n; }
void Student::setLastName(string n){ ln = n; }
void Student::setGrade(double g){ grade = g; } 

void Student::printInfo(){
	cout << this->getFirstName() << " " << this->getLastName();
	cout << " has a grade of " << this->getGrade() << endl;
}


int main(){
	Student s;
	string in;
	double in2;

	cout << "Enter first name: ";
	cin >> in;
	s.setFirstName(in);

	cout << "Enter last name: ";
	cin >> in;
	s.setLastName(in);

	cout << "Enter grade: ";
	cin >> in2;
	s.setGrade(in2);

	cout << endl;
	s.printInfo();
	cout << endl;

	return 0;
}