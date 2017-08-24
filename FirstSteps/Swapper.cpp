#include <iostream>
using namespace std;

void swap(int &a, int &b){
	int tmp = a;
	a = b;
	b = tmp;
}

int main(){
	int first;
	int second;

	cout << "Enter some ints or something\n";

	cin >> first;
	cin >> second;

	cout << first << ", " << second << "\n";
	swap(first, second);
	cout << first << ", " << second << "\n";

	return 0;
}