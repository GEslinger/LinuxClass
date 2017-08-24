#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

class GradeThing{
public:
	GradeThing();
	void setName();
	void setGrades();
	vector<int> getGrades() const;
	int sumGrades() const;
private:
	vector<int> grades;
	string name;
};

GradeThing::GradeThing(){
	grades = vector<int>(10);
}

void GradeThing::setName(){
	cout << "Enter Course Name!";
	cin >> name;
}

void GradeThing::setGrades(){
	cout << "Input all grades\n";
	for(int i = 0; i < grades.size(); i++){
		cin >> grades[i];
	}
	cout << "thanks\n";
}

vector<int> GradeThing::getGrades() const{
	return grades;
}

int GradeThing::sumGrades() const{
	int out = 0;
	for(int i = 0; i < grades.size(); i++){
		out += grades[i];
	}
	return out;
}



int main(){
	GradeThing physC;
	physC.setName();
	physC.setGrades();

	physC.getGrades();

	return 0;
}