#include <vector>
#include <string>
using namespace std;

class Student{
public:
	Student(string f, string l);
	string fName() const;
	string lName() const;
	void printGrades();
	void addGrade(double g);
	bool delGrade(int g);
	bool changeGrade(int g, double newG);
	double getAvg() const;
private:
	string firstName;
	string lastName;
	vector<double> grades;
};


Student::Student(string f, string l){
	firstName = f;
	lastName = l;
}

string Student::fName()const{ return firstName; }
string Student::lName()const{ return lastName; }

void Student::printGrades(){
	if(grades.size() == 0){
		cout << "No grades\n";
		return;
	}

	for(int i = 0; i < grades.size(); i++){
		cout << i << ":" << grades[i] << "  ";
	}

	cout << endl;
}

void Student::addGrade(double g){
	cout << "Giving " << firstName << " " << lastName << " a " << g << endl;
	grades.push_back(g);
}

bool Student::delGrade(int j){
	if(j >= grades.size()){
		cout << "No grade of that index!\n";
		return false;
	};

	cout << "Removing " << firstName << " " << lastName << "'s " << grades[j] << endl;

	grades.erase(grades.begin()+j);
	return true;
}

bool Student::changeGrade(int g, double newG){
	if(!this->delGrade(g)){
		return false;
	}
	
	cout << "And replacing it with a " << newG << endl;
	grades.insert(grades.begin()+g,newG);

	return true;
}

double Student::getAvg() const {
	double avg = 0;

	for(int i = 0; i < grades.size(); i++){
		avg += grades[i];
	}

	avg /= grades.size();
	return avg;
}