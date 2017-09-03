#include <iostream>
#include <string>
using namespace std;

class GradeBook{
public:
	void setCourseName(string name);
	void setLetterGrade(string g);
	string getCourseName() const;
	string getLetterGrade() const;
	void displayMessage() const;
private:
	string courseName;
	string letterGrade;
};

void GradeBook::setCourseName(string name){ courseName = name; }

void GradeBook::setLetterGrade(string g){ letterGrade = g; }

string getCourseName() const { return courseName; }

string getLetterGrade() const { return letterGrade; }

void displayMessage() const {
	cout << "GradeBook for " << this->getCourseName() << endl;
	cout << "Letter grade is " << this->getLetterGrade() << endl;
}


int main(){
	GradeBook a;
	cout << "Input course name: ";
	a.setCourseName(cin.get());
	cout << "Input current letter grade: ";
	a.setLetterGrade(cin.get());

	cout << endl << a.displayMessage() << endl;

	GradeBook b;
	cout << "Next couse name: "
	b.setCourseName(cin.get());
	cout << "Next current letter grade: "
	b.setLetterGrade(cin.get());

	cout << endl << b.displayMessage() << endl;


	return 0;
}