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

string GradeBook::getCourseName() const { return courseName; }

string GradeBook::getLetterGrade() const { return letterGrade; }

void GradeBook::displayMessage() const {
	cout << "GradeBook for " << this->getCourseName() << endl;
	cout << "Letter grade is " << this->getLetterGrade() << endl;
}


int main(){
	string in;

	GradeBook a;
	cout << "Input course name: ";
	cin >> in;
	a.setCourseName(in);
	cout << "Input current letter grade: ";
	cin >> in;
	a.setLetterGrade(in);

	cout << endl;
	a.displayMessage();
	cout << endl;

	GradeBook b;
	cout << "Next couse name: ";
	cin >> in;
	b.setCourseName(in);
	cout << "Next current letter grade: ";
	cin >> in;
	b.setLetterGrade(in);

	cout << endl;
	b.displayMessage();
	cout << endl;


	return 0;
}